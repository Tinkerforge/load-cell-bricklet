function octave_example_simple()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change to your UID

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    lc = java_new("com.tinkerforge.BrickletLoadCell", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current weight (unit is g)
    weight = lc.getWeight();
    fprintf("Weight: %g kg\n", weight/1000.0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end
