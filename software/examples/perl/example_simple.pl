#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletLoadCell;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change to your UID

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $lc = Tinkerforge::BrickletLoadCell->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get current weight (unit is g)
my $weight = $lc->get_weight();
print "Weight: " . $weight/1000.0 . " kg\n";

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
