using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change to your UID

	// Callback for weight greater than 200 g
	static void ReachedCB(BrickletLoadCell sender, int weight)
	{
		System.Console.WriteLine("Weight: " + weight/1000.0 + " kg");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletLoadCell dd = new BrickletLoadCell(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
		dd.SetDebouncePeriod(10000);

		// Register threshold reached callback to function ReachedCB
		dd.WeightReached += ReachedCB;

		// Configure threshold for "greater than 200 g"
		dd.SetWeightCallbackThreshold('>', 200, 0);

		System.Console.WriteLine("Press enter to exit");
		System.Console.ReadLine();
		ipcon.Disconnect();
	}
}
