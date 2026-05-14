#include "ising_simulation.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include "random.hpp"
#include "search.h"

constexpr float CRITICAL_TEMP{2.27};
constexpr float CRITICAL_BETA{1.0f / CRITICAL_TEMP}; // ≈ 0.4405


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

std::vector<size_t> IsingSimulation::get_neighbors(size_t row, size_t col) const {
    static const std::vector<int> direction_row{-1, 1, 0, 0};
    static const std::vector<int> direction_col{0, 0, 1, -1};
    int rows_ = static_cast<int>(rows);
    int cols_ = static_cast<int>(cols);
    std::vector<size_t> neighbors;
    for(size_t dir = 0; dir < direction_row.size(); ++dir)
    {
        int n_row = (static_cast<int>(row) + direction_row[dir] + rows_) % rows_;
        int n_col = (static_cast<int>(col) + direction_col[dir] + cols_) % cols_;
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
        int value = state[neighbor] == 0 ? -1 : 1;
        local_energy += value;
    }
    int point_value = current_value == 0 ? -1 : 1;
    return -1 * point_value * local_energy;
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

int IsingSimulation::calculate_energy_diff(const std::vector<uint8_t>& matrix, size_t pos_act, size_t pos_inact){
    size_t row_active   = pos_act / cols;
    size_t col_active   = pos_act % cols;
    size_t row_inactive = pos_inact / cols;
    size_t col_inactive = pos_inact % cols;

    int dE = -2 * energy_around_point(matrix, row_active,   col_active,   matrix[pos_act])
             -2 * energy_around_point(matrix, row_inactive, col_inactive, matrix[pos_inact]);

    // adjacency correction
    auto neighbors_active = get_neighbors(row_active, col_active);
    auto are_neighbors = std::find(neighbors_active.begin(), neighbors_active.end(), pos_inact);
    if (are_neighbors != neighbors_active.end()) {
        int sa = matrix[pos_act]    == 0 ? -1 : 1;
        int sb = matrix[pos_inact]  == 0 ? -1 : 1;
        dE += -4 * sa * sb;
    }
    return dE;
}

std::vector<float> IsingSimulation::pre_calculate_betas(uint32_t iteration, double beta_initial) const {
    uint32_t stride = 4;
    float delta = (CRITICAL_BETA - beta_initial) / static_cast<float>(stride - 1);
    
    std::vector<float> betas;
    betas.reserve(iteration);
    
    for (uint32_t step = 0; step < stride; ++step) {
        float beta = beta_initial + step * delta;
        uint32_t count = iteration / stride;
        
        if (step == stride - 1)
            count += iteration % stride;
        for (uint32_t j = 0; j < count; ++j)
            betas.push_back(beta);
    }
    return betas;
}

double IsingSimulation::neighbor_correlation_diff(
    const std::vector<uint8_t>& matrix,
    size_t row, size_t col, size_t ignore_pos,
    int diff_spin) const
{
    // difference is symmetric if they are neighbors -> contribution is 0 so we skipp
    //contribution will change in 4 direction not 2
    double diff = 0.0;
    for (size_t nb : get_neighbors(row, col)) {
        if (nb == ignore_pos) continue;
        int spin_nb = matrix[nb] == 0 ? -1 : 1;
        diff += (double)(diff_spin) * spin_nb;
    }
    return diff;
}

double IsingSimulation::calculate_correlation_diff(
    const std::vector<uint8_t>& matrix,
    size_t pos_act,
    size_t pos_inact) const
{
    size_t row_active   = pos_act   / cols;
    size_t col_active   = pos_act   % cols;
    size_t row_inactive = pos_inact / cols;
    size_t col_inactive = pos_inact % cols;

    int spin_act_old = matrix[pos_act]   == 0 ? -1 : 1;
    int spin_inact_old = matrix[pos_inact] == 0 ? -1 : 1;
    // after swap, values flip
    int diff_act_spin = -2*spin_act_old;
    int diff_inact_spin = -2*spin_inact_old;

    double diff_correlation = 0.0;
    // active site, 
    diff_correlation += neighbor_correlation_diff(matrix, row_active, col_active, pos_inact, diff_act_spin);
    // neighbors of inactive site
    diff_correlation += neighbor_correlation_diff(matrix, row_inactive, col_inactive, pos_act, diff_inact_spin);
    
    return diff_correlation;
}


double IsingSimulation::calculate_correlation_sum(const std::vector<uint8_t>& matrix) const {
    double sum{0.0};

    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            int spin_i = matrix[row * cols + col] == 0 ? -1 : 1;

            size_t right_neighbor{row * cols + (col + 1) % cols};
            int spin_right = matrix[right_neighbor] == 0 ? -1 : 1;

            size_t down_neighbor{((row + 1) % rows) * cols + col};
            int spin_down = matrix[down_neighbor] == 0 ? -1 : 1;

            sum += spin_i * spin_right;
            sum += spin_i * spin_down;
        }
    }
    return sum;
}

