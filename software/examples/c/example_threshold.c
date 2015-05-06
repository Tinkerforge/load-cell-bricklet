#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_load_cell.h"

#define HOST "localhost"
#define PORT 4223
#define UID "XYZ" // Change to your UID

// Callback for weight greater than 200 g
void cb_reached(uint16_t weight, void *user_data) {
	(void)user_data; // avoid unused parameter warning

	printf("Weight %f kg\n", weight/1000.0);
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

	// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
	load_cell_set_debounce_period(&lc, 10000);

	// Register threshold reached callback to function cb_reached
	load_cell_register_callback(&lc,
	                            LOAD_CELL_CALLBACK_WEIGHT_REACHED,
	                            (void *)cb_reached,
	                            NULL);

	// Configure threshold for "greater than 200 g"
	load_cell_set_weight_callback_threshold(&lc, '>', 200, 0);

	printf("Press key to exit\n");
	getchar();
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
}
