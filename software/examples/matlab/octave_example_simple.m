function octave_example_simple()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Load Cell Bricklet

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    lc = javaObject("com.tinkerforge.BrickletLoadCell", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current weight
    weight = lc.getWeight();
    fprintf("Weight: %d g\n", weight);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end
