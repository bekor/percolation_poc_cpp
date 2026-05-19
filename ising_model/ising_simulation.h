#pragma once
#include "metric.hpp"
#include "search.h"
#include <array>
#include <vector>

class IsingSimulation {
public:
    IsingSimulation(size_t rows, size_t cols);
    MetricIsing run_simulation(const std::vector<uint8_t>& state, uint16_t original_activation, uint32_t simulation_number, float beta, bool is_moving_betas);


    bool has_spanning_cluster(const std::vector<uint8_t>& state);
    int energy_around_point(const std::vector<uint8_t>& state, size_t row, size_t col, uint8_t current_value);
    int calculate_energy(const std::vector<uint8_t>& state);
    MetricIsing metropolis(const std::vector<uint8_t>& state, uint32_t iteration, 
                          int energy, double beta, bool is_moving_betas);
    int calculate_energy_diff(const std::vector<uint8_t>& matrix, size_t pos_act, size_t pos_inact);
    std::array<size_t, 4> get_neighbors(size_t row, size_t col) const;
    std::vector<float> pre_calculate_betas(uint32_t iteration, double beta) const;
    double calculate_avg_spin(const std::vector<uint8_t>& matrix) const;
    double neighbor_correlation_diff(const std::vector<uint8_t>& matrix, size_t row, size_t col, size_t ignore_pos, int diff_spin) const;
    double calculate_correlation_diff(const std::vector<uint8_t>& matrix, size_t pos_act, size_t pos_inact) const;
    
    double calculate_correlation_sum(const std::vector<uint8_t>& matrix) const;
    
private:
    size_t rows;
    size_t cols;
    BreathFirstSearch search;

    std::vector<std::array<size_t,4>> neighbor_table;
    void precaculate_neighbors();
};