#include "simulation.h"

#include <vector>
#include <algorithm>
#include <iostream>

#include "random.hpp"
#include "search.h"

bool Simulation::has_spanning_cluster(const std::vector<uint8_t>& state) {
    bool end_reached = false;
    for(size_t row = 0; row < rows; ++row){
        end_reached = search.bfs(state, row, 0);
        if(end_reached)
            return end_reached;
    }
    return end_reached;
}

bool Simulation::act_on_matrix(std::vector<uint8_t>& matrix, std::vector<size_t>& active_idxs, std::vector<size_t>& inactive_idxs){
    size_t a_idx = uniform_random_size(0, active_idxs.size()   - 1);
    size_t d_idx = uniform_random_size(0, inactive_idxs.size() - 1);
    size_t pos_active   = active_idxs[a_idx];
    size_t pos_deactive = inactive_idxs[d_idx];

    std::swap(matrix[pos_active], matrix[pos_deactive]);

    active_idxs[a_idx]   = pos_deactive;
    inactive_idxs[d_idx] = pos_active;
    return true;
}

Metric Simulation::run_simulation(const std::vector<uint8_t>& state,
                                            uint16_t original_activation,
                                            uint16_t simulation_number) 
{
    uint16_t spanning_count = 0;
    Metric metric;
    metric.initial_activation = original_activation;

    std::vector<uint8_t> matrix = state;
    std::vector<size_t> active_pos, deactive_pos;
    for (size_t i = 0; i < matrix.size(); ++i) {
        if (matrix[i] == 1)
            active_pos.push_back(i);
        else
            deactive_pos.push_back(i);
    }

    auto is_spanning = has_spanning_cluster(matrix);
    for(uint16_t i = 0; i < simulation_number; ++i) {
        auto is_changed = act_on_matrix(matrix, active_pos, deactive_pos);

        if(is_changed)
            is_spanning = has_spanning_cluster(matrix);
        if(is_spanning) spanning_count++;

        // save metric
        metric.evolution_state.push_back(matrix);
        metric.spanning.push_back(is_spanning);
    }
    uint16_t activation = std::accumulate(matrix.begin(), matrix.end(), 0);
    std::cout << "original activation: " << original_activation << "\n"
              << "final activation: " << activation << "\n"
              << "iteration count: " << simulation_number << "\n"
              << "spanning count: " << spanning_count << std::endl;

    return metric;
}