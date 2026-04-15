#pragma once
#include "matrix.hpp"

typedef uint16_t u16;
typedef uint32_t u32;

Matrix<u16> interaction(const Matrix<u16>& prob_matrix, Matrix<bool>& matrix, u16 interaction_radius);
