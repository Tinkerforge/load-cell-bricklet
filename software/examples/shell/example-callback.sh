#!/bin/sh
# connects to localhost:4223 by default, use --host and --port to change it

# change to your UID
uid=XYZ

# set period for weight callback to 1s (1000ms)
# note: the weight callback is only called every second if the
#       weight has changed since the last call!
tinkerforge call load-cell-bricklet $uid set-weight-callback-period 1000

# handle incoming weight callbacks (unit is g)
tinkerforge dispatch load-cell-bricklet $uid weight
