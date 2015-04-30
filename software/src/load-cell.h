/* load-cell-bricklet
 * Copyright (C) 2015 Olaf Lüke <olaf@tinkerforge.com>
 *
 * load-cell.h: Implementation of Load Cell Bricklet messages
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

#ifndef LOAD_CELL_H
#define LOAD_CELL_H

#include <stdint.h>

#include "bricklib/com/com_common.h"

#define GAIN_128X 0
#define GAIN_64X  1
#define GAIN_32X  2
#define RATE_10HZ 0
#define RATE_80HZ 1

#define CALIBRATION_EEPROM_POSITION (BRICKLET_PLUGIN_MAX_SIZE + 96)
#define CONFIGURATION_EEPROM_POSITION (BRICKLET_PLUGIN_MAX_SIZE + 128)

#define FID_GET_WEIGHT 1
#define FID_SET_WEIGHT_CALLBACK_PERIOD 2
#define FID_GET_WEIGHT_CALLBACK_PERIOD 3
#define FID_SET_WEIGHT_CALLBACK_THRESHOLD 4
#define FID_GET_WEIGHT_CALLBACK_THRESHOLD 5
#define FID_SET_DEBOUNCE_PERIOD 6
#define FID_GET_DEBOUNCE_PERIOD 7
#define FID_SET_MOVING_AVERAGE 8
#define FID_GET_MOVING_AVERAGE 9
#define FID_LED_ON 10
#define FID_LED_OFF 11
#define FID_IS_LED_ON 12
#define FID_CALIBRATE 13
#define FID_TARE 14
#define FID_SET_CONFIGURATION 15
#define FID_GET_CONFIGURATION 16
#define FID_WEIGHT 17
#define FID_WEIGHT_REACHED 18

typedef struct {
	MessageHeader header;
	uint8_t length;
} __attribute__((__packed__)) SetMovingAverage;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetMovingAverage;

typedef struct {
	MessageHeader header;
	uint8_t length;
} __attribute__((__packed__)) GetMovingAverageReturn;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) LEDOn;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) LEDOff;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) IsLEDOn;

typedef struct {
	MessageHeader header;
	bool value;
} __attribute__((__packed__)) IsLEDOnReturn;

typedef struct {
	MessageHeader header;
	uint32_t weight;
} __attribute__((__packed__)) Calibrate;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) Tare;

typedef struct {
	MessageHeader header;
	uint8_t rate;
	uint8_t gain;
} __attribute__((__packed__)) SetConfiguration;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetConfiguration;

typedef struct {
	MessageHeader header;
	uint8_t rate;
	uint8_t gain;
} __attribute__((__packed__)) GetConfigurationReturn;

void set_moving_average(const ComType com, const SetMovingAverage *data);
void get_moving_average(const ComType com, const GetMovingAverage *data);
void calibrate(const ComType com, const Calibrate *data);
void tare(const ComType com, const Tare *data);
void set_configuration(const ComType com, const SetConfiguration *data);
void get_configuration(const ComType com, const GetConfiguration *data);
void led_on(const ComType com, const LEDOn *data);
void led_off(const ComType com, const LEDOff *data);
void is_led_on(const ComType com, const IsLEDOn *data);

void update_rate(void);
void reinitialize_moving_average(void);
void new_value(const int32_t value);
int32_t get_weight(const int32_t value);
void write_calibration_to_eeprom(void);
void read_calibration_from_eeprom(void);
void write_configuration_to_eeprom(void);
void read_configuration_from_eeprom(void);

void invocation(const ComType com, const uint8_t *data);
void constructor(void);
void destructor(void);
void tick(const uint8_t tick_type);

#endif
