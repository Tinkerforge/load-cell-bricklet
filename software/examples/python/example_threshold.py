#!/usr/bin/env python
# -*- coding: utf-8 -*-  

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change to your UID

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_load_cell import LoadCell

# Turn LED on if weight is greater than 200 grams
def cb_reached(lc, weight):
    lc.led_on()

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    lc = LoadCell(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Get threshold callbacks with a debounce time of 1 second (1000ms)
    lc.set_debounce_period(1000)

    # Register threshold reached callback to function cb_reached
    func = lambda x: cb_reached(lc, x)
    lc.register_callback(lc.CALLBACK_WEIGHT_REACHED, func)

    # Configure threshold for "greater than 200 grams"
    lc.set_weight_callback_threshold('>', 200, 0)

    raw_input('Press key to exit\n') # Use input() in Python 3
    ipcon.disconnect()
