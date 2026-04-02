use serde::Deserialize;

#[derive(Deserialize, Debug)]
pub struct SimulationData {
    pub config: Config,
    pub metrics: Metrics,
}

#[derive(Deserialize, Debug)]
pub struct Config {
    pub matrix_col: u16,
    pub matrix_row: u16,
    pub prob_from: u16,
    pub prob_to: u16,
    pub simulation_number: u32,
    pub steps: u16,
}

#[derive(Deserialize, Debug)]
pub struct Metrics {
    pub activation: Vec<f32>,
    pub spanning: Vec<f32>
}