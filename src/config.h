#pragma once
#include <cstdint>
#include <algorithm>
#include <filesystem>
#include <string_view>
#include <sstream>

constexpr std::string_view METRICS_EXTENSION = ".txt";
typedef uint16_t u16;

class ProbabilityRange {
public:
    ProbabilityRange(u16 from, u16 to, u16 step_number): 
                    from(from), to(to), step_number(step_number){};
    ~ProbabilityRange() = default;

    u16 scaled_prob_from() const { return scale(from); }
    u16 scaled_prob_to() const { return scale(to); }
    u16 prob_step() const { return (scale(to) - scale(from)) / step_number; };

    u16 get_from() const { return from; }
    u16 get_to() const { return to; }
    u16 get_step_number() const { return step_number; }

private:
    const u16 max = std::numeric_limits<u16>::max();
    u16 from;
    u16 to;
    u16 step_number;
    int scale(int prob) const {
        return static_cast<u16>((static_cast<double>(prob) / 100.0) * max);
    };
};


class Configuration{
public:
    Configuration(unsigned int sim_number, ProbabilityRange prob_range, 
                  u16 matrix_row, u16 matrix_col,
                  std::string_view dir_path,
                  std::string metrics_file_name);
    ~Configuration() = default;

    unsigned int simulation_number() const { return sim_number; }
    const ProbabilityRange& get_probability_range() const { return prob_range; }
    u16 get_matrix_row() const { return matrix_row; }
    u16 get_matrix_col() const { return matrix_col; }
    std::filesystem::path get_metrics_file() const { 
        std::stringstream sstm;
        sstm << "_" << std::to_string(matrix_row) 
             << "x" << std::to_string(matrix_col) 
             << "_" << std::to_string(sim_number) << METRICS_EXTENSION;
        std::string file_spec = sstm.str();
        return std::filesystem::path(dir_path)/(metrics_file_name+file_spec);
    }


private:
    unsigned int sim_number;
    ProbabilityRange prob_range;
    u16 matrix_row;
    u16 matrix_col;
    std::string_view dir_path;
    std::string metrics_file_name;
};