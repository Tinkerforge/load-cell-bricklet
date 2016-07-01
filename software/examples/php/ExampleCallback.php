<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletLoadCell.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletLoadCell;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Load Cell Bricklet

// Callback function for weight callback (parameter has unit g)
function cb_weight($weight)
{
    echo "Weight: $weight g\n";
}

$ipcon = new IPConnection(); // Create IP connection
$lc = new BrickletLoadCell(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register weight callback to function cb_weight
$lc->registerCallback(BrickletLoadCell::CALLBACK_WEIGHT, 'cb_weight');

// Set period for weight callback to 1s (1000ms)
// Note: The weight callback is only called every second
//       if the weight has changed since the last call!
$lc->setWeightCallbackPeriod(1000);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
