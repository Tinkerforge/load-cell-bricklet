package main

import (
	"fmt"
	"github.com/Tinkerforge/go-api-bindings/ipconnection"
	"github.com/Tinkerforge/go-api-bindings/load_cell_bricklet"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Load Cell Bricklet.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	lc, _ := load_cell_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Get threshold receivers with a debounce time of 1 second (1000ms).
	lc.SetDebouncePeriod(1000)

	lc.RegisterWeightReachedCallback(func(weight int32) {
		fmt.Printf("Weight: %d g\n", weight)
	})

	// Configure threshold for weight "greater than 200 g".
	lc.SetWeightCallbackThreshold('>', 200, 0)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
