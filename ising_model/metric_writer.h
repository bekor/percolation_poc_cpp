#pragma once

#include <highfive/highfive.hpp>
#include <cstdint>
#include <string>

#include "metric.hpp"


class MetricWriter {
public:
    MetricWriter(const std::string& path, size_t rows, size_t cols, size_t n_metrics, size_t n_steps);

    void write(const Metric& metric);

private:
    HighFive::File    file_;
    HighFive::DataSet ds_probability_;
    HighFive::DataSet ds_initial_activation_;
    HighFive::DataSet ds_evolution_;   // (n_metrics, n_steps, rows*cols)
    HighFive::DataSet ds_spanning_;    // (n_metrics, n_steps)
    size_t            rows_;
    size_t            cols_;
    size_t            steps_;          // evolution_state length — fixed per write
    size_t            count_;

    template <typename T>
    void append_scalar(HighFive::DataSet& dset, T val);
};

class IsingMetricWriter {
public:
    IsingMetricWriter(const std::string& path, size_t rows, size_t cols, size_t n_metrics, size_t n_steps, bool has_evolution);

    void write(const MetricIsing& metric);

private:
    HighFive::File    file_;
    HighFive::DataSet ds_probability_;
    HighFive::DataSet ds_initial_activation_;
    HighFive::DataSet ds_initial_energy_;
    HighFive::DataSet ds_evolution_;   // (n_metrics, n_steps, rows*cols)
    HighFive::DataSet ds_spanning_;    // (n_metrics, n_steps)
    HighFive::DataSet ds_acceptance_;    // (n_metrics, n_steps)
    HighFive::DataSet ds_net_energy_;    // (n_metrics, n_steps)
    HighFive::DataSet ds_beta_;
    HighFive::DataSet ds_correlation_; // (n_metrics, n_steps)
    size_t            rows_;
    size_t            cols_;
    size_t            steps_;          // evolution_state length — fixed per write
    size_t            count_;
    bool              has_evolution_;

    template <typename T>
    void append_scalar(HighFive::DataSet& dset, T val);
};