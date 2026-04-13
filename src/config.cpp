#include "config.h"

Configuration::Configuration(unsigned int sim_number, ProbabilityRange prob_range, 
                             u16 matrix_row, u16 matrix_col, std::string_view dir_path, std::string metrics_file_name): 
        sim_number(sim_number), prob_range(prob_range), matrix_row(matrix_row), matrix_col(matrix_col), dir_path(dir_path), metrics_file_name(metrics_file_name)
{}
