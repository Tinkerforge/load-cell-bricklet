#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_load_cell'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change to your UID

ipcon = IPConnection.new # Create IP connection
lc = BrickletLoadCell.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Get threshold callbacks with a debounce time of 1 second (1000ms)
lc.set_debounce_period 1000

# Register threshold reached callback for weight greater than 2 kg (parameter has unit g)
lc.register_callback(BrickletLoadCell::CALLBACK_WEIGHT_REACHED) do |weight|
  puts "Weight: #{weight/1000.0} kg"
end

# Configure threshold for "greater than 2 kg" (unit is g)
lc.set_weight_callback_threshold '>', 2*1000, 0

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
