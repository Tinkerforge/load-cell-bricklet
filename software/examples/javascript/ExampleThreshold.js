var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change to your UID

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var lc = new Tinkerforge.BrickletLoadCell(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Get threshold callbacks with a debounce time of 1 second (1000ms)
        lc.setDebouncePeriod(1000);

        // Configure threshold for weight "greater than 200 g" (unit is g)
        lc.setWeightCallbackThreshold('>', 200, 0);
    }
);

// Register weight reached callback
lc.on(Tinkerforge.BrickletLoadCell.CALLBACK_WEIGHT_REACHED,
    // Callback function for weight reached callback (parameter has unit g)
    function (weight) {
        console.log('Weight: ' + weight/1000.0 + ' kg');
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
