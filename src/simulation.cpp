#include "simulation.h"
#include "search.h"
#include <random>
#include <iostream>

void Simulation::run_simulation(){
    prb_matrix probability_matrix{};
    std::bitset<9> matrix{};

    const unsigned int sim_num = config.get_simulation_number();
    const u16 from = config.get_probability_range().scaled_prob_from();
    const u16 to = config.get_probability_range().scaled_prob_to();
    const u16 prob_step = config.get_probability_range().prob_step();
    metrics.simulation_num = sim_num;
    
    for(unsigned int i = 0; i < sim_num; ++i){
        populate_prob_matrix(probability_matrix);
        bool has_spanning = false;
        for(u16 prob = from; prob < to; prob += prob_step){
            u16 step = (prob - from) / prob_step;
            set_activation_matrix(prob, probability_matrix, matrix);
            auto population = matrix.count();

            if(has_spanning){
                metrics.spanning_at_prob.at(step) += 1;
            }
            else{
                bool spanning = has_spanning_cluster(matrix);
                if(spanning){
                    has_spanning = true;
                    // print_bit_matrix(matrix);
                    metrics.spanning_at_prob.at(step) += 1;
                }
            }
            metrics.activation_per_run.at(step) += population;
        }
    }
}

auto Simulation::uniform_random() {
    static std::uniform_int_distribution<u16> distrib{
        0, std::numeric_limits<u16>::max()
    };
    static std::random_device device;
    static std::mt19937 engine{device()};
    return distrib(engine);
}

void Simulation::populate_prob_matrix(prb_matrix& matrix) {
    for (auto& row : matrix)
        std::generate(row.begin(), row.end(), uniform_random);
}

void Simulation::set_activation_matrix(u16 probability, const prb_matrix& prob_matrix, std::bitset<9>& matrix) {
    // TODO: if bitset size changes this have to change too!!
    constexpr size_t MAX_COLS = 3;
    for(size_t row = 0; row < prob_matrix.size(); ++row){
        for(size_t col = 0; col < prob_matrix.at(0).size(); ++col){
            matrix[row * MAX_COLS + col] = (prob_matrix[row][col] <= probability);
        }
    }
}

bool Simulation::has_spanning_cluster(const std::bitset<9>& matrix){
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

