#include "search.h"
#include <catch2/catch_test_macros.hpp>
#include <bitset>


// Helper to build a bitset from a readable 3x3 grid
// "111 111 111" = all connected
std::bitset<9> make_matrix(const std::string& s) {
    std::bitset<9> m;
    int bit = 0;
    for (char c : s) {
        if (c == '0' || c == '1') {
            m[bit++] = (c == '1');
        }
    }
    return m;
}

TEST_CASE("bfs on fully connected matrix", "[bfs]") {
    // 1 1 1
    // 1 1 1
    // 1 1 1
    auto matrix = make_matrix("111 111 111");
    REQUIRE(bfs(matrix, 0) == true);
    REQUIRE(bfs(matrix, 4) == true);
    REQUIRE(bfs(matrix, 8) == true);
}

TEST_CASE("bfs on empty matrix", "[bfs]") {
    // 0 0 0
    // 0 0 0
    // 0 0 0
    std::bitset<9> matrix;  // all zero
    REQUIRE(bfs(matrix, 0) == false);
}

TEST_CASE("bfs starting from inactive node", "[bfs]") {
    // 1 1 1
    // 0 0 1
    // 1 1 1
    auto matrix = make_matrix("111 001 111");
    REQUIRE(bfs(matrix, 3) == false);  // start node is off
}

TEST_CASE("bfs on one path matrix", "[bfs]") {
    // 1 1 1
    // 1 0 0
    // 1 0 0
    auto matrix = make_matrix("111 100 100");
    REQUIRE(bfs(matrix, 0) == true);   // straight line
    REQUIRE(bfs(matrix, 3) == true);   // up and right
    REQUIRE(bfs(matrix, 6) == true);  // up and left
}


TEST_CASE("bfs on partially connected matrix", "[bfs]") {
    // 1 1 0
    // 0 0 0
    // 0 0 1
    auto matrix = make_matrix("110 000 001");
    REQUIRE(bfs(matrix, 0) == false);   // top-left cluster reachable
    REQUIRE(bfs(matrix, 3) == false);   // isolated node, reachable from itself
    REQUIRE(bfs(matrix, 6) == false);  // inactive node
}

TEST_CASE("bfs on edgecase continuous array", "[bfs]") {
    // 0 1 1
    // 1 0 1
    // 1 0 0
    auto matrix = make_matrix("011 101 100");
    REQUIRE(bfs(matrix, 3) == false);  // previous can be idx 2
    REQUIRE(bfs(matrix, 6) == false);  // previous can be idx 5
}

TEST_CASE("bfs on edgecase co", "[bfs]") {
    // 0 1 1
    // 1 0 1
    // 1 0 0
    auto matrix = make_matrix("111 110 001");
    REQUIRE(bfs(matrix, 0) == true);  // previous can be idx 2
    REQUIRE(bfs(matrix, 6) == false);  // previous can be idx 5
}