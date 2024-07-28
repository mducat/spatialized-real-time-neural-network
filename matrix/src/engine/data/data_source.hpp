
#ifndef DATA_SOURCE_HPP
#define DATA_SOURCE_HPP

#include <deque>

class DataSource {
public:
    virtual ~DataSource() = default;

    std::deque<double> getValues() { return values; };

    [[nodiscard]] double getMinValue() const { return minValue; };
    [[nodiscard]] double getMaxValue() const { return maxValue; }

    [[nodiscard]] double getMinRange() const { return minRange; };
    [[nodiscard]] double getMaxRange() const { return maxRange; }

    [[nodiscard]] std::size_t getSize() const { return size; }

protected:
    std::deque<double> values;

    double minValue = 0;
    double maxValue = 0;

    double minRange = - 1.0;
    double maxRange = 1.0;

    std::size_t size = 500;
};

#endif //DATA_SOURCE_HPP
