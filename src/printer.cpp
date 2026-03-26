#include "printer.h"
#include <numeric>

// TODO: if bitset size changes this have to change too!!
const static size_t ROW = 3;
const static size_t COL = 3;

void print_bit_matrix(const std::bitset<9>& matrix){
    std::cout << "The matrix: " << std::endl;
    for(size_t row = 0; row < ROW; ++row){
        for(size_t col = 0; col < COL; ++col){
            std::cout << matrix[row*3 + col];
        }
        std::cout << std::endl;
    }
    std::cout << " ==== END === " << std::endl;
}

void print_metrics(std::string_view title, const std::array<double, 200>& metrics) {
    std::cout << title << "\n";
    for(size_t i = 0; i < metrics.size(); ++i){
        std::cout << "index of prob: " << i << " avg value: " << metrics.at(i) << "\n";
    }
    std::cout << "-----------------" << std::endl;
}