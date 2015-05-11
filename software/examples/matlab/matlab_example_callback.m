function matlab_example_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletLoadCell;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'amb'; % Change to your UID

    ipcon = IPConnection(); % Create IP connection
    lc = BrickletLoadCell(UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Set Period for weight callback to 1s (1000ms)
    % Note: The callback is only called every second if the
    %       weight has changed since the last call!
    lc.setWeightCallbackPeriod(1000);

    % Register weight callback to function cb_weight
    set(lc, 'WeightCallback', @(h, e) cb_weight(e));

    input('Press any key to exit...\n', 's');
    ipcon.disconnect();
end

% Callback function for weight callback (parameter has unit g)
function cb_weight(e)
    fprintf('Weight: %g kg\n', e.weight/1000.0);
end
