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

bool bfs(const std::vector<uint8_t>& matrix, u16 rows, u16 cols, u16 start_row, u16 start_col, 
         std::vector<uint32_t> visited, uint32_t visit_id)
{
    size_t start_pos = cols * start_row + start_col;
    assert((start_pos < matrix.size()) && "position calculation failed");

    if(matrix[start_pos] == 0)
        return false;

    static constexpr int dr[4] = {-1, 1, 0, 0};
    static constexpr int dc[4] = {0, 0, 1, -1};

    std::queue<std::pair<u16,u16>> coords_queue;
    coords_queue.push({start_row, start_col});

    visited[start_pos] = visit_id;

    while(!coords_queue.empty())
    {
        auto [row, col] = coords_queue.front();
        coords_queue.pop();
        if(is_end(matrix, row, col, cols))
            return true;

        for(int dir = 0; dir < 4; ++dir)
        {
            int next_row = row + dr[dir];
            int next_col = col + dc[dir];

            if(next_row < 0 || next_row >= rows)
                continue;

            if(next_col < 0 || next_col >= cols)
                continue;

            size_t position = next_row * cols + next_col;

            if(!matrix[position] || visited[position] == visit_id)
                continue;

            visited[position] = visit_id;

            coords_queue.push({
                static_cast<u16>(next_row),
                static_cast<u16>(next_col)
            });
        }
    }

    return false;
}
