#pragma once

#include "states.hpp"
#include "metric.hpp"
#include "search.h"
#include <vector>

class Simulation {
public:
    Simulation(size_t rows, size_t cols): rows(rows), cols(cols), search(rows, cols){};
    Metric run_simulation(const std::vector<uint8_t>& state, uint16_t original_activation, uint16_t simulation_number);
protected:
    // shared helpers if needed
    bool has_spanning_cluster(const std::vector<uint8_t>& state);
    bool act_on_matrix(std::vector<uint8_t>& matrix, std::vector<size_t>& active_idxs, std::vector<size_t>& inactive_idxs);
private:
    size_t rows; 
    size_t cols;
    BreathFirstSearch search;
};