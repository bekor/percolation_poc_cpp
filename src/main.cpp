
#include <array>
#include <iostream>
#include <string_view>
#include <filesystem>

#include "printer.h"
#include "file_io.h"
#include "config.h"
#include "simulation.h"

typedef uint16_t u16;
typedef uint32_t u32;
typedef std::array<u32, 200> metrics_array;
constexpr std::string_view CONFIG_JSON = "config.json";

void generate_metrics(const Metrics& metrics, const std::filesystem::path out_path){
    const metrics_array activation = metrics.activation_per_run;
    const metrics_array spanning_at_prob = metrics.spanning_at_prob;
    const unsigned int simulation_num = metrics.simulation_num;

    std::array<double, 200> avg_activation_per_run{};
    for(u32 i = 0; i < activation.size(); ++i){
        double norm_activation = static_cast<double>(activation.at(i)) / simulation_num;
        avg_activation_per_run.at(i) = norm_activation;
    }

    print_metrics("Avg activation at probability: ", avg_activation_per_run);

    std::array<double, 200> avg_norm_spanning{};
    for(u32 i = 0; i < spanning_at_prob.size(); ++i){
        avg_norm_spanning.at(i) = static_cast<double>(spanning_at_prob.at(i)) / simulation_num;
    }
    print_metrics("Avg spanning at probability: ", avg_norm_spanning);

    write_results(avg_activation_per_run, avg_norm_spanning, out_path);
}


int main() {
    Configuration config = read_config(std::filesystem::path(RESOURCES_DIR)/ CONFIG_JSON, RESOURCES_DIR);
    print_config(config);

    write_paremeters(config);

    Simulation simulation{config};
    std::cout << " START SUMULATION " << std::endl;
    simulation.run_simulation();
    std::cout << " END SUMULATION " << std::endl;
    Metrics metrics = simulation.get_metrics();
    generate_metrics(metrics, config.get_metrics_file());
}