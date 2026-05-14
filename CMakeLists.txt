cmake_minimum_required(VERSION 3.20)
project(percolation_poc VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

# ── 3rd Party libs ──────────────────────────────────────────────
## nlohmann json
include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

## HDF5
FetchContent_Declare(
    HighFive
    GIT_REPOSITORY https://github.com/highfive-devs/highfive.git
    GIT_TAG        v3.3.0
)
set(HIGHFIVE_USE_BOOST OFF)
set(HIGHFIVE_USE_EIGEN OFF)
set(HIGHFIVE_EXAMPLES OFF)
set(HIGHFIVE_UNIT_TESTS OFF)
FetchContent_MakeAvailable(HighFive)
message(STATUS "HighFive source: ${highfive_SOURCE_DIR}")

# ── Reference resources ──────────────────────────────────────────
configure_file(
    ${CMAKE_SOURCE_DIR}/resources/config.json
    ${CMAKE_BINARY_DIR}/resources/config.json
    COPYONLY
)

# ── Main executable ──────────────────────────────────────────────
set(PERC_H 
    src/simulation.h
    src/printer.h
    src/file_io.h
    src/state_dump.h
    src/config.h
    src/interaction.h
    src/search.h
)

set(PERC_SRC
    src/printer.cpp
    src/interaction.cpp
    src/file_io.cpp
    src/config.cpp
    src/search.cpp
    src/simulation.cpp
    src/state_dump.cpp
    src/main.cpp
)

add_executable(percolation_poc ${PERC_H} ${PERC_SRC})
target_include_directories(percolation_poc PRIVATE
    src
    ${highfive_SOURCE_DIR}/include
)
target_link_libraries(percolation_poc PRIVATE
    nlohmann_json::nlohmann_json
    HighFive::HighFive
)
target_compile_definitions(percolation_poc PRIVATE
    RESOURCES_DIR="${CMAKE_BINARY_DIR}/resources/"
)
target_compile_definitions(percolation_poc PRIVATE
    OUT_DIR="${CMAKE_BINARY_DIR}/../out_simulations/"
)

# ── Ising model app ──────────────────────────────────────────────
set(ISING_H
    ising_model/states.hpp
    ising_model/metric.hpp
    ising_model/random.hpp
    ising_model/metric_writer.h
    ising_model/h5parser.h
    ising_model/simulation.h
    ising_model/ising_simulation.h
    ising_model/search.h
)

set(ISING_SRC
    ising_model/h5parser.cpp
    ising_model/metric_writer.cpp
    ising_model/search.cpp
    ising_model/simulation.cpp
    ising_model/ising_simulation.cpp
    ising_model/main.cpp
)

add_executable(ising_model ${ISING_SRC} ${ISING_H})
target_include_directories(ising_model PRIVATE
    ising_model
    ${highfive_SOURCE_DIR}/include
)

target_link_libraries(ising_model PRIVATE
    HighFive::HighFive
    nlohmann_json::nlohmann_json
)
target_compile_definitions(ising_model PRIVATE
    RESOURCES_DIR="${CMAKE_BINARY_DIR}/resources/"
)

# ── Tests (Catch2 via brew) ──────────────────────────────────────
find_package(Catch2 REQUIRED)


file(GLOB_RECURSE TEST_FILES test/*.cpp)
set(PERC_H 
    src/simulation.h
    src/printer.h
    src/file_io.h
    src/state_dump.h
    src/config.h
    src/interaction.h
    src/search.h
)

set(PERC_SRC
    src/printer.cpp
    src/interaction.cpp
    src/file_io.cpp
    src/config.cpp
    src/search.cpp
    src/simulation.cpp
    src/state_dump.cpp
)

set(ISING_TEST_SRC
    ising_model/ising_simulation.cpp
    ising_model/search.cpp
)

add_executable(tests ${TEST_FILES} ${PERC_H} ${PERC_SRC} ${ISING_TEST_SRC})
target_include_directories(tests PRIVATE
    src
    ising_model
    ${highfive_SOURCE_DIR}/include
)
target_link_libraries(tests PRIVATE
    Catch2::Catch2WithMain
    nlohmann_json::nlohmann_json
    HighFive::HighFive
)

enable_testing()
add_test(NAME unit_tests COMMAND tests)