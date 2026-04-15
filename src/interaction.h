#pragma once
#include "matrix.hpp"

typedef uint16_t u16;

void interaction(const Matrix<u16>& prob_matrix, Matrix<bool>& matrix, u16 interaction_radius);
