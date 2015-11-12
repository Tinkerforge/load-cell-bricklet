<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletLoadCell.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletLoadCell;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change to your UID

// Callback function for weight reached callback (parameter has unit g)
function cb_weightReached($weight)
{
    echo "Weight: $weight g\n";
}

$ipcon = new IPConnection(); // Create IP connection
$lc = new BrickletLoadCell(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get threshold callbacks with a debounce time of 1 second (1000ms)
$lc->setDebouncePeriod(1000);

// Register weight reached callback to function cb_weightReached
$lc->registerCallback(BrickletLoadCell::CALLBACK_WEIGHT_REACHED, 'cb_weightReached');

// Configure threshold for weight "greater than 200 g" (unit is g)
$lc->setWeightCallbackThreshold('>', 200, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
