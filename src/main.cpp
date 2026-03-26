
#include <array>
#include <random>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <string_view>

#include "printer.h"
#include "search.h"
#include "file_io.h"

typedef uint16_t u16;
typedef uint32_t u32;
typedef std::array<u32, 200> metrics_array;
using prb_matrix = std::array<std::array<u16, 3>, 3>;     // TODO: if bitset size changes this have to change too!!
const std::string_view OUTPUT_FILE = "percolation_metrics.txt";

auto uniform_random() {
    static std::uniform_int_distribution<u16> distrib{
        0, std::numeric_limits<u16>::max()
    };
    static std::random_device device;
    static std::mt19937 engine{device()};
    return distrib(engine);
}

void populate_prob_matrix(prb_matrix& matrix) {
    for (auto& row : matrix)
        std::generate(row.begin(), row.end(), uniform_random);
}

void set_activation_matrix(u16 probability, const prb_matrix& p_matrix, std::bitset<9>& matrix) {
    // TODO: if bitset size changes this have to change too!!
    constexpr size_t MAX_COLS = 3;
    for(size_t row = 0; row < p_matrix.size(); ++row){
        for(size_t col = 0; col < p_matrix.at(0).size(); ++col){
            matrix[row * MAX_COLS + col] = (p_matrix[row][col] <= probability);
        }
    }
}

bool has_spanning_cluster(const std::bitset<9>& matrix){
    if (matrix.none())
        return false;

    bool end_reached = false;
    // TODO: if bitset size changes this have to change too!!
    for(int start = 0; start < 7; start += 3){
        end_reached = bfs(matrix, start);
        if(end_reached)
            return end_reached;
    }
    return end_reached;
}

std::tuple<bool, u16> run_matrix_prob(u16 probability, const prb_matrix& p_matrix, std::bitset<9>& matrix) {
    set_activation_matrix(probability, p_matrix, matrix);
    return {has_spanning_cluster(matrix), matrix.count()};
}


void generate_metrics(const metrics_array& activation_per_run,
                      const metrics_array& spanning_at_prob, u32 simulation_num){
    std::array<double, 200> avg_activation_per_run{};
    for(u32 i = 0; i < activation_per_run.size(); ++i){
        double norm_activation = static_cast<double>(activation_per_run.at(i)) / simulation_num;
        avg_activation_per_run.at(i) = norm_activation;
    }

    print_metrics("Avg activation at probability: ", avg_activation_per_run);

    std::array<double, 200> avg_norm_spanning{};
    for(u32 i = 0; i < spanning_at_prob.size(); ++i){
        avg_norm_spanning.at(i) = static_cast<double>(spanning_at_prob.at(i)) / simulation_num;
    }
    print_metrics("Avg spanning at probability: ", avg_norm_spanning);

    write_results(avg_activation_per_run, avg_norm_spanning, OUTPUT_FILE);
}


int main() {
    const unsigned int simulation_num = 10000;

    const u16 max = std::numeric_limits<u16>::max();
    const auto scale = [&](int prob) -> u16 {
        return static_cast<u16>((static_cast<double>(prob) / 100.0) * max);
    };

    const int prob_from = 15;
    const int prob_to = 85;
    const u16 scaled_prob_from = scale(prob_from);
    const u16 scaled_prob_to = scale(prob_to);

    const u16 prob_step = (scaled_prob_to - scaled_prob_from) / 199;

    std::cout << "from : " << scaled_prob_from 
              << ", to: " << scaled_prob_to 
              << " step: " << prob_step << "\n";
    write_paremeters(simulation_num, scaled_prob_from, scaled_prob_to, prob_step, OUTPUT_FILE);

    prb_matrix probability_matrix{};
    std::bitset<9> matrix{};

    metrics_array activation_per_run{};
    metrics_array spanning_at_prob{};
    for(unsigned int i = 0; i < simulation_num; ++i){
        for(u16 prob = scaled_prob_from; prob < scaled_prob_to; prob += prob_step){
            u16 step = (prob-scaled_prob_from)/prob_step;
            populate_prob_matrix(probability_matrix);
            auto [spanning, population] = run_matrix_prob(prob, probability_matrix, matrix);
            if(spanning){
                // print_bit_matrix(matrix);
                spanning_at_prob.at(step) += 1;
            }
            activation_per_run.at(step) += population;
        }
    }
    generate_metrics(activation_per_run, spanning_at_prob, simulation_num);
}