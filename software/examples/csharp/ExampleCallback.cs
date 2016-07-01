using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Load Cell Bricklet

	// Callback function for weight callback (parameter has unit g)
	static void WeightCB(BrickletLoadCell sender, int weight)
	{
		Console.WriteLine("Weight: " + weight + " g");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletLoadCell lc = new BrickletLoadCell(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register weight callback to function WeightCB
		lc.Weight += WeightCB;

		// Set period for weight callback to 1s (1000ms)
		// Note: The weight callback is only called every second
		//       if the weight has changed since the last call!
		lc.SetWeightCallbackPeriod(1000);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
