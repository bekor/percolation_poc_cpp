#include "search.h"

#include <algorithm>
#include <queue>
#include <tuple>
#include <vector>
#include <iostream> // for debug

typedef uint16_t u16;

static bool is_end(const std::vector<uint8_t>& matrix, u16 row, u16 col, size_t cols){
    return matrix[cols * row + col]
            && cols-1 == col;   
}

std::vector<std::tuple<u16, u16>> find_neighbors(const std::vector<uint8_t>& matrix,
                                                int rows, int cols, 
                                                u16 row, u16 col, std::vector<bool>& visited){
    // north, south, east, west direction
    static const std::vector<int> direction_row{-1, 1, 0, 0};
    static const std::vector<int> direction_col{0, 0, 1, -1};

    std::vector<std::tuple<u16, u16>> neighbors{};
    for(size_t i = 0; i < direction_row.size(); ++i){
        int next_row = row + direction_row[i];
        int next_col = col + direction_col[i];

        // bounding check
        if(next_row < 0 || next_row >= rows)
            continue;
        if(next_col < 0 || next_col >= cols)
            continue;

        size_t position = next_row * cols + next_col;

        if(!matrix[position] || visited[position])
            continue;

        visited[position] = true;
        neighbors.push_back({next_row, next_col});
    }
    return neighbors;
}

bool bfs(const std::vector<uint8_t>& matrix, u16 rows, u16 cols, u16 start_row, u16 start_col) {
    size_t position = cols * start_row + start_col;
    assert((position < matrix.size()) && "position calculation failed");
    if(matrix.at(position) == 0)
        return false;
    bool end_reached = false;
    std::queue<std::pair<u16, u16>> coord_queue;
    coord_queue.push({start_row, start_col});

    std::vector<bool> visited(rows * cols, false);
    visited[position] = true;

    while(!coord_queue.empty()){
        auto curr_coord = coord_queue.front();
        u16 current_row = std::get<0>(curr_coord);
        u16 current_col = std::get<1>(curr_coord);
        coord_queue.pop();

        if(is_end(matrix, current_row, current_col, cols)){
            end_reached = true;
            break;
        }
        auto neighbors = find_neighbors(matrix, rows, cols, current_row, current_col, visited);
        for(auto [nxt_row, nxt_col] : neighbors){
            coord_queue.push({nxt_row, nxt_col});
        }
    }
    return end_reached;
}