#pragma once
#include <iostream>
#include <array>
#include <tuple>
#include <bitset>
#include <string_view>

template<typename T, size_t ROW, size_t COL>
void print_matrix(const std::array<std::array<T, COL>, ROW>& matrix){
    for(size_t row = 0; row < ROW; ++row){
        for(size_t col = 0; col < COL; ++col){
            std::cout << matrix.at(row).at(col);
        }
        std::cout << std::endl;
    }
}

// TODO: if bitset size changes this have to change too!!
void print_bit_matrix(const std::bitset<9>& matrix);

void print_metrics(std::string_view title, const std::array<double, 200>& metrics);