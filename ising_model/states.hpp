#pragma once
#include <vector>

struct States {
    std::vector<std::vector<uint8_t>> matrices;  // [state_idx][r * cols + c]
    std::vector<float>                percents;
    std::vector<uint16_t>             activations;
    size_t                            rows;
    size_t                            cols;
    size_t                            count;
};