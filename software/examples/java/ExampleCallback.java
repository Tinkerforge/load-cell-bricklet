import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletLoadCell;

public class ExampleCallback {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Load Cell Bricklet
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletLoadCell lc = new BrickletLoadCell(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Add weight listener (parameter has unit g)
		lc.addWeightListener(new BrickletLoadCell.WeightListener() {
			public void weight(int weight) {
				System.out.println("Weight: " + weight + " g");
			}
		});

		// Set period for weight callback to 1s (1000ms)
		// Note: The weight callback is only called every second
		//       if the weight has changed since the last call!
		lc.setWeightCallbackPeriod(1000);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
