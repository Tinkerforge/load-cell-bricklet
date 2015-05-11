var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change to your UID

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var lc = new Tinkerforge.BrickletLoadCell(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function(error) {
        console.log('Error: '+error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function(connectReason) {
        // Get threshold callbacks with a debounce time of 10 seconds (10000ms)
        lc.setDebouncePeriod(10000);
        // Configure threshold for "greater than 200 g"
        lc.setWeightCallbackThreshold('>', 200, 0);
    }
);

// Register threshold reached callback to function cb_reached
lc.on(Tinkerforge.BrickletLoadCell.CALLBACK_WEIGHT_REACHED,
    // Callback for weight greater than 200 g
    function(weight) {
        console.log('Weight: ' +weight/1000 + ' kg.');
    }
);

console.log("Press any key to exit ...");
process.stdin.on('data',
    function(data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
