cmake_minimum_required(VERSION 3.20)
project(percolation_poc VERSION 1.0 LANGUAGES CXX)

# ── C++ standard ────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Compiler warnings ────────────────────────────────────────────
add_compile_options(-Wall -Wextra -Wpedantic)

# ── Main executable ──────────────────────────────────────────────
file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(percolation_poc ${SRC_FILES})
target_include_directories(percolation_poc PRIVATE src)

# ── Tests (Catch2 via brew) ──────────────────────────────────────
find_package(Catch2 REQUIRED)

file(GLOB_RECURSE TEST_FILES test/*.cpp)
# exclude main.cpp from test build
list(FILTER SRC_FILES EXCLUDE REGEX ".*main\\.cpp$")

add_executable(tests ${TEST_FILES} ${SRC_FILES})
target_include_directories(tests PRIVATE src)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

enable_testing()
add_test(NAME unit_tests COMMAND tests)