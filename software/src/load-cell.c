/* load-cell-bricklet
 * Copyright (C) 2015 Olaf Lüke <olaf@tinkerforge.com>
 *
 * load-cell.c: Implementation of Load Cell Bricklet messages
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "load-cell.h"

#include "brickletlib/bricklet_entry.h"
#include "brickletlib/bricklet_simple.h"
#include "bricklib/utility/util_definitions.h"
#include "bricklib/bricklet/bricklet_communication.h"
#include "config.h"

#define I2C_EEPROM_ADDRESS_HIGH 84

#define HALF_CLOCK_400KHZ  1250  // 2.5us per clock

#define SIMPLE_UNIT_WEIGHT 0

const SimpleMessageProperty smp[] = {
	{SIMPLE_UNIT_WEIGHT, SIMPLE_TRANSFER_VALUE, SIMPLE_DIRECTION_GET}, // TYPE_GET_WEIGHT
	{SIMPLE_UNIT_WEIGHT, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_SET}, // TYPE_SET_WEIGHT_CALLBACK_PERIOD
	{SIMPLE_UNIT_WEIGHT, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_GET}, // TYPE_GET_WEIGHT_CALLBACK_PERIOD
	{SIMPLE_UNIT_WEIGHT, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_SET}, // TYPE_SET_WEIGHT_CALLBACK_THRESHOLD
	{SIMPLE_UNIT_WEIGHT, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_GET}, // TYPE_GET_WEIGHT_CALLBACK_THRESHOLD
	{0, SIMPLE_TRANSFER_DEBOUNCE, SIMPLE_DIRECTION_SET}, // TYPE_SET_DEBOUNCE_PERIOD
	{0, SIMPLE_TRANSFER_DEBOUNCE, SIMPLE_DIRECTION_GET}, // TYPE_GET_DEBOUNCE_PERIOD
};

const SimpleUnitProperty sup[] = {
	{NULL, SIMPLE_SIGNEDNESS_INT, FID_WEIGHT, FID_WEIGHT_REACHED, SIMPLE_UNIT_WEIGHT} // load-cell
};

const uint8_t smp_length = sizeof(smp);

void invocation(const ComType com, const uint8_t *data) {
	switch(((MessageHeader*)data)->fid) {
		case FID_GET_WEIGHT:
		case FID_SET_WEIGHT_CALLBACK_PERIOD:
		case FID_GET_WEIGHT_CALLBACK_PERIOD:
		case FID_SET_WEIGHT_CALLBACK_THRESHOLD:
		case FID_GET_WEIGHT_CALLBACK_THRESHOLD:
		case FID_SET_DEBOUNCE_PERIOD:
		case FID_GET_DEBOUNCE_PERIOD: {
			simple_invocation(com, data);
			break;
		}

		case FID_SET_MOVING_AVERAGE: {
			set_moving_average(com, (SetMovingAverage*)data);
			break;
		}

		case FID_GET_MOVING_AVERAGE: {
			get_moving_average(com, (GetMovingAverage*)data);
			break;
		}

		case FID_LED_ON: {
			led_on(com, (LEDOn*)data);
			break;
		}

		case FID_LED_OFF: {
			led_off(com, (LEDOff*)data);
			break;
		}

		case FID_IS_LED_ON: {
			is_led_on(com, (IsLEDOn*)data);
			break;
		}

		case FID_CALIBRATE: {
			calibrate(com, (Calibrate*)data);
			break;
		}

		case FID_TARE: {
			tare(com, (Tare*)data);
			break;
		}

		case FID_SET_CONFIGURATION: {
			set_configuration(com, (SetConfiguration*)data);
			break;
		}

		case FID_GET_CONFIGURATION: {
			get_configuration(com, (GetConfiguration*)data);
			break;
		}

		default: {
			BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_NOT_SUPPORTED, com);
			break;
		}
	}
}

void constructor(void) {
	_Static_assert(sizeof(BrickContext) <= BRICKLET_CONTEXT_MAX_SIZE, "BrickContext too big");

	PIN_DAT.type = PIO_INPUT;
	PIN_DAT.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&PIN_DAT, 1);

	PIN_CLK.type = PIO_OUTPUT_0;
	PIN_CLK.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&PIN_CLK, 1);

	PIN_LED.type = PIO_OUTPUT_1;
	PIN_LED.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&PIN_LED, 1);

	simple_constructor();

	BC->offset = 0;
	BC->gain_div = 1;
	BC->gain_mul = 1;
	BC->moving_average_upto = DEFAULT_MOVING_AVERAGE;
	BC->last_adc_value = 0;
	BC->current_gain = GAIN_128X;
	BC->current_rate = RATE_10HZ;
	BC->led_value = false;
	BC->tare_value = 0;

	reinitialize_moving_average();
	read_calibration_from_eeprom();
	read_configuration_from_eeprom();
	update_rate();
}

void destructor(void) {
	simple_destructor();
}

// Generate one bit of data with 400kHz speed, see hx711 datasheet page 5
bool clock_one_bit(void) {
	PIN_CLK.pio->PIO_SODR = PIN_CLK.mask; // clock high
	SLEEP_NS(HALF_CLOCK_400KHZ);
	bool ret = PIN_DAT.pio->PIO_PDSR & PIN_DAT.mask; // read bit
	PIN_CLK.pio->PIO_CODR = PIN_CLK.mask; // clock low
	SLEEP_NS(HALF_CLOCK_400KHZ);
	return ret;
}

void tick(const uint8_t tick_type) {
	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		if(!(PIN_DAT.pio->PIO_PDSR & PIN_DAT.mask)) {
			SLEEP_NS(100); // Wait 0.1us before first clock edge (see hx711 datasheet page 5, T_1)

			int32_t value = 0;
			for(uint8_t i = 0; i < 24; i++) {
				value |= clock_one_bit() << (23-i);
			}

			// 24 bit twos complement -> 32 bit twos complement
			if(value & 0x800000) {
				value |= 0xFF000000;
			}
			value = (1 << 24) - value;

			// 128x = 1 clock, 32x = 2 clock, 64x = 3 clock (see hx711 datasheet page 5, "next conversion")
			switch(BC->current_gain) {
				case GAIN_64X:  clock_one_bit();
				case GAIN_32X:  clock_one_bit();
				case GAIN_128X: clock_one_bit();
			}

			new_value(value);
		}
	}

	simple_tick(tick_type);
}

void update_rate(void) {
	if(BC->current_rate == RATE_10HZ) {
		PIN_RAT.type = PIO_OUTPUT_0;
	} else {
		PIN_RAT.type = PIO_OUTPUT_1;
	}

	PIN_RAT.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&PIN_RAT, 1);
}

void reinitialize_moving_average(void) {
	int32_t initial_value = BC->last_adc_value;
	for(uint8_t i = 0; i < BC->moving_average_upto; i++) {
		BC->moving_average[i] = initial_value;
	}
	BC->moving_average_tick = 0;
	BC->moving_average_sum = initial_value*BC->moving_average_upto;
}

// The gcc 64 bit division implementation is bigger than 4kb and thus
// does not fit on a Bricklet EEPROM. We implement our own slower but
// more space efficient division.
void div_64(uint64_t dividend, uint64_t divisor, uint64_t *result, uint64_t *remainder) {
	if(dividend == 0) {
		return;
	}

	*remainder = dividend;
	*result = 0;
	uint64_t mask = 1;

	while(divisor < *remainder) {
		divisor = divisor << 1;
	    mask = mask << 1;
	}
	do {
	    if(*remainder >= divisor) {
	    	*remainder = *remainder - divisor;
	        *result = *result + mask;
	    }
	    divisor = divisor >> 1;
	    mask = mask >> 1;
	} while(mask != 0);
}

void new_value(const int32_t value) {
	BC->moving_average_sum = BC->moving_average_sum -
	                         BC->moving_average[BC->moving_average_tick] +
	                         value;

	BC->moving_average[BC->moving_average_tick] = value;
	BC->moving_average_tick = (BC->moving_average_tick + 1) % BC->moving_average_upto;

	BC->last_adc_value = (BC->moving_average_sum + BC->moving_average_upto/2)/BC->moving_average_upto;

	int32_t offset_adc_value = BC->last_adc_value - BC->offset;
	bool sign = true;
	if(offset_adc_value < 0) {
		sign = false;
		offset_adc_value = -offset_adc_value;
	}

	// Use 64 bit arithmetic to calculate weight, this way we don't have to throw away precision
	uint64_t result = 0;
	uint64_t remainder = 0;
	div_64(((uint64_t)(offset_adc_value))*((uint64_t)BC->gain_mul), BC->gain_div, &result, &remainder);

	BC->last_value[0] = BC->value[0];

	if(sign) {
		BC->value[0] = result;

		// Round value up if necessary
		if(remainder > BC->gain_div/2) {
			BC->value[0]++;
		}
	} else {
		BC->value[0] = -result;
		// Round value down if necessary
		if(remainder > BC->gain_div/2) {
			BC->value[0]--;
		}
	}

	BC->value[0] += BC->tare_value;
}

void set_moving_average(const ComType com, const SetMovingAverage *data) {
	if(BC->moving_average_upto != data->length) {
		if(data->length < 1) {
			BC->moving_average_upto = 1;
		} else if(data->length > MAX_MOVING_AVERAGE) {
			BC->moving_average_upto = MAX_MOVING_AVERAGE;
		} else {
			BC->moving_average_upto = data->length;
		}

		reinitialize_moving_average();
	}

	BA->com_return_setter(com, data);
}

void get_moving_average(const ComType com, const GetMovingAverage *data) {
	GetMovingAverageReturn gmar;
	gmar.header        = data->header;
	gmar.header.length = sizeof(GetMovingAverageReturn);
	gmar.length        = BC->moving_average_upto;

	BA->send_blocking_with_timeout(&gmar, sizeof(GetMovingAverageReturn), com);
}

void led_on(const ComType com, const LEDOn *data) {
	BC->led_value = true;
	PIN_LED.type = PIO_OUTPUT_0;
	BA->PIO_Configure(&PIN_LED, 1);

	BA->com_return_setter(com, data);
}

void led_off(const ComType com, const LEDOff *data) {
	BC->led_value = false;
	PIN_LED.type = PIO_OUTPUT_1;
	BA->PIO_Configure(&PIN_LED, 1);

	BA->com_return_setter(com, data);
}

void is_led_on(const ComType com, const IsLEDOn *data) {
	IsLEDOnReturn iledor;
	iledor.header        = data->header;
	iledor.header.length = sizeof(IsLEDOnReturn);
	iledor.value         = BC->led_value;

	BA->send_blocking_with_timeout(&iledor, sizeof(IsLEDOnReturn), com);
}

void calibrate(const ComType com, const Calibrate *data) {
	if(data->weight == 0) {
		BC->offset = BC->last_adc_value;
	} else {
		int32_t offset_adc_value = BC->last_adc_value - BC->offset;

		if(offset_adc_value < 0) {
			offset_adc_value = 0;
		}

		BC->gain_mul = data->weight;
		BC->gain_div = offset_adc_value;
	}

	write_calibration_to_eeprom();

	BA->com_return_setter(com, data);
}

void tare(const ComType com, const Tare *data) {
	BC->tare_value += -BC->value[0];

	BA->com_return_setter(com, data);
}

void set_configuration(const ComType com, const SetConfiguration *data) {
	if((data->gain > 2) || (data->rate > 1)) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	BC->current_gain = data->gain;
	BC->current_rate = data->rate;

	write_configuration_to_eeprom();
	update_rate();

	BA->com_return_setter(com, data);
}

void get_configuration(const ComType com, const GetConfiguration *data) {
	GetConfigurationReturn gcr;
	gcr.header        = data->header;
	gcr.header.length = sizeof(GetConfigurationReturn);
	gcr.rate          = BC->current_rate;
	gcr.gain          = BC->current_gain;

	BA->send_blocking_with_timeout(&gcr, sizeof(GetConfigurationReturn), com);
}

void write_calibration_to_eeprom(void) {
	uint32_t cal[4] = {BC->offset, BC->gain_mul, BC->gain_div, 0xDEADBEEF};
	if(BC->gain_div == 0) {
		cal[2] = 1;
	}

	BA->bricklet_select(BS->port - 'a');
	BA->i2c_eeprom_master_write(BA->twid->pTwi,
	                            CALIBRATION_EEPROM_POSITION,
	                            (const char*)cal,
	                            4*4);
	BA->bricklet_deselect(BS->port - 'a');
}

void read_calibration_from_eeprom(void) {
	uint32_t cal[4];
	BA->bricklet_select(BS->port - 'a');
	BA->i2c_eeprom_master_read(BA->twid->pTwi,
	                           CALIBRATION_EEPROM_POSITION,
	                           (char*)cal,
	                           4*4);
	BA->bricklet_deselect(BS->port - 'a');

	if(cal[3] == 0xDEADBEEF) {
		BC->offset = cal[0];
		BC->gain_mul = cal[1];
		BC->gain_div = cal[2];
		if(BC->gain_div == 0) {
			BC->offset = 0;
			BC->gain_mul = 1;
			BC->gain_div = 1;
		}
	}
}

void write_configuration_to_eeprom(void) {
	uint8_t conf[6] = {BC->current_gain, BC->current_rate, 0xDE, 0xAD, 0xBE, 0xEF};
	BA->bricklet_select(BS->port - 'a');
	BA->i2c_eeprom_master_write(BA->twid->pTwi,
	                            CONFIGURATION_EEPROM_POSITION,
	                            (const char*)conf,
	                            6);
	BA->bricklet_deselect(BS->port - 'a');
}

void read_configuration_from_eeprom(void) {
	uint8_t conf[6];
	BA->bricklet_select(BS->port - 'a');
	BA->i2c_eeprom_master_read(BA->twid->pTwi,
	                           CONFIGURATION_EEPROM_POSITION,
	                           (char*)conf,
	                           6);
	BA->bricklet_deselect(BS->port - 'a');

	if(conf[2] == 0xDE && conf[3] == 0xAD && conf[4] == 0xBE && conf[5] == 0xEF) {
		BC->current_gain = conf[0];
		BC->current_rate = conf[1];
	}
}
