#pragma once
#include <vector>

typedef uint16_t u16;

class BreathFirstSearch{
public:
    BreathFirstSearch(u16 rows, u16 cols);
    bool bfs(const std::vector<uint8_t>& matrix, u16 row, u16 col);
private:
    void reset_visited();
    u16 rows;
    u16 cols;
    std::vector<uint32_t> visited;
    uint32_t visited_id;
    std::vector<uint32_t> bfs_queue;
};