#include "config.h"

int ProbabilityRange::scale(int prob) const {
        return static_cast<u16>((static_cast<double>(prob) / 100.0) * max);
}

Configuration::Configuration(unsigned int sim_number, ProbabilityRange prob_range, 
                             u16 matrix_row, u16 matrix_col, 
                             u16 interaction_radius, std::string interaction_type,
                             std::string_view dir_path, std::string metrics_file_name): 
        sim_number(sim_number), prob_range(prob_range), matrix_row(matrix_row), 
        matrix_col(matrix_col), interaction_radius(interaction_radius), interaction_type(interaction_type),
        dir_path(dir_path), metrics_file_name(metrics_file_name)
{}

std::filesystem::path Configuration::get_metrics_file() const { 
        std::stringstream sstm;
        sstm << "_" << std::to_string(matrix_row) 
                << "x" << std::to_string(matrix_col) 
                << "_" << std::to_string(sim_number) << METRICS_EXTENSION;
        std::string file_spec = sstm.str();
        return std::filesystem::path(dir_path)/(metrics_file_name+file_spec);
}

std::filesystem::path Configuration::get_state_dump_file() const { 
        std::stringstream sstm;
        sstm << "_" << std::to_string(matrix_row) 
                << "x" << std::to_string(matrix_col) 
                << "_" << std::to_string(sim_number) << STATE_DUMP_EXTENSION;
        std::string file_spec = sstm.str();
        return std::filesystem::path(dir_path)/(metrics_file_name+file_spec);
}