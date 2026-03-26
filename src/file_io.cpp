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

void write_paremeters(u16 sim_count, u16 from_prob, u16 to_prob, u16 prob_step, std::string_view filename) {
    std::ofstream file(filename);
    file << "SIM_COUNT:" << sim_count
         << ",FROM:" << from_prob
         << ",TO:" << to_prob
         << ",STEP:" << prob_step
         << "\n";
}