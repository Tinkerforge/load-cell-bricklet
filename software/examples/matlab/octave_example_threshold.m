function octave_example_threshold()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "amb"; % Change to your UID

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    lc = java_new("com.tinkerforge.BrickletLoadCell", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Set threshold callbacks with a debounce time of 10 seconds (10000ms)
    lc.setDebouncePeriod(10000);

    % Configure threshold for "greater than 200 g"
    lc.setWeightCallbackThreshold(lc.THRESHOLD_OPTION_GREATER, 200, 0);

    % Register threshold reached callback to function cb_reached
    lc.addWeightReachedCallback(@cb_reached);

    input("Press any key to exit...\n", "s");
    ipcon.disconnect();
end

% Callback function for weight callback (parameter has unit g)
function cb_reached(e)
    fprintf("Weight: %g kg\n", e.weight/1000.0);
end
