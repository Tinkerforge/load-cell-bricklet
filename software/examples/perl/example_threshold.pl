#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletLoadCell;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Load Cell Bricklet

# Callback subroutine for weight reached callback
sub cb_weight_reached
{
    my ($weight) = @_;

    print "Weight: $weight g\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $lc = Tinkerforge::BrickletLoadCell->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get threshold callbacks with a debounce time of 1 second (1000ms)
$lc->set_debounce_period(1000);

# Register weight reached callback to subroutine cb_weight_reached
$lc->register_callback($lc->CALLBACK_WEIGHT_REACHED, 'cb_weight_reached');

# Configure threshold for weight "greater than 200 g"
$lc->set_weight_callback_threshold('>', 200, 0);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
