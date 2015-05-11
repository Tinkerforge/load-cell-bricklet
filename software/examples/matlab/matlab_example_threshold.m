function matlab_example_threshold()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletLoadCell;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'amb'; % Change to your UID

    ipcon = IPConnection(); % Create IP connection
    lc = BrickletLoadCell(UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Set threshold callbacks with a debounce time of 10 seconds (10000ms)
    lc.setDebouncePeriod(10000);

    % Register threshold reached callback to function cb_reached
    set(lc, 'WeightReachedCallback', @(h, e) cb_reached(e));

    % Configure threshold for "greater than 200 g" (unit is g)
    lc.setWeightCallbackThreshold('>', 200, 0);

    input('Press any key to exit...\n', 's');
    ipcon.disconnect();
end

% Callback for weight greater than 200 g
function cb_reached(e)
    fprintf('Weight: %g kg.\n', e.weight/1000.0);
end
