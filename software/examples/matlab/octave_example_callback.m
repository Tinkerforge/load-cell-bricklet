function octave_example_callback()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change to your UID

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    lc = java_new("com.tinkerforge.BrickletLoadCell", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register weight callback to function cb_weight
    lc.addWeightCallback(@cb_weight);

    % Set period for weight callback to 1s (1000ms)
    % Note: The weight callback is only called every second
    %       if the weight has changed since the last call!
    lc.setWeightCallbackPeriod(1000);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for weight callback (parameter has unit g)
function cb_weight(e)
    fprintf("Weight: %g kg\n", e.weight/1000.0);
end
