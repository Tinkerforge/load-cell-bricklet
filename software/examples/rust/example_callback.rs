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

    let weight_receiver = lc.get_weight_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `lc` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for weight in weight_receiver {
            println!("Weight: {} g", weight);
        }
    });

    // Set period for weight receiver to 1s (1000ms).
    // Note: The weight callback is only called every second
    //       if the weight has changed since the last call!
    lc.set_weight_callback_period(1000);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
