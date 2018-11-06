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

    //Create listener for weight events.
    let weight_listener = load_cell_bricklet.get_weight_receiver();
    // Spawn thread to handle received events. This thread ends when the load_cell_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in weight_listener {
            println!("Weight: {}{}", event, " g");
        }
    });

    // Set period for weight listener to 1s (1000ms)
    // Note: The weight callback is only called every second
    //       if the weight has changed since the last call!
    load_cell_bricklet.set_weight_callback_period(1000);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
