
#ifndef DYNAMIC_DATA_HPP
#define DYNAMIC_DATA_HPP

#include <functional>

#include "data_source.hpp"

class DynamicDataSource final : public DataSource {
public:
    explicit DynamicDataSource(const std::function<double()> &);

    void recordValue();

private:
    std::function<double()> source;

};

#endif //DYNAMIC_DATA_HPP
