//
// Created by thornhill on 03/01/25.
//

#pragma once

#include <network_object.hpp>


class Dataset;

class DatasetEntry : public NetworkObject {
public:

    explicit DatasetEntry(const std::shared_ptr<Dataset> &, std::size_t, std::size_t = 0);
    ~DatasetEntry() override;

    void update(double) override;
    [[nodiscard]] double value() const override;

private:
    std::size_t _tensor_index;
    std::size_t _series_index;

};
