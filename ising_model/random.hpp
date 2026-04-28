#pragma once
#include <random>

inline std::mt19937& rng() {
    static thread_local std::mt19937 engine{std::random_device{}()};
    return engine;
}

inline uint32_t uniform_random(uint32_t from, uint32_t to) {
    return std::uniform_int_distribution<uint32_t>(from, to)(rng());
}

inline size_t uniform_random_size(size_t from, size_t to) {
    return std::uniform_int_distribution<size_t>(from, to)(rng());
}

inline double uniform_double_random(double from, double to) {
    return std::uniform_real_distribution<double>(from, to)(rng());
}