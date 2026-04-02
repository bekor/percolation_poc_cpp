use plotters::prelude::*;
use crate::sim_data::*;

pub fn plot(sim: &SimulationData) -> Result<(), Box<dyn std::error::Error>> {
    plot_activation(sim)?;
    plot_spanning(sim)?;
    println!("Saved activation_plot.png and spanning_plot.png");
    Ok(())
}

fn scale(value: u16) -> f32 {
    let max_u16   = u16::MAX as f32;
    (value as f32 / max_u16) * 100.0
}

fn x_axis(sim: &SimulationData) -> Vec<f32> {
    let n = sim.metrics.activation.len();
    let from = scale(sim.config.prob_from);
    let to = scale(sim.config.prob_to);
    let c = from + (to - from) * 5 as f32 / (n - 1) as f32;
    println!("range: {n}, from: {from}, to: {to}, calc: {c}");
    (0..n)
        .map(|i| from + (to - from) * i as f32 / (n - 1) as f32)
        .collect()
}

fn plot_activation(sim: &SimulationData) -> Result<(), Box<dyn std::error::Error>> {
    let x = x_axis(sim);
    let y = &sim.metrics.activation;

    let root = BitMapBackend::new("activation_plot.png", (900, 500)).into_drawing_area();
    root.fill(&WHITE)?;

    let x_min = scale(sim.config.prob_from);
    let x_max = scale(sim.config.prob_to);
    let y_max = y.iter().cloned().fold(f32::NEG_INFINITY, f32::max) * 1.05;

    let mut chart = ChartBuilder::on(&root)
        .caption(
            format!("Mean Activation  (sim_count={})", sim.config.simulation_number),
            ("sans-serif", 20),
        )
        .margin(30)
        .x_label_area_size(40)
        .y_label_area_size(50)
        .build_cartesian_2d(x_min..x_max, 0f32..y_max)?;

    chart
        .configure_mesh()
        .x_desc("Occupation probability (%)")
        .y_desc("Mean active cells")
        .draw()?;

    chart.draw_series(LineSeries::new(
        x.iter().zip(y.iter()).map(|(&xi, &yi)| (xi, yi)),
        &BLUE,
    ))?
    .label("activation")
    .legend(|(x, y)| PathElement::new(vec![(x, y), (x + 20, y)], &BLUE));

    // draw horizontal reference lines at 0 and 9
    chart.draw_series(std::iter::once(PathElement::new(
        vec![(x_min, 0.0), (x_max, 0.0)], &BLACK.mix(0.3),
    )))?;
    chart.draw_series(std::iter::once(PathElement::new(
        vec![(x_min, 9.0), (x_max, 9.0)], &RED.mix(0.4),
    )))?;

    chart.configure_series_labels()
        .border_style(&BLACK)
        .background_style(&WHITE.mix(0.8))
        .draw()?;

    root.present()?;
    Ok(())
}

fn plot_spanning(sim: &SimulationData) -> Result<(), Box<dyn std::error::Error>> {
    let x = x_axis(sim);
    let y = &sim.metrics.spanning;

    let root = BitMapBackend::new("spanning_plot.png", (900, 500)).into_drawing_area();
    root.fill(&WHITE)?;

    let x_min = scale(sim.config.prob_from);
    let x_max = scale(sim.config.prob_to);

    let mut chart = ChartBuilder::on(&root)
        .caption(
            format!("Spanning probability  (sim_count={})", sim.config.simulation_number),
            ("sans-serif", 20),
        )
        .margin(30)
        .x_label_area_size(40)
        .y_label_area_size(50)
        .build_cartesian_2d(x_min..x_max, 0f32..1.05)?;

    chart
        .configure_mesh()
        .x_desc("Occupation probability (%)")
        .y_desc("P(spanning)")
        .draw()?;

    chart.draw_series(LineSeries::new(
        x.iter().zip(y.iter()).map(|(&xi, &yi)| (xi, yi)),
        &RED,
    ))?
    .label("P(spanning)")
    .legend(|(x, y)| PathElement::new(vec![(x, y), (x + 20, y)], &RED));

    // draw 0.5 threshold line
    chart.draw_series(std::iter::once(PathElement::new(
        vec![(x_min, 0.5), (x_max, 0.5)],
        ShapeStyle::from(&BLACK.mix(0.4)).stroke_width(1),
    )))?
    .label("p = 0.5")
    .legend(|(x, y)| PathElement::new(vec![(x, y), (x + 20, y)], &BLACK));

    chart.configure_series_labels()
        .border_style(&BLACK)
        .background_style(&WHITE.mix(0.8))
        .draw()?;

    root.present()?;
    Ok(())
}