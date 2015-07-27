using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change to your UID

	// Callback function for weight greater than 2 kg (parameter has unit g)
	static void WeightReachedCB(BrickletLoadCell sender, int weight)
	{
		System.Console.WriteLine("Weight: " + weight/1000.0 + " kg");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletLoadCell lc = new BrickletLoadCell(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get threshold callbacks with a debounce time of 1 second (1000ms)
		lc.SetDebouncePeriod(1000);

		// Register threshold reached callback to function WeightReachedCB
		lc.WeightReached += WeightReachedCB;

		// Configure threshold for "greater than 2 kg" (unit is g)
		lc.SetWeightCallbackThreshold('>', 2*1000, 0);

		System.Console.WriteLine("Press enter to exit");
		System.Console.ReadLine();
		ipcon.Disconnect();
	}
}
