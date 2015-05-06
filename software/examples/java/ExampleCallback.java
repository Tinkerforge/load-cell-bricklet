import com.tinkerforge.BrickletLoadCell;
import com.tinkerforge.IPConnection;

public class ExampleCallback {
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

		// Set Period for weight callback to 1s (1000ms)
		// Note: The weight callback is only called every second if the 
		//       weight has changed since the last call!
		lc.setWeightCallbackPeriod(1000);

		// Add and implement weight listener (called if weight changes)
		lc.addWeightListener(new BrickletLoadCell.WeightListener() {
			public void weight(int weight) {
				System.out.println("Weight: " + weight/1000.0 + " kg");
			}
		});

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
