#!/bin/sh
# connects to localhost:4223 by default, use --host and --port to change it

# change to your UID
uid=XYZ

# get threshold callbacks with a debounce time of 10 seconds (10000ms)
tinkerforge call load-cell-bricklet $uid set-debounce-period 10000

# configure threshold for "greater than 200 g" (unit is g)
tinkerforge call load-cell-bricklet $uid set-weight-callback-threshold greater 200 0

# handle incoming weight-reached callbacks (unit is g)
tinkerforge dispatch load-cell-bricklet $uid weight-reached\
 --execute "echo Weight: {weight} g."
