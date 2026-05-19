#include "search.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "matrix.hpp"

// Helper to build a bitset from a readable 3x3 grid
// "111 111 111" = all connected
Matrix<bool> make_matrix(const std::string& s, u16 rows, u16 cols) {
    Matrix<bool> m{rows, cols};
    u16 row = 0, col = 0;
    for (char c : s) {
        if (c == '0' || c == '1') {
            m.at(row, col) = (c == '1');
            ++col;
            if (col == cols) { col = 0; ++row; }
        }
    }
    return m;
}

TEST_CASE("bfs on fully connected matrix", "[bfs]") {
    // 1 1 1
    // 1 1 1
    // 1 1 1
    BreathFirstSearch b{3,3};
    auto matrix = make_matrix("111 111 111", 3, 3);

    REQUIRE(b.bfs(matrix.data(), 0, 0) == true);
    REQUIRE(b.bfs(matrix.data(), 1, 1) == true);
    REQUIRE(b.bfs(matrix.data(), 2, 2) == true);
}

TEST_CASE("bfs on empty matrix", "[bfs]") {
    // 0 0 0
    // 0 0 0
    // 0 0 0
    BreathFirstSearch b{3,3};
    Matrix<bool> matrix{3, 3};
    REQUIRE(b.bfs(matrix.data(), 0, 0) == false);
}

TEST_CASE("bfs starting from inactive node", "[bfs]") {
    // 1 1 1
    // 0 0 1
    // 1 1 1
    BreathFirstSearch b{3,3};
    auto matrix = make_matrix("111 001 111", 3, 3);
    REQUIRE(b.bfs(matrix.data(), 1, 0) == false);
}

TEST_CASE("bfs on one path matrix", "[bfs]") {
    // 1 1 1
    // 1 0 0
    // 1 0 0
    BreathFirstSearch b{3,3};
    auto matrix = make_matrix("111 100 100", 3, 3);
    REQUIRE(b.bfs(matrix.data(), 0, 0) == true); // straight line
    REQUIRE(b.bfs(matrix.data(), 1, 0) == true);   // up and right
    REQUIRE(b.bfs(matrix.data(), 2, 0) == true);  // up and left
}


TEST_CASE("bfs on partially connected matrix", "[bfs]") {
    // 1 1 0
    // 0 0 0
    // 0 0 1
    BreathFirstSearch b{3,3};
    auto matrix = make_matrix("110 000 001", 3, 3);
    REQUIRE(b.bfs(matrix.data(), 0, 0) == false);   // top-left cluster reachable
    REQUIRE(b.bfs(matrix.data(), 1, 0) == false);   // isolated node, reachable from itself
    REQUIRE(b.bfs(matrix.data(), 2, 0) == false);  // inactive node
}

TEST_CASE("bfs on edgecase continuous array", "[bfs]") {
    // 0 1 1
    // 1 0 1
    // 1 0 0
    BreathFirstSearch b{3,3};
    auto matrix = make_matrix("011 101 100", 3, 3);
    REQUIRE(b.bfs(matrix.data(), 1, 0) == false);  // previous can be idx 2
    REQUIRE(b.bfs(matrix.data(), 2, 0) == false);  // previous can be idx 5
}

TEST_CASE("bfs on edgecase co", "[bfs]") {
    // 0 1 1
    // 1 0 1
    // 1 0 0
    BreathFirstSearch b{3,3};
    auto matrix = make_matrix("111 110 001", 3, 3);
    REQUIRE(b.bfs(matrix.data(), 0, 0) == true);  // previous can be idx 2
    REQUIRE(b.bfs(matrix.data(), 2, 0) == false);  // previous can be idx 5
}