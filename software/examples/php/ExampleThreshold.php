<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletLoadCell.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletLoadCell;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change to your UID

// Callback for weight greater than 200 g
function cb_reached($weight)
{
    echo "Weight " . $weight / 1000.0 . " kg.\n";
}

$ipcon = new IPConnection(); // Create IP connection
$lc = new BrickletLoadCell(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
$lc->setDebouncePeriod(10000);

// Register threshold reached callback to function cb_reached
$lc->registerCallback(BrickletLoadCell::CALLBACK_WEIGHT_REACHED, 'cb_reached');

// Configure threshold for "greater than 200 g"
$lc->setWeightCallbackThreshold('>', 200, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
