#pragma once

#include <vector>

struct Metric {
    float probability;
    uint16_t initial_activation;
    std::vector<std::vector<uint8_t>> evolution_state;
    std::vector<uint8_t> spanning;
};

struct MetricIsing : public Metric {
    int initial_energy;
    std::vector<uint8_t> acceptance;
    std::vector<int> net_energy;
    std::vector<float> beta;
    std::vector<float> correlation;
};