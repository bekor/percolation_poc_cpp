#pragma once

#include "states.hpp"
#include "metric.hpp"
#include <vector>

class Simulation {
public:
    Metric run_simulation(const std::vector<uint8_t>& state, size_t rows, size_t cols, uint16_t original_activation, uint16_t simulation_number);
protected:
    // shared helpers if needed
    bool has_spanning_cluster(std::vector<uint8_t> state, size_t rows, size_t cols) const;
    bool act_on_matrix(std::vector<uint8_t>& matrix, std::vector<size_t>& active_idxs, std::vector<size_t>& inactive_idxs);
};