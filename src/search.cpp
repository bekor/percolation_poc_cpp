#include "search.h"

#include <algorithm>
#include <deque>
#include <array>
#include <vector>
#include <iostream>

typedef uint16_t u16;
constexpr std::array<u16, 3> end_coors{2,5,8}; // TODO: if bitset size changes this have to change too!!

// TODO: if bitset size changes this have to change too!!
static bool is_end(const std::bitset<9>& matrix, int current){
    return matrix[current] 
            && (std::find(end_coors.begin(), end_coors.end(), current) != end_coors.end());   
}

std::vector<int> find_neighbors(const std::bitset<9>& matrix, int pos, std::bitset<9>& visited){
    // north, south, east, west direction
    static const std::array<int, 4> direction_row{-1, 1, 0, 0};
    static const std::array<int, 4> direction_col{0, 0, 1, -1};
    std::vector<int> neighbors{};
    for(size_t i = 0; i < direction_row.size(); ++i){
        int row = direction_row[i];
        int col = direction_col[i];
        // bounding check
        int next = pos + col + (3*row);// TODO: if bitset size changes this have to change too!!

        if(next < 0 || next > 8)
            continue;
        if(matrix[next] == 0 || visited[next] == 1)
            continue;
        // special case for continues array
        if((pos == 3 && next == 2) || (pos == 6 && next == 5)) // TODO: if bitset size changes this have to change too!!
            continue;

        visited[next] = 1;
        neighbors.push_back(next);
    }
    return neighbors;
}

// TODO: if bitset size changes this have to change too!!
bool bfs(const std::bitset<9>& matrix, int start){
    if(matrix[start] == 0)
        return false;
    bool end_reached = false;
    std::deque<int> neighbors_queue{start};
    std::bitset<9> visited{}; // TODO: if bitset size changes this have to change too!!
    visited[start] = 1;

    while(!neighbors_queue.empty()){
        int current = neighbors_queue.front();
        neighbors_queue.pop_front();
        if(is_end(matrix, current)){
            end_reached = true;
            break;
        }
        auto neighbors = find_neighbors(matrix, current, visited);
        for(auto neighbor : neighbors){
            neighbors_queue.push_back(neighbor);
        }
    }

    return end_reached;
}