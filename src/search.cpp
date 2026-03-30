#include "search.h"

#include <algorithm>
#include <queue>
#include <tuple>
#include <vector>

typedef uint16_t u16;

static bool is_end(const Matrix<bool>& matrix, u16 current_row, u16 current_col){
    return matrix(current_row, current_col) 
            && (matrix.cols()-1) == current_col;   
}

std::vector<std::tuple<u16, u16>> find_neighbors(const Matrix<bool>& matrix, u16 row, u16 col, std::vector<unsigned int>& visited){
    // north, south, east, west direction
    static const std::vector<int> direction_row{-1, 1, 0, 0};
    static const std::vector<int> direction_col{0, 0, 1, -1};

    auto is_visited = [&visited](u16 position) -> bool {
        return (std::find(visited.begin(), visited.end(), position) != visited.end());
    };

    std::vector<std::tuple<u16, u16>> neighbors{};
    for(size_t i = 0; i < direction_row.size(); ++i){
        int next_row = row + direction_row[i];
        int next_col = col + direction_col[i];

        // bounding check
        if(next_row < 0 || next_row >= matrix.rows())
            continue;
        if(next_col < 0 || next_col >= matrix.cols())
            continue;

        int position = next_row * matrix.cols() + next_col;

        if(!matrix(next_row, next_col) || is_visited(position))
            continue;

        visited.push_back(position);
        neighbors.push_back({next_row, next_col});
    }
    return neighbors;
}

bool bfs(const Matrix<bool>& matrix, u16 start_row, u16 start_col){
    if(matrix.at(start_row, start_col) == 0)
        return false;
    bool end_reached = false;
    std::queue<std::pair<u16, u16>> coord_queue;
    coord_queue.push({start_row, start_col});

    std::vector<unsigned int> visited{}; // visited coordinates
    visited.push_back(matrix.cols() * start_row + start_col);

    while(!coord_queue.empty()){
        auto curr_coord = coord_queue.front();
        u16 current_row = std::get<0>(curr_coord);
        u16 current_col = std::get<1>(curr_coord);
        coord_queue.pop();

        if(is_end(matrix, current_row, current_col)){
            end_reached = true;
            break;
        }
        auto neighbors = find_neighbors(matrix, current_row, current_col, visited);
        for(auto [nxt_row, nxt_col] : neighbors){
            coord_queue.push({nxt_row, nxt_col});
        }
    }
    return end_reached;
}