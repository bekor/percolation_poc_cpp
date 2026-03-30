#include "simulation.h"

#include <random>

#include "search.h"

namespace {
    u16 uniform_random() {
        static std::uniform_int_distribution<u16> distrib{
            0, std::numeric_limits<u16>::max()
        };
        static std::random_device device{};
        static std::mt19937 engine{device()};
        return distrib(engine);
    }
}

void Simulation::run_simulation(){
    Matrix<u16> probability_matrix{config.get_matrix_row(), config.get_matrix_col()};
    Matrix<bool> matrix{config.get_matrix_row(), config.get_matrix_col()};

    const unsigned int sim_num = config.simulation_number();
    const u16 from = config.get_probability_range().scaled_prob_from();
    const u16 prob_step = config.get_probability_range().prob_step();
    const u16 num_steps = config.get_probability_range().get_step_number();

    metrics.simulation_num = sim_num;
    for(unsigned int i = 0; i < sim_num; ++i){
        populate_prob_matrix(probability_matrix);
        bool has_spanning = false;
        for(u16 step = 0; step < num_steps; ++step){
            u16 prob = from + step * prob_step;
            set_activation_matrix(prob, probability_matrix, matrix);
            auto population = matrix.count(true);

            if(!has_spanning){
                has_spanning = has_spanning_cluster(matrix);
            }
            if(has_spanning){
                metrics.spanning_at_prob.at(step) += 1;
            }
            metrics.activation_per_run.at(step) += population;
        }
    }
}

void Simulation::populate_prob_matrix(Matrix<u16>& matrix) {
    std::generate(matrix.begin(), matrix.end(), uniform_random);
}

void Simulation::set_activation_matrix(u16 probability, const Matrix<u16>& prob_matrix, Matrix<bool>& matrix) {
    for(size_t row = 0; row < prob_matrix.rows(); ++row){
        for(size_t col = 0; col < prob_matrix.cols(); ++col){
            matrix.at(row, col) = (prob_matrix(row, col) <= probability);
        }
    }
}

bool Simulation::has_spanning_cluster(const Matrix<bool>& matrix){
    // if (matrix.count(false) == matrix.size())
    //     return false;

    bool end_reached = false;
    for(int row = 0; row < matrix.rows(); ++row){
        end_reached = bfs(matrix, row, 0);
        if(end_reached)
            return end_reached;
    }
    return end_reached;
}

