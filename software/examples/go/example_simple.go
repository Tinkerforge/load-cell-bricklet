package main

import (
	"fmt"
	"tinkerforge/ipconnection"
	"tinkerforge/load_cell_bricklet"
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

	// Get current weight.
	weight, _ := lc.GetWeight()
	fmt.Printf("Weight:  g\n", weight)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
