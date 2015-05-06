#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_load_cell.h"

#define HOST "localhost"
#define PORT 4223
#define UID "XYZ" // Change to your UID

// Callback function for weight callback (parameter has unit g/10)
void cb_weight(uint16_t weight, void *user_data) {
	(void)user_data; // avoid unused parameter warning

	printf("Weight: %f kg\n", weight/1000.0);
}

int main() {
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	LoadCell lc;
	load_cell_create(&lc, UID, &ipcon); 

	// Connect to brickd
	if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		exit(1);
	}
	// Don't use device before ipcon is connected

	// Set Period for weight callback to 1s (1000ms)
	// Note: The weight callback is only called every second if the 
	//       weight has changed since the last call!
	load_cell_set_weight_callback_period(&lc, 1000);

	// Register weight callback to function cb_weight
	load_cell_register_callback(&lc,
	                            LOAD_CELL_CALLBACK_WEIGHT,
	                            (void *)cb_weight,
	                            NULL);

	printf("Press key to exit\n");
	getchar();
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
}
