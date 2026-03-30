cmake_minimum_required(VERSION 3.20)
project(percolation_poc VERSION 1.0 LANGUAGES CXX)

# ── C++ standard ────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── 3rd Party libs ──────────────────────────────────────────────
include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3  # latest stable
)
FetchContent_MakeAvailable(nlohmann_json)

# ── Reference resources ──────────────────────────────────────────
configure_file(
    ${CMAKE_SOURCE_DIR}/resources/config.json
    ${CMAKE_BINARY_DIR}/resources/config.json
    COPYONLY
)

# ── Compiler warnings ────────────────────────────────────────────
add_compile_options(-Wall -Wextra -Wpedantic)

# ── Main executable ──────────────────────────────────────────────
file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(percolation_poc ${SRC_FILES})
target_include_directories(percolation_poc PRIVATE src)

target_link_libraries(percolation_poc PRIVATE nlohmann_json::nlohmann_json)

# ── Reference resources ──────────────────────────────────────────
target_compile_definitions(percolation_poc PRIVATE
    RESOURCES_DIR="${CMAKE_BINARY_DIR}/resources/"
)

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