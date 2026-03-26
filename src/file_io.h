#pragma once
#include <array>
#include <string_view>

typedef uint16_t u16;

void write_results(const std::array<double, 200>& avg_activation, 
                   const std::array<double, 200>& avg_spanning, 
                   std::string_view filename);

void write_paremeters(u16 sim_count, u16 from_prob, u16 to_prob, u16 prob_step, std::string_view filename);