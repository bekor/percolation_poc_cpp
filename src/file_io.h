#pragma once
#include <array>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "config.h"
using json = nlohmann::json;

typedef uint16_t u16;
typedef uint32_t u32;

Configuration read_config(const std::filesystem::path& path);

void write_results(const std::array<double, 200>& avg_activation, 
                   const std::array<double, 200>& avg_spanning, 
                   const std::filesystem::path& path);

void write_paremeters(const Configuration& config, 
                      const std::filesystem::path& path);

json try_to_open(const std::filesystem::path& path);