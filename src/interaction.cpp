#include "interaction.h"

#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

std::vector<std::tuple<u16, u16>> get_neighbors(const Matrix<bool>& matrix, u16 row, u16 col){
    static const std::vector<int> direction_row{-1, 1, 0, 0};
    static const std::vector<int> direction_col{0, 0, 1, -1};

    std::vector<std::tuple<u16, u16>> neighbors{};
    for(size_t i = 0; i < direction_row.size(); ++i){
        int next_row = row + direction_row[i];
        int next_col = col + direction_col[i];

        // bounding check
        if(next_row < 0 || next_row >= matrix.rows())
            continue;
        if(next_col < 0 || next_col >= matrix.cols())
            continue;
        neighbors.push_back({next_row, next_col});
    }
    return neighbors;
}

void interaction_around_cell(Matrix<u16>& prob_matrix, 
                            Matrix<bool>& matrix, 
                            u16 interaction_radius, u16 row, u16 col) 
{
    auto coord_neighbors = get_neighbors(matrix, row, col);
    std::vector<u16> prob_neighbors{};
    std::vector<bool> states_neighbors{};
    u16 cell_probability = prob_matrix.at(row, col);
    // std::cout << "activated cell: " << row << ", " << col << " with: " << cell_probability << "\n";
    float sum_of_probabilities = cell_probability;
    for (auto const& [row_n, col_n] : coord_neighbors){
        auto neighbor_prob = prob_matrix.at(row_n, col_n);
        prob_neighbors.push_back(neighbor_prob);
        sum_of_probabilities += neighbor_prob;
    }

    u16 u16_max = std::numeric_limits<u16>::max();
    sum_of_probabilities /= (5 * u16_max);
    float sigmoid =  0.5 * (sum_of_probabilities / (1 + sum_of_probabilities) + 1);
    u16 descending_step = static_cast<float>(cell_probability) * sigmoid;
    // std::cout << "step: " << descending_step << " sig: " << sigmoid << " sm prob: " << sum_of_probabilities << "\n";
    for (auto const& [row_n, col_n] : coord_neighbors){
        if(!matrix[row_n, col_n]){
            if(u16_max - prob_matrix[row_n, col_n] > descending_step){
                prob_matrix[row_n, col_n] += descending_step;
                // std::cout << "HERE is the step: " << prob_matrix[row_n, col_n] << ", ";
            }
        }
    }
    // std::cout << std::endl;
}

Matrix<u16> interaction(const Matrix<u16>& prob_matrix, 
                 Matrix<bool>& matrix, 
                 u16 interaction_radius) 
{
    auto next_prob_matrix = prob_matrix;
    for(auto row = 0; row < prob_matrix.rows(); ++row){
        for(auto col = 0; col < prob_matrix.cols(); ++col){
            // only calculate interaction if the position is activated
            if(matrix[row, col])
                interaction_around_cell(next_prob_matrix, matrix, interaction_radius, row, col);
        }
    }
    return next_prob_matrix;
}