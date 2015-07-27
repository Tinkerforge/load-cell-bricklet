import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletLoadCell;

public class ExampleThreshold {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;
	private static final String UID = "XYZ"; // Change to your UID

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions you
	//       might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletLoadCell lc = new BrickletLoadCell(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get threshold callbacks with a debounce time of 1 second (1000ms)
		lc.setDebouncePeriod(1000);

		// Configure threshold for "greater than 2 kg" (unit is g)
		lc.setWeightCallbackThreshold('>', 2*1000, 0);

		// Add threshold reached listener for weight greater than 2 kg (parameter has unit g)
		lc.addWeightReachedListener(new BrickletLoadCell.WeightReachedListener() {
			public void weightReached(int weight) {
				System.out.println("Weight: " + weight/1000.0 + " kg");
			}
		});

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
