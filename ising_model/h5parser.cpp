#include "h5parser.h"
#include <highfive/highfive.hpp>
#include <vector>

States load_states(const std::string& path) {
    HighFive::File file(path, HighFive::File::ReadOnly);

    auto ds     = file.getDataSet("states");
    auto dims   = ds.getSpace().getDimensions();

    States out;
    out.count = dims[0];
    out.rows  = dims[1];
    out.cols  = dims[2];

    file.getDataSet("percent").read(out.percents);
    file.getDataSet("activation").read(out.activations);

    out.matrices.resize(out.count);
    for (size_t s = 0; s < out.count; ++s) {
        out.matrices[s].resize(out.rows * out.cols);
        ds.select({s, 0, 0}, {1, out.rows, out.cols})
          .read_raw(out.matrices[s].data());
    }

    return out;
}