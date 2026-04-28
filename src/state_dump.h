#pragma once

#include <highfive/highfive.hpp>
#include <cstdint>
#include <string>

#include "matrix.hpp"
typedef uint16_t u16;
namespace hf = HighFive;


template <typename S>
concept Scalar = std::is_arithmetic_v<S>;

class StateDump {
public:
    StateDump(const std::string& path, hsize_t rows, hsize_t cols);
    ~StateDump() = default;

    void write(const Matrix<bool>& mat, float percent, u16 activation);

private:
    hf::File    file_;
    hf::DataSet dset_states_;
    hf::DataSet dset_percent_;
    hf::DataSet dset_activation_;
    hsize_t     rows_, cols_, count_;

    template <Scalar S>
    void append_scalar(hf::DataSet& dset, const S val){
        dset.resize({count_ + 1});
        dset.select({count_}, {1}).write_raw(&val);
    }
};