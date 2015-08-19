#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletLoadCell;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change to your UID

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $lc = Tinkerforge::BrickletLoadCell->new(&UID, $ipcon); # Create device object

# Callback subroutine for weight callback (parameter has unit g)
sub cb_weight
{
    my ($weight) = @_;

    print "Weight: " . $weight/1000.0 . " kg\n";
}

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Set period for weight callback to 1s (1000ms)
# Note: The weight callback is only called every second
#       if the weight has changed since the last call!
$lc->set_weight_callback_period(1000);

# Register weight callback to subroutine cb_weight
$lc->register_callback($lc->CALLBACK_WEIGHT, 'cb_weight');

print "Press any key to exit...\n";
<STDIN>;
$ipcon->disconnect();
