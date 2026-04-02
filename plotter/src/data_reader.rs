use crate::sim_data::*;

use std::error::Error;
use std::fs::File;
use std::io::BufReader;
use std::path::Path;


pub fn read_simulation_data<P: AsRef<Path> + std::fmt::Display>(path: P) -> Result<SimulationData, Box<dyn Error>>{
    println!("{}",path);
    let file = File::open(path)?;
    let reader = BufReader::new(file);

    let data = serde_json::from_reader(reader)?;
    Ok(data)
}