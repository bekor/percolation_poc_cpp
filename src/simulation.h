#pragma once
#include "config.h"
#include <array>
#include <bitset>
#include <cstdint>
#include <algorithm>

typedef uint16_t u16;
typedef uint32_t u32;
typedef std::array<u32, 200> metrics_array;
using prb_matrix = std::array<std::array<u16, 3>, 3>;     // TODO: if bitset size changes this have to change too!!


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

    static auto uniform_random();
    void populate_prob_matrix(prb_matrix& matrix);
    void set_activation_matrix(u16 probability, const prb_matrix& prob_matrix, std::bitset<9>& matrix);
    bool has_spanning_cluster(const std::bitset<9>& matrix);
};

