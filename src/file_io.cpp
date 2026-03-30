#include "file_io.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

json try_to_open(const std::filesystem::path& path) {
    if(!std::filesystem::exists(path)) {
        std::cerr << "Cannot open config file: " << path << std::endl;
    }

    std::ifstream file(path);
    json data;
    try {
        if (file.is_open() && file.peek() != std::ifstream::traits_type::eof())
            data = json::parse(file);
    }
    catch (json::parse_error& ex){
        std::cerr << "Parse error at byte " << ex.byte << std::endl;
    }
    return data;
}

Configuration read_config(const std::filesystem::path& path){
    json data = try_to_open(path);

    unsigned int sim_num = data["simulation_number"].get<unsigned int>();
    u16 mx_row    = data["matrix_rows"].get<u16>();
    u16 mx_col    = data["matrix_cols"].get<u16>();
    u16 from      = data["probability"]["from"].get<u16>();
    u16 to        = data["probability"]["to"].get<u16>();
    u16 step      = data["probability"]["step"].get<u16>();
    return Configuration{
        sim_num,
        ProbabilityRange{from, to, step},
        mx_row, mx_col
    };
}

void write_results(const std::array<double, 200>& avg_activation, 
                   const std::array<double, 200>& avg_spanning, 
                   const std::filesystem::path& path) {
    json out_json = try_to_open(path);
    out_json["metrics"] = {
        {"activation", avg_activation},
        {"spanning"  , avg_spanning}
    };
    
    std::ofstream file(path);
    file << out_json.dump(4);
}

void write_paremeters(const Configuration& config, 
                      const std::filesystem::path& path) {
    auto prob_range = config.get_probability_range();
    u16 from_prob = prob_range.scaled_prob_from();
    u16 to_prob = prob_range.scaled_prob_to();
    u16 prob_step = prob_range.prob_step();

    json out_json = try_to_open(path);
    out_json["config"] = {
        {"simulation_number", config.simulation_number()},
        {"prob_from"        , from_prob},
        {"prob_to"          , to_prob},
        {"steps"            , prob_step},
        {"matrix_row"       , config.get_matrix_row()},
        {"matrix_col"       , config.get_matrix_col()}
    };

    std::ofstream file(path);
    file << out_json.dump(4);
}