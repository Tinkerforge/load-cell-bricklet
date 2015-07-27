#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change to your UID

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_load_cell import LoadCell

# Callback function for weight greater than 2 kg (parameter has unit g)
def cb_weight_reached(weight):
    print('Weight: ' + str(weight/1000.0) + ' kg')

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    lc = LoadCell(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Get threshold callbacks with a debounce time of 1 second (1000ms)
    lc.set_debounce_period(1000)

    # Register threshold reached callback to function cb_weight_reached
    lc.register_callback(lc.CALLBACK_WEIGHT_REACHED, cb_weight_reached)

    # Configure threshold for "greater than 2 kg" (unit is g)
    lc.set_weight_callback_threshold('>', 2*1000, 0)

    raw_input('Press key to exit\n') # Use input() in Python 3
    ipcon.disconnect()
