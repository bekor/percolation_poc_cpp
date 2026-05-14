#pragma once
#include <string>
#include <vector>

struct PercolationConfig {
    std::string input_file;
    float       percent_low;
    float       percent_high;
};
 
struct UncorrelatedConfig {
    std::string output_file;
    size_t      sim_count;
};
 
struct IsingConfig {
    std::vector<float> betas;
    size_t             sim_count;
    bool               save_evolution;
    bool               moving_betas;
};