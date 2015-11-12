function matlab_example_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletLoadCell;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change to your UID

    ipcon = IPConnection(); % Create IP connection
    lc = handle(BrickletLoadCell(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register weight callback to function cb_weight
    set(lc, 'WeightCallback', @(h, e) cb_weight(e));

    % Set period for weight callback to 1s (1000ms)
    % Note: The weight callback is only called every second
    %       if the weight has changed since the last call!
    lc.setWeightCallbackPeriod(1000);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for weight callback (parameter has unit g)
function cb_weight(e)
    fprintf('Weight: %i g\n', e.weight);
end
