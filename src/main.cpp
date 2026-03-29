
#include <array>
#include <iostream>
#include <string_view>

#include "printer.h"
#include "file_io.h"
#include "config.h"
#include "simulation.h"

typedef uint16_t u16;
typedef uint32_t u32;
typedef std::array<u32, 200> metrics_array;
const std::string_view OUTPUT_FILE = "percolation_metrics.txt";

void generate_metrics(const Metrics& metrics){
    const metrics_array activation = metrics.activation_per_run;
    const metrics_array spanning_at_prob = metrics.spanning_at_prob;
    const unsigned int simulation_num = metrics.simulation_num;

    std::cout << "sum numb: " << simulation_num << std::endl;

    std::array<double, 200> avg_activation_per_run{};
    for(u32 i = 0; i < activation.size(); ++i){
        double norm_activation = static_cast<double>(activation.at(i)) / simulation_num;
        std::cout << "norm: " << norm_activation << "\n";
        avg_activation_per_run.at(i) = norm_activation;
    }

    print_metrics("Avg activation at probability: ", avg_activation_per_run);

    std::array<double, 200> avg_norm_spanning{};
    for(u32 i = 0; i < spanning_at_prob.size(); ++i){
        avg_norm_spanning.at(i) = static_cast<double>(spanning_at_prob.at(i)) / simulation_num;
    }
    print_metrics("Avg spanning at probability: ", avg_norm_spanning);

    write_results(avg_activation_per_run, avg_norm_spanning, OUTPUT_FILE);
}


int main() {
    const u32 simulation_num = 100000;
    const u16 probability_from = 10;
    const u16 probability_to = 90;
    const u16 step_number = 200;
    const ProbabilityRange prob_range{probability_from, probability_to, step_number};

    Configuration config{simulation_num, prob_range};
    print_config(config);

    write_paremeters(simulation_num, prob_range, OUTPUT_FILE);

    Simulation simulation{config};
    std::cout << " START SUMULATION " << std::endl;
    simulation.run_simulation();
    std::cout << " END SUMULATION " << std::endl;
    Metrics metrics = simulation.get_metrics();
    generate_metrics(metrics);
}