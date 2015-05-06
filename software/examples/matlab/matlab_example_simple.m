function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletLoadCell;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change to your UID

    ipcon = IPConnection(); % Create IP connection
    lc = BrickletLoadCell(UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current weight (unit is g)
    weight = lc.getWeight();
    fprintf('Weight: %g kg\n', weight/1000.0);

    input('Press any key to exit...\n', 's');
    ipcon.disconnect();
end
