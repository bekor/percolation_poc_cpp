#include "search.h"

#include <algorithm>
#include <queue>
#include <tuple>
#include <vector>
#include <iostream> // for debug

typedef uint16_t u16;

BreathFirstSearch::BreathFirstSearch(u16 cols, u16 rows) : rows(rows), cols(cols){
    size_t len = rows * cols;
    visited.assign(len, 0);
    visited_id = 1;
    bfs_queue.reserve(rows * cols);
}

void BreathFirstSearch::reset_visited(){
    visited_id++;
    if(visited_id == std::numeric_limits<uint32_t>::max())
    {
        std::fill(visited.begin(), visited.end(), 0);
        visited_id = 1;
    }
}

static bool is_end(const std::vector<uint8_t>& matrix, u16 row, u16 col, size_t cols){
    return matrix[cols * row + col]
            && cols-1 == col;   
}

bool BreathFirstSearch::bfs(const std::vector<uint8_t>& matrix, u16 row, u16 col)
{
    size_t start_pos = cols * row + col;
    assert((start_pos < matrix.size()) && "position calculation failed");

    if(matrix[start_pos] == 0)
        return false;

    static constexpr int dr[4] = {-1, 1, 0, 0};
    static constexpr int dc[4] = {0, 0, 1, -1};

    size_t head = 0;
    bfs_queue.clear();
    bfs_queue.push_back(start_pos);

    visited[start_pos] = visited_id;

    while(head < bfs_queue.size())
    {
        auto index = bfs_queue[head];
        row = index / cols;
        col = index % cols;
        head++;
        if(is_end(matrix, row, col, cols)){
            reset_visited();
            return true;
        }

        for(int dir = 0; dir < 4; ++dir)
        {
            int next_row = row + dr[dir];
            int next_col = col + dc[dir];

            if(next_row < 0 || next_row >= rows)
                continue;

            if(next_col < 0 || next_col >= cols)
                continue;

            size_t position = next_row * cols + next_col;

            if(!matrix[position] || visited[position] == visited_id)
                continue;

            visited[position] = visited_id;

            bfs_queue.push_back(position);
        }
    }
    reset_visited();
    return false;
}

bool BreathFirstSearch::bfs(const std::vector<bool>& matrix, u16 row, u16 col)
{
    size_t start_pos = cols * row + col;
    assert((start_pos < matrix.size()) && "position calculation failed");

    if(!matrix[start_pos])
        return false;

    static constexpr int dr[4] = {-1, 1, 0, 0};
    static constexpr int dc[4] = {0, 0, 1, -1};

    size_t head = 0;
    bfs_queue.clear();
    bfs_queue.push_back(start_pos);

    visited[start_pos] = visited_id;

    while(head < bfs_queue.size())
    {
        auto index = bfs_queue[head];
        row = index / cols;
        col = index % cols;
        head++;
        if(matrix[cols * row + col] && cols-1 == col){
            reset_visited();
            return true;
        }

        for(int dir = 0; dir < 4; ++dir)
        {
            int next_row = row + dr[dir];
            int next_col = col + dc[dir];

            if(next_row < 0 || next_row >= rows)
                continue;

            if(next_col < 0 || next_col >= cols)
                continue;

            size_t position = next_row * cols + next_col;

            if(!matrix[position] || visited[position] == visited_id)
                continue;

            visited[position] = visited_id;

            bfs_queue.push_back(position);
        }
    }
    reset_visited();
    return false;
}
