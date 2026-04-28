#pragma once
#include "metric.hpp"

class IsingSimulation {
public:
    IsingSimulation(size_t rows, size_t cols);
    MetricIsing run_simulation(const std::vector<uint8_t>& state, uint16_t original_activation, uint32_t simulation_number);
private:
    size_t rows;
    size_t cols;

    bool has_spanning_cluster(const std::vector<uint8_t>& state) const;
    int energy_around_point(const std::vector<uint8_t>& state, size_t row, size_t col, uint8_t current_value);
    int calculate_energy(const std::vector<uint8_t>& state);
    MetricIsing metropolis(const std::vector<uint8_t>& state, uint32_t iteration, 
                            uint16_t original_activation, int energy, double temperature_BJ);
    int calculate_energy_diff(const std::vector<uint8_t>& matrix, size_t pos_act, size_t pos_inact);
    int energy_around_neighbor_at_swap(const std::vector<uint8_t>& matrix, const std::vector<size_t>& neighbors, 
                                size_t pos_origin, size_t pos_other);
    std::vector<size_t> get_neighbors(size_t row, size_t col) const;

};