use std::{error::Error, io, thread};
use tinkerforge::{ipconnection::IpConnection, load_cell_bricklet::*};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Load Cell Bricklet

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let load_cell_bricklet = LoadCellBricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    // Get threshold listeners with a debounce time of 1 second (1000ms)
    load_cell_bricklet.set_debounce_period(1000);

    //Create listener for weight reached events.
    let weight_reached_listener = load_cell_bricklet.get_weight_reached_receiver();
    // Spawn thread to handle received events. This thread ends when the load_cell_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in weight_reached_listener {
            println!("Weight: {}{}", event, " g");
        }
    });

    // Configure threshold for weight "greater than 200 g"
    load_cell_bricklet.set_weight_callback_threshold('>', 200, 0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
