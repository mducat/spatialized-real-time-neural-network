
#ifndef STATIC_DATA_HPP
#define STATIC_DATA_HPP

#include <functional>

#include "data_source.hpp"

class StaticDataSource final : public DataSource {
public:
    explicit StaticDataSource(const std::function<double(double)> &);

    void range(double, double);
    void resolution(std::size_t);

private:
    std::function<double(double)> source;

    void generateValues();

    double rangeMaxX = 1.0;
    double rangeMinX = -1.0;
};

#endif //STATIC_DATA_HPP
