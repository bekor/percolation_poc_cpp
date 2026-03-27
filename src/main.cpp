
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
    const unsigned int simulation_num = 10000;

    const ProbabilityRange prob_range{15, 85, 199};

    const u16 scaled_prob_from = prob_range.scaled_prob_from();
    u16 scaled_prob_to = prob_range.scaled_prob_to();
    u16 prob_step = prob_range.prob_step();
    

    std::cout << "from : " << scaled_prob_from 
              << ", to: " << scaled_prob_to 
              << " step: " << prob_step << "\n";
    
    write_paremeters(simulation_num, scaled_prob_from, scaled_prob_to, prob_step, OUTPUT_FILE);

    Configuration config{simulation_num, prob_range};
    Simulation simulation{config};
    std::cout << " START SUMULATION " << std::endl;
    simulation.run_simulation();
    std::cout << " END SUMULATION " << std::endl;
    Metrics metrics = simulation.get_metrics();
    generate_metrics(metrics);
}