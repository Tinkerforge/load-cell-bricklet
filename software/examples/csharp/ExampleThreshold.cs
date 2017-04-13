using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Load Cell Bricklet

	// Callback function for weight reached callback (parameter has unit g)
	static void WeightReachedCB(BrickletLoadCell sender, int weight)
	{
		Console.WriteLine("Weight: " + weight + " g");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletLoadCell lc = new BrickletLoadCell(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get threshold callbacks with a debounce time of 1 second (1000ms)
		lc.SetDebouncePeriod(1000);

		// Register weight reached callback to function WeightReachedCB
		lc.WeightReachedCallback += WeightReachedCB;

		// Configure threshold for weight "greater than 200 g" (unit is g)
		lc.SetWeightCallbackThreshold('>', 200, 0);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
