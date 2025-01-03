//
// Created by thornhill on 03/01/25.
//

#pragma once

#include <network_object.hpp>


class Dataset;

class DatasetInput : public NetworkObject {
public:

    explicit DatasetInput(std::shared_ptr<Dataset>, std::size_t index);
    ~DatasetInput() override;

    void update(double) override;
    [[nodiscard]] double value() const override;

private:
    std::size_t _index;

};
