#pragma once
#include <iostream>
#include <array>
#include <string_view>

#include "matrix.hpp"
#include "config.h"

template<typename T, size_t ROW, size_t COL>
void print_matrix(const std::array<std::array<T, COL>, ROW>& matrix){
    for(size_t row = 0; row < ROW; ++row){
        for(size_t col = 0; col < COL; ++col){
            std::cout << matrix.at(row).at(col);
        }
        std::cout << std::endl;
    }
}

void print_bit_matrix(const Matrix<bool>& matrix);

void print_metrics(std::string_view title, const std::array<double, 200>& metrics);

void print_config(const Configuration& config);