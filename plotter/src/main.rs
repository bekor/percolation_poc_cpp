mod plotter;
use plotter::plot;
mod data_reader;
use data_reader::*;
mod sim_data;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let data = read_simulation_data("percolation_metrics.json")?;

    plot(&data)?;
    Ok(())
}