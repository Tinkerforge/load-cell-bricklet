function matlab_example_threshold()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletLoadCell;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change to your UID

    ipcon = IPConnection(); % Create IP connection
    lc = handle(BrickletLoadCell(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get threshold callbacks with a debounce time of 1 second (1000ms)
    lc.setDebouncePeriod(1000);

    % Register weight reached callback to function cb_weight_reached
    set(lc, 'WeightReachedCallback', @(h, e) cb_weight_reached(e));

    % Configure threshold for weight "greater than 200 g" (unit is g)
    lc.setWeightCallbackThreshold('>', 200, 0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for weight reached callback (parameter has unit g)
function cb_weight_reached(e)
    fprintf('Weight: %i g\n', e.weight);
end
