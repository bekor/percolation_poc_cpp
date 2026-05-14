
#include <iostream>

#include "config.hpp"
#include "h5parser.h"
#include "simulation.h"
#include "ising_simulation.h"
#include "metric.hpp"
#include "metric_writer.h"

[[nodiscard]]
States make_subset(const States& states, float low, float high, bool is_single) {
    States out;
    out.rows = states.rows;
    out.cols = states.cols;
 
    for (size_t i = 0; i < states.count; ++i) {
        float p = states.percents[i];
        if (p > low && p < high) {
            out.matrices.push_back(states.matrices[i]);
            out.percents.push_back(p);
            out.activations.push_back(states.activations[i]);
            if(is_single)
                break;
        }
    }
    out.count = out.matrices.size();
    return out;
}

void uncorrelated_simulation(const States& states, const UncorrelatedConfig& cfg){
    std::string metrics_file = std::string(RESOURCES_DIR) + cfg.output_file;
    size_t simulation_number = 1000;

    MetricWriter writer(metrics_file, states.rows, states.cols, states.count, cfg.sim_count);
    Simulation usim;
    
    for(size_t i = 0; i < states.count; ++i) {
        std::cout << "index: " << i << " percent: " << states.percents[i] << std::endl;
        auto current_metric = usim.run_simulation(states.matrices[i], states.rows, states.cols, 
                                                    states.activations[i], cfg.sim_count);
        current_metric.probability = states.percents[i];
        writer.write(current_metric);
    }
}

void ising_simulation(const States& states, float beta, const IsingConfig& cfg){
    std::ostringstream ss;
    ss << "/ising_metrics_" << states.rows << "_" << cfg.sim_count << "_" << beta << (cfg.moving_betas ? "_moving" : "") << ".h5";
    std::string file_name = ss.str();
    std::string metrics_file = std::string(RESOURCES_DIR) + file_name;

    std::cout << "lengths (m, perc, act): " << states.matrices.size() << ", "
        << states.percents.size() << ", "<< states.activations.size()
        << " row,col: " << states.rows << ", " << states.cols << " count " << states.count 
        << " beta: " << beta << "moving beta: " << cfg.moving_betas <<"\n";
    
    IsingMetricWriter writer{metrics_file, states.rows, states.cols, 
                            states.count, cfg.sim_count, cfg.save_evolution};
    IsingSimulation isim{states.rows, states.cols};
    
    for(size_t i = 0; i < states.count; ++i) {
        std::cout << "index: " << i << " percent: " << states.percents[i] << std::endl;
        auto current_metric = isim.run_simulation(states.matrices[i], states.activations[i], 
                                                 cfg.sim_count, beta, cfg.moving_betas);
        current_metric.probability = states.percents[i];
        std::cout << "writing h5 at for " << i << " percent: " << states.percents[i] << std::endl;
        writer.write(current_metric);
    }
}

int main() {
    PercolationConfig   pcfg{(std::string(RESOURCES_DIR) + "/percolation_metrics_30x30_100.h5"), 59.0f, 59.5f};
    UncorrelatedConfig  ucfg{(std::string(RESOURCES_DIR) + "/metrics_30x30_100_p.h5"), 1'000};
    IsingConfig         icfg{{ 0.1f }, 1'000'000, false, true};
    // std::string filename = std::string(RESOURCES_DIR) + "/percolation_metrics_30x30_100.h5";
    // std::string filename = RESOURCES_DIR/"percolation_metrics_60x60_50.h5";

    auto states = load_states(pcfg.input_file);
    std::cout << "states: " << states.count
            << " rows: "  << states.rows
            << " cols: "  << states.cols << "\n";

    auto subset = make_subset(states, pcfg.percent_low, pcfg.percent_high, false);

    std::cout << "subset size: " << subset.count << std::endl;
    // uncorrelated_simulation(states, ucfg);
    for(auto beta: icfg.betas){
        ising_simulation(subset, beta, icfg);
    }
    return 0;
}
    