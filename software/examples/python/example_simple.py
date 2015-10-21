#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change to your UID

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_load_cell import BrickletLoadCell

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    lc = BrickletLoadCell(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Get current weight (unit is g)
    weight = lc.get_weight()
    print("Weight: " + str(weight/1000.0) + " kg")

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
