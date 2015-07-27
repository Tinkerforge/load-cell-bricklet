using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change to your UID

	// Callback function for weight callback (parameter has unit g)
	static void WeightCB(BrickletLoadCell sender, int weight)
	{
		System.Console.WriteLine("Weight: " + weight/1000.0 + " kg");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletLoadCell lc = new BrickletLoadCell(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Set period for weight callback to 1s (1000ms)
		// Note: The weight callback is only called every second
		//       if the weight has changed since the last call!
		lc.SetWeightCallbackPeriod(1000);

		// Register weight callback to function WeightCB
		lc.Weight += WeightCB;

		System.Console.WriteLine("Press enter to exit");
		System.Console.ReadLine();
		ipcon.Disconnect();
	}
}
