#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Load Cell Bricklet

# Get threshold callbacks with a debounce time of 1 second (1000ms)
tinkerforge call load-cell-bricklet $uid set-debounce-period 1000

# Handle incoming weight reached callbacks (parameter has unit g)
tinkerforge dispatch load-cell-bricklet $uid weight-reached &

# Configure threshold for weight "greater than 200 g" (unit is g)
tinkerforge call load-cell-bricklet $uid set-weight-callback-threshold threshold-option-greater 200 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
