#pragma once
#include <array>
#include <string_view>
#include "config.h"

typedef uint16_t u16;
typedef uint32_t u32;

void write_results(const std::array<double, 200>& avg_activation, 
                   const std::array<double, 200>& avg_spanning, 
                   std::string_view filename);

void write_paremeters(u32 sim_count, const ProbabilityRange& prob_range, std::string_view filename);