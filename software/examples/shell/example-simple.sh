#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change to your UID

# Get current weight (unit is g)
tinkerforge call load-cell-bricklet $uid get-weight
