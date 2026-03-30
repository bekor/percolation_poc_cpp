#include "printer.h"
#include <numeric>

void print_bit_matrix(const Matrix<bool>& matrix){
    std::cout << "The matrix: " << std::endl;
    for(size_t row = 0; row < matrix.rows(); ++row){
        for(size_t col = 0; col < matrix.cols(); ++col){
            std::cout << matrix(row, col);
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

void print_config(const Configuration& config){
    std::cout << "Number of simulation run : " << config.simulation_number() << "\n"
                << " from: " << config.get_probability_range().scaled_prob_from() 
                << " to: " << config.get_probability_range().scaled_prob_to() 
                << " step: " << config.get_probability_range().prob_step()
                << "\n";
}