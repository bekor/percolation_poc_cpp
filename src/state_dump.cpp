#include "state_dump.h"


StateDump::StateDump(const std::string& path, hsize_t rows, hsize_t cols)
    : file_(path, hf::File::Truncate), rows_(rows), cols_(cols), count_(0)
{
    hf::DataSetCreateProps props3;
    props3.add(hf::Chunking({1, rows, cols}));
    props3.add(hf::Deflate(4));

    dset_states_ = file_.createDataSet<uint8_t>(
        "states",
        hf::DataSpace({0, rows, cols}, {hf::DataSpace::UNLIMITED, rows, cols}),
        props3
    );

    hf::DataSetCreateProps props1;
    props1.add(hf::Chunking({64}));

    dset_percent_ = file_.createDataSet<float>(
        "percent",
        hf::DataSpace({0}, {hf::DataSpace::UNLIMITED}),
        props1
    );
    dset_activation_ = file_.createDataSet<uint16_t>(
        "activation",
        hf::DataSpace({0}, {hf::DataSpace::UNLIMITED}),
        props1
    );
}

void StateDump::write(const Matrix<bool>& mat, float percent, u16 activation)
{

    const auto& raw = mat.data();
    std::vector<uint8_t> buf(raw.size());
    for (size_t i = 0; i < raw.size(); ++i)
        buf[i] = raw[i] ? 1 : 0;

    dset_states_.resize({count_ + 1, rows_, cols_});
    dset_states_.select({count_, 0, 0}, {1, rows_, cols_}).write_raw(buf.data());

    append_scalar(dset_percent_, percent);
    append_scalar(dset_activation_, activation);

    ++count_;
}
