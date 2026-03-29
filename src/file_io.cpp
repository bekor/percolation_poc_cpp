#include "file_io.h"

#include <fstream>

void write_results(const std::array<double, 200>& avg_activation, 
                   const std::array<double, 200>& avg_spanning, 
                   std::string_view filename) {
    std::ofstream file(filename, std::ios::app);
    file << "ACTIVATION:\n";
    for (double v : avg_activation)
        file << v << "\n";
    file << "SPANNING:\n";
    for (double v : avg_spanning)
        file << v << "\n";
}

void write_paremeters(u32 sim_count, const ProbabilityRange& prob_range, std::string_view filename) {
    u16 from_prob = prob_range.scaled_prob_from();
    u16 to_prob = prob_range.scaled_prob_to();
    u16 prob_step = prob_range.prob_step();

    std::ofstream file(filename);
    file << "SIM_COUNT:" << sim_count
         << ",FROM:" << from_prob
         << ",TO:" << to_prob
         << ",STEP:" << prob_step
         << "\n";
}