
#ifndef DATA_SOURCE_HPP
#define DATA_SOURCE_HPP

#include <deque>

class DataSource {
public:
    virtual ~DataSource() = default;

    std::deque<double> getValues() { return values; };

    [[nodiscard]] double getMinValue() const { return minValue; };
    [[nodiscard]] double getMaxValue() const { return maxValue; }

    [[nodiscard]] std::size_t getSize() const { return size; }

protected:
    std::deque<double> values;

    double minValue = 0;
    double maxValue = 0;

    std::size_t size = 500;
};

#endif //DATA_SOURCE_HPP
