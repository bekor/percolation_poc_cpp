#include "metric_writer.h"


MetricWriter::MetricWriter(const std::string& path, size_t rows, size_t cols, 
                           size_t n_metrics, size_t n_steps)
    : file_(path, HighFive::File::Truncate), rows_(rows), cols_(cols),
      steps_(n_steps), count_(0)
{
    size_t flat = rows * cols;

    HighFive::DataSetCreateProps props1;
    props1.add(HighFive::Chunking({64}));

    // fixed size — no UNLIMITED needed
    ds_probability_ = file_.createDataSet<float>(
        "probability", HighFive::DataSpace({n_metrics}), props1
    );
    ds_initial_activation_ = file_.createDataSet<uint16_t>(
        "initial_activation", HighFive::DataSpace({n_metrics}), props1
    );

    HighFive::DataSetCreateProps props2;
    props2.add(HighFive::Chunking({1, n_steps}));
    ds_spanning_ = file_.createDataSet<uint8_t>(
        "spanning", HighFive::DataSpace({n_metrics, n_steps}), props2
    );

    HighFive::DataSetCreateProps props3;
    props3.add(HighFive::Chunking({1, 1, flat}));
    props3.add(HighFive::Deflate(4));
    ds_evolution_ = file_.createDataSet<uint8_t>(
        "evolution", HighFive::DataSpace({n_metrics, n_steps, flat}), props3
    );
}

void MetricWriter::write(const Metric& metric)
{
    size_t flat = rows_ * cols_;

    ds_probability_.select({count_}, {1}).write_raw(&metric.probability);
    ds_initial_activation_.select({count_},  {1}).write_raw(&metric.initial_activation);

    ds_spanning_.select({count_, 0}, {1, steps_})
                .write_raw(metric.spanning.data());

    for (size_t s = 0; s < steps_; ++s)
        ds_evolution_.select({count_, s, 0}, {1, 1, flat})
                     .write_raw(metric.evolution_state[s].data());

    ++count_;
}


IsingMetricWriter::IsingMetricWriter(const std::string& path, size_t rows, size_t cols, 
                           size_t n_metrics, size_t n_steps, bool has_evolution)
    : file_(path, HighFive::File::Truncate), rows_(rows), cols_(cols),
      steps_(n_steps), count_(0), has_evolution_(has_evolution)
{
    size_t flat = rows * cols;

    HighFive::DataSetCreateProps props1;
    size_t chunk1 =std::min(n_metrics, size_t{64});
    props1.add(HighFive::Chunking({chunk1}));

    // fixed size — no UNLIMITED needed
    ds_probability_ = file_.createDataSet<float>(
        "probability", HighFive::DataSpace({n_metrics}), props1
    );
    ds_initial_activation_ = file_.createDataSet<uint16_t>(
        "initial_activation", HighFive::DataSpace({n_metrics}), props1
    );
    ds_initial_energy_ = file_.createDataSet<uint16_t>(
        "initial_energy", HighFive::DataSpace({n_metrics}), props1
    );
    ds_beta_ = file_.createDataSet<float>(
        "beta", HighFive::DataSpace({n_metrics}), props1
    );
    
    HighFive::DataSetCreateProps props2;
    props2.add(HighFive::Chunking({1, n_steps}));
    ds_spanning_ = file_.createDataSet<uint8_t>(
        "spanning", HighFive::DataSpace({n_metrics, n_steps}), props2
    );

    props2.add(HighFive::Chunking({1, n_steps}));
    ds_acceptance_ = file_.createDataSet<uint8_t>(
        "acceptance", HighFive::DataSpace({n_metrics, n_steps}), props2
    );

    props2.add(HighFive::Chunking({1, n_steps}));
    ds_net_energy_ = file_.createDataSet<int>(
        "net_energy", HighFive::DataSpace({n_metrics, n_steps}), props2
    );

    if(has_evolution){
        HighFive::DataSetCreateProps props3;
        props3.add(HighFive::Chunking({1, 1, flat}));
        props3.add(HighFive::Deflate(4));
        ds_evolution_ = file_.createDataSet<uint8_t>(
            "evolution", HighFive::DataSpace({n_metrics, n_steps, flat}), props3
        );
    }
}

void IsingMetricWriter::write(const MetricIsing& metric)
{
    size_t flat = rows_ * cols_;

    ds_probability_.select({count_}, {1}).write_raw(&metric.probability);
    ds_initial_activation_.select({count_},  {1}).write_raw(&metric.initial_activation);
    ds_initial_energy_.select({count_},  {1}).write_raw(&metric.initial_energy);
    ds_beta_.select({count_}, {1}).write_raw(&metric.beta);
    
    ds_spanning_.select({count_, 0}, {1, steps_})
                .write_raw(metric.spanning.data());

    ds_acceptance_.select({count_, 0}, {1, steps_})
            .write_raw(metric.acceptance.data());

    ds_net_energy_.select({count_, 0}, {1, steps_})
            .write_raw(metric.net_energy.data());

    if(has_evolution_){
        for (size_t s = 0; s < steps_; ++s)
            ds_evolution_.select({count_, s, 0}, {1, 1, flat})
                        .write_raw(metric.evolution_state[s].data());
    }

    ++count_;
}
