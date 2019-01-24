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

	lc.RegisterWeightCallback(func(weight int32) {
		fmt.Printf("Weight: %d g\n", weight)
	})

	// Set period for weight receiver to 1s (1000ms).
	// Note: The weight callback is only called every second
	//       if the weight has changed since the last call!
	lc.SetWeightCallbackPeriod(1000)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()
}
