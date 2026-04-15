#pragma once
#include <iostream>
#include <array>
#include <string_view>

#include "matrix.hpp"
#include "config.h"

template<typename T>
void print_matrix(const Matrix<T>& matrix){
    std::cout << "----------\n";
    for(size_t row = 0; row < matrix.rows(); ++row){
        for(size_t col = 0; col < matrix.cols(); ++col){
            std::cout << matrix[row, col] << " | ";
        }
        std::cout << std::endl;
    }
}

void print_bit_matrix(const Matrix<bool>& matrix);

void print_metrics(std::string_view title, const std::array<double, 200>& metrics);

void print_config(const Configuration& config);