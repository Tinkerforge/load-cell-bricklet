function octave_example_threshold()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Load Cell Bricklet

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    lc = java_new("com.tinkerforge.BrickletLoadCell", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get threshold callbacks with a debounce time of 1 second (1000ms)
    lc.setDebouncePeriod(1000);

    % Register weight reached callback to function cb_weight_reached
    lc.addWeightReachedCallback(@cb_weight_reached);

    % Configure threshold for weight "greater than 200 g" (unit is g)
    lc.setWeightCallbackThreshold(">", 200, 0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for weight reached callback (parameter has unit g)
function cb_weight_reached(e)
    fprintf("Weight: %d g\n", e.weight);
end
