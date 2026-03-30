#pragma once
#include "config.h"
#include "matrix.hpp"

#include <array>
#include <cstdint>
#include <algorithm>


typedef uint16_t u16;
typedef uint32_t u32;
typedef std::array<u32, 200> metrics_array;

struct Metrics {
    metrics_array activation_per_run{};
    metrics_array spanning_at_prob{};
    int simulation_num = 0;
};

class Simulation {
public:
    Simulation(const Configuration& config): config(config) {};
    void run_simulation();

    const Metrics& get_metrics(){ return metrics; };

private:
    const Configuration& config;
    Metrics metrics{};

    void populate_prob_matrix(Matrix<u16>& matrix);
    void set_activation_matrix(u16 probability, const Matrix<u16>& prob_matrix, Matrix<bool>& matrix);
    bool has_spanning_cluster(const Matrix<bool>& matrix);
};

