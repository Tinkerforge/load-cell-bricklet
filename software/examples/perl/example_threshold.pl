#!/usr/bin/perl  

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletLoadCell;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change to your UID

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $lc = Tinkerforge::BrickletLoadCell->new(&UID, $ipcon); # Create device object

# Callback for weight greater than 200 g
sub cb_reached
{
    my ($weight) = @_;

    print "Weight ".$weight/1000.0." kg\n";
}

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
$lc->set_debounce_period(10000);

# Register threshold reached callback to function cb_reached
$lc->register_callback($lc->CALLBACK_WEIGHT_REACHED, 'cb_reached');

# Configure threshold for "greater than 200 g"
$lc->set_weight_callback_threshold('>', 200, 0);

print "Press any key to exit...\n";
<STDIN>;
$ipcon->disconnect();
