use std::{error::Error, io, thread};
use tinkerforge::{ip_connection::IpConnection, load_cell_bricklet::*};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Load Cell Bricklet.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let lc = LoadCellBricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    // Get threshold receivers with a debounce time of 1 second (1000ms).
    lc.set_debounce_period(1000);

    let weight_reached_receiver = lc.get_weight_reached_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `lc` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for weight_reached in weight_reached_receiver {
            println!("Weight: {} g", weight_reached);
        }
    });

    // Configure threshold for weight "greater than 200 g".
    lc.set_weight_callback_threshold('>', 200, 0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
