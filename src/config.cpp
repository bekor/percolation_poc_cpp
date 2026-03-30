#include "config.h"

Configuration::Configuration(unsigned int sim_number, ProbabilityRange prob_range, 
                             u16 matrix_row, u16 matrix_col): 
        sim_number(sim_number), prob_range(prob_range), matrix_row(matrix_row), matrix_col(matrix_col)
{}
