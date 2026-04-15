#include "interaction.h"

#include <vector>
#include <tuple>

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

void interaction_around_cell(const Matrix<u16>& prob_matrix, 
                            Matrix<bool>& matrix, 
                            u16 interaction_radius, u16 row, u16 col) 
{
    auto coords_neighbors = get_neighbors(matrix, row, col);
    std::vector<u16> prob_neighbors{};
    std::vector<bool> states_neighbors{};
    for (auto const& [row_n, col_n] : coords_neighbors){
        prob_neighbors.push_back(prob_matrix.at(row_n, col_n));
    }
}

void interaction(const Matrix<u16>& prob_matrix, 
                            Matrix<bool>& matrix, 
                            u16 interaction_radius) 
{
    for(auto row = 0; row < prob_matrix.rows(); ++row){
        for(auto col = 0; col < prob_matrix.cols(); ++col){
            // only calculate interaction if the position is activated
            // if(matrix[row][col])
            //     interaction_around_cell(prob_matrix, matrix,interaction_radius, row, col);
        }
    }

}