#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Load Cell Bricklet

# Handle incoming weight callbacks (parameter has unit g)
tinkerforge dispatch load-cell-bricklet $uid weight &

# Set period for weight callback to 1s (1000ms)
# Note: The weight callback is only called every second
#       if the weight has changed since the last call!
tinkerforge call load-cell-bricklet $uid set-weight-callback-period 1000

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
