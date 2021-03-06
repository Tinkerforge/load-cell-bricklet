#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_load_cell'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Load Cell Bricklet

ipcon = IPConnection.new # Create IP connection
lc = BrickletLoadCell.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Register weight callback
lc.register_callback(BrickletLoadCell::CALLBACK_WEIGHT) do |weight|
  puts "Weight: #{weight} g"
end

# Set period for weight callback to 1s (1000ms)
# Note: The weight callback is only called every second
#       if the weight has changed since the last call!
lc.set_weight_callback_period 1000

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
