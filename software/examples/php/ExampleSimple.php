<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletLoadCell.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletLoadCell;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Load Cell Bricklet

$ipcon = new IPConnection(); // Create IP connection
$lc = new BrickletLoadCell(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current weight (unit is g)
$weight = $lc->getWeight();
echo "Weight: $weight g\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
