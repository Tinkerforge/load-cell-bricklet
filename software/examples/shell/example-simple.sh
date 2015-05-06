#!/bin/sh
# connects to localhost:4223 by default, use --host and --port to change it

# change to your UID
uid=XYZ

# get current weight (unit is g)
tinkerforge call load-cell-bricklet $uid get-weight
