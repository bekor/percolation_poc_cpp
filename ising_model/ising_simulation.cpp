#include "ising_simulation.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include "random.hpp"
#include "search.h"

IsingSimulation::IsingSimulation(size_t rows, size_t cols) : rows(rows), cols(cols){}

bool IsingSimulation::has_spanning_cluster(const std::vector<uint8_t>& state) const {
    bool end_reached = false;
    for(size_t row = 0; row < rows; ++row){
        end_reached = bfs(state, rows, cols, row, 0);
        if(end_reached)
            return end_reached;
    }
    return end_reached;
}

std::vector<size_t> IsingSimulation::get_neighbors(size_t row, size_t col) const{
    static const std::vector<int> direction_row{-1, 1, 0, 0};
    static const std::vector<int> direction_col{0, 0, 1, -1};
    int rows_ = static_cast<int>(rows);
    int cols_ = static_cast<int>(cols);
    std::vector<size_t> neighbors;
    for(size_t dir = 0; dir < direction_row.size(); ++dir)
    {
        int n_row = static_cast<int>(row) + direction_row[dir];
        int n_col = static_cast<int>(col) + direction_col[dir];

        // bounding
        if(n_row < 0)
            n_row = rows_ - 1;
        else if(n_row >= rows_)
            n_row = 0;

        if(n_col < 0)
            n_col = cols_ - 1;
        else if(n_col >= cols_)
            n_col = 0;

        neighbors.push_back(n_row * cols + n_col);
    }
    return neighbors;
}

int IsingSimulation::energy_around_point(const std::vector<uint8_t>& state, size_t row, size_t col, 
                        uint8_t current_value){
    int local_energy = 0;
    auto neighbors = get_neighbors(row, col);

    for(auto neighbor : neighbors){
        assert(neighbor < state.size());
        if(current_value == state[neighbor])
            local_energy++;
        else
            local_energy--;
    }
    return local_energy;
}

int IsingSimulation::calculate_energy(const std::vector<uint8_t>& state){
    int energy = 0;
    for(size_t row = 0; row < rows; ++row) {
        for(size_t col = 0; col < cols; ++col) {
            size_t current_position = row * cols + col;
            uint8_t current_value = state[current_position];
            energy += energy_around_point(state, row, col, current_value);
        }
    }
    return energy / 2; // reduced double count
}

int IsingSimulation::energy_around_neighbor_at_swap(const std::vector<uint8_t>& matrix, const std::vector<size_t>& neighbors, 
                                size_t pos_origin, size_t pos_other){
    uint8_t swapped_value = matrix[pos_origin] == 1 ? 0 : 1;
    int local_energy = 0;

    for(auto neighbor : neighbors){
        int value = matrix[neighbor];
        if (neighbor == pos_other){
            value = value == 1 ? 0 : 1;
        }
        if(value == swapped_value)
            local_energy++;
        else
            local_energy--;
    }
    return local_energy;
}


int IsingSimulation::calculate_energy_diff(const std::vector<uint8_t>& matrix, size_t pos_act, size_t pos_inact){
    size_t row_active   = pos_act / cols;
    size_t col_active   = pos_act % cols;
    size_t row_inactive = pos_inact / cols;
    size_t col_inactive = pos_inact % cols;
    auto initial_value_active   = matrix[pos_act];
    auto initial_value_inactive = matrix[pos_inact];

    auto initial_energy_active   = energy_around_point(matrix, row_active, col_active, initial_value_active);
    auto initial_energy_inactive = energy_around_point(matrix, row_inactive, col_inactive, initial_value_inactive);

    auto neighbors_active   = get_neighbors(row_active, col_active);
    auto neighbors_inactive = get_neighbors(row_inactive, col_inactive);

    int active_final_energy = energy_around_neighbor_at_swap(matrix, neighbors_active, pos_act, pos_inact);
    int inactive_final_energy   = energy_around_neighbor_at_swap(matrix, neighbors_inactive, pos_inact, pos_act);
    
    return (active_final_energy - initial_energy_active) + inactive_final_energy - initial_energy_inactive;
}

MetricIsing IsingSimulation::metropolis(const std::vector<uint8_t>& state, uint32_t iteration, 
                                        uint16_t original_activation, int energy, double inv_temperature){
    MetricIsing metric;
    metric.evolution_state.resize(iteration);
    metric.initial_activation = original_activation;

    std::vector<uint8_t> matrix = state;
    std::vector<size_t> active_idxs, inactive_idxs;
    for (size_t i = 0; i < matrix.size(); ++i) {
        if (matrix[i] == 1)
            active_idxs.push_back(i);
        else
            inactive_idxs.push_back(i);
    }

    uint32_t spanning_count = 0;

    for(uint32_t i = 0; i < iteration; ++i){
        size_t a_idx = uniform_random_size(0, active_idxs.size()   - 1);
        size_t i_idx = uniform_random_size(0, inactive_idxs.size() - 1);
        size_t pos_active   = active_idxs[a_idx];
        size_t pos_inactive = inactive_idxs[i_idx];

        assert(pos_active < matrix.size());
        assert(pos_inactive < matrix.size());

        int energy_diff  = calculate_energy_diff(matrix, pos_active, pos_inactive);
        bool is_accepted = energy_diff <= 0 ||
              uniform_double_random(0.0, 1.0) < std::exp(-1 * inv_temperature * energy_diff);

        if(is_accepted){
            std::swap(matrix[pos_active], matrix[pos_inactive]);
            energy += energy_diff;
        }

        auto is_spanning = has_spanning_cluster(matrix);

        if(is_spanning)
            spanning_count++;
        metric.evolution_state[i] = matrix;
        metric.acceptance.push_back(is_accepted ? 1 : 0);
        metric.net_energy.push_back(energy);
        metric.spanning.push_back(is_spanning);

        // std::cout << "metric size: " << metric.spanning.size() << std::endl;
    }
    std::cout << "iteration count: " << iteration << "\n"
              << "spanning count: " << spanning_count << std::endl;

    return metric;
}

MetricIsing IsingSimulation::run_simulation(const std::vector<uint8_t>& state,
                                            uint16_t original_activation, 
                                            uint32_t simulation_number) 
{
    
    int energy = calculate_energy(state);
    int initial_energy = energy;
    std::cout << "energy: " << energy << std::endl;
    
    float inv_temperature = 0.8;
    MetricIsing metric = metropolis(state, simulation_number, original_activation, energy, inv_temperature);
    metric.initial_activation = original_activation;
    metric.initial_energy = initial_energy;

    std::cout << "metropolis ended for energy " << initial_energy << std::endl;
    return metric;
}