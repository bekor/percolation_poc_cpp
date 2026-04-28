
#include <iostream>

#include "h5parser.h"
#include "simulation.h"
#include "ising_simulation.h"
#include "metric.hpp"
#include "metric_writer.h"

const std::string PATH = "/Users/bekor/work/uni_vien/Lab_Stat_Mech/POC/sandbox/build/resources/";

void uncorrelated_simulation(States states){
    std::string metrics_file = PATH+"metrics_30x30_100_p.h5";
    size_t simulation_number = 1000;

    int count_subset_states = 0;
    for(size_t i = 0; i < states.count; ++i) {
        if(states.percents[i] > 58.5 && states.percents[i] < 60.5) {
            count_subset_states++;
        }
    }
    std::cout << "subset size: " << count_subset_states << std::endl;


    // MetricWriter writer(metrics_file, states.rows, states.cols, count_subset_states, simulation_number);
    // Simulation usim;
    
    // for(size_t i = 0; i < states.count; ++i) {
    //     if(states.percents[i] > 58.5) {
    //         std::cout << "index: " << i << " percent: " << states.percents[i] << std::endl;
    //         auto current_metric = usim.run_simulation(states.matrices[i], states.rows, states.cols, states.activations[i], simulation_number);
    //         current_metric.probability = states.percents[i];
    //         writer.write(current_metric);
    //     }
    // }
}

void ising_simulation(States states){
    std::string metrics_file = PATH+"ising_metrics_30x30_100000_08.h5";
    size_t simulation_number = 100000;
    std::cout << "lengths (m, perc, act): " << states.matrices.size() << ", "<< states.percents.size() << ", "<< states.activations.size()
        << " row,col: " << states.rows << ", " << states.cols << " count " << states.count << "\n";
    IsingMetricWriter writer(metrics_file, states.rows, states.cols, states.count, simulation_number);
    IsingSimulation isim{states.rows, states.cols};
    
    for(size_t i = 0; i < states.count; ++i) {
        std::cout << "index: " << i << " percent: " << states.percents[i] << std::endl;
        auto current_metric = isim.run_simulation(states.matrices[i], states.activations[i], simulation_number);
        current_metric.probability = states.percents[i];
        std::cout << "writing h5 at for " << i << " percent: " << states.percents[i] << std::endl;
        writer.write(current_metric);
    }
}

int main() {
    std::string filename = PATH+"percolation_metrics_30x30_100.h5";

    auto states = load_states(filename);
    std::cout << "states: " << states.count
            << " rows: "  << states.rows
            << " cols: "  << states.cols << "\n";

    States subset;
    int count_subset_states = 0;
    subset.rows = states.rows;
    subset.cols = states.cols;
    for(size_t i = 0; i < states.count; ++i) {
        if(states.percents[i] > 59.0 && states.percents[i] < 60.5) {
            std::cout << states.percents[i] << " , act: " << states.activations[i] << std::endl;
            count_subset_states++;
            subset.matrices.push_back(states.matrices[i]);
            subset.percents.push_back(states.percents[i]);
            subset.activations.push_back(states.activations[i]);
        }
        if ( states.percents[i] >= 60.5)
            break;
    }
    subset.count = count_subset_states;

    std::cout << "subset size: " << count_subset_states << std::endl;
    // uncorrelated_simulation(states);
    ising_simulation(subset);
    return 0;
}
    