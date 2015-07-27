#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_load_cell.h"

#define HOST "localhost"
#define PORT 4223
#define UID "XYZ" // Change to your UID

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

	// Get current weight (unit is g)
	int32_t weight;
	if(load_cell_get_weight(&lc, &weight) < 0) {
		fprintf(stderr, "Could not get weight, probably timeout\n");
		exit(1);
	}

	printf("Weight: %f kg\n", weight/1000.0);

	printf("Press key to exit\n");
	getchar();
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
}