double IsingSimulation::calculate_avg_spin(const std::vector<uint8_t>& matrix) const {
    int sum = std::accumulate(matrix.begin(), matrix.end(), 0,
                [](int acc, uint8_t spin){ return acc + (spin == 0 ? -1 : 1);});
    return static_cast<double>(sum) / matrix.size();
}

MetricIsing IsingSimulation::metropolis(const std::vector<uint8_t>& state, uint32_t iteration, 
                                        uint16_t original_activation, int energy, double beta,
                                        bool is_moving_betas){
    MetricIsing metric;
    metric.evolution_state.reserve(iteration / 1000 + 1);
    metric.initial_activation = original_activation;
    if(is_moving_betas)
        metric.beta = pre_calculate_betas(iteration, beta);
    else{
        metric.beta = std::vector<float>(iteration, beta);
    }

    std::vector<uint8_t> matrix = state;
    std::vector<size_t> active_idxs, inactive_idxs;
    for (size_t i = 0; i < matrix.size(); ++i) {
        if (matrix[i] == 1)
            active_idxs.push_back(i);
        else
            inactive_idxs.push_back(i);
    }

    double corr_sum = calculate_correlation_sum(matrix);
    int pair_count = 2 * rows * cols;
    double average_spin = calculate_avg_spin(matrix);
    double correlation = (corr_sum / pair_count) - average_spin * average_spin;
    std::cout<< "correlation: " << correlation << " corr sum : " << corr_sum << std::endl;

    uint32_t spanning_count = 0;

    for(uint32_t i = 0; i < iteration; ++i){
        size_t a_idx = uniform_random_size(0, active_idxs.size()   - 1);
        size_t i_idx = uniform_random_size(0, inactive_idxs.size() - 1);
        size_t pos_active   = active_idxs[a_idx];
        size_t pos_inactive = inactive_idxs[i_idx];
        double inv_temperature = metric.beta[i];

        assert(pos_active < matrix.size());
        assert(pos_inactive < matrix.size());
        int energy_diff  = calculate_energy_diff(matrix, pos_active, pos_inactive);
        float acceptance_prob = std::exp(-1 * inv_temperature * energy_diff);
        bool is_accepted = energy_diff <= 0 || uniform_double_random(0.0, 1.0) < acceptance_prob;

        if(is_accepted){
            corr_sum += calculate_correlation_diff(matrix, pos_active, pos_inactive);
            correlation = (corr_sum / pair_count) - average_spin * average_spin;
            std::swap(matrix[pos_active], matrix[pos_inactive]);
            energy += energy_diff;
            active_idxs[a_idx]   = pos_inactive;
            inactive_idxs[i_idx] = pos_active;
        }

        auto is_spanning = has_spanning_cluster(matrix);

        if(is_spanning)
            spanning_count++;
        if(i % 1000 == 0){
            metric.evolution_state.push_back(matrix);
        }
        
        metric.correlation.push_back(correlation);
        metric.acceptance.push_back(is_accepted ? 1 : 0);
        metric.net_energy.push_back(energy);
        metric.spanning.push_back(is_spanning);
    }
    std::cout << "iteration count: " << iteration << "\n"
              << "spanning count: " << spanning_count << std::endl;

    return metric;
}

MetricIsing IsingSimulation::run_simulation(const std::vector<uint8_t>& state,
                                            uint16_t original_activation, 
                                            uint32_t simulation_number,
                                            float beta, bool is_moving_betas) 
{
    
    int energy = calculate_energy(state);
    int initial_energy = energy;
    std::cout << "energy: " << energy << ", beta: " << beta << std::endl;
    
    MetricIsing metric = metropolis(state, simulation_number, original_activation, energy, beta, is_moving_betas);
    metric.initial_activation = original_activation;
    metric.initial_energy = initial_energy;

    std::cout << "metropolis ended for energy " << initial_energy << std::endl;
    return metric;
}