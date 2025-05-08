
#ifndef DATA_SOURCE_HPP
#define DATA_SOURCE_HPP

#include <deque>

// @TODO: link scale (i.e. time)

class DataSource {
public:
    virtual ~DataSource() = default;

    std::deque<double> getValues() { return _values; };

    [[nodiscard]] double getMinValue() const { return _min_value; };
    [[nodiscard]] double getMaxValue() const { return _max_value; }

    [[nodiscard]] double getMinRange() const { return _min_range; };
    [[nodiscard]] double getMaxRange() const { return _max_range; }

    [[nodiscard]] std::size_t getSize() const { return _size; }

protected:
    std::deque<double> _values;

    double _min_value = 0;
    double _max_value = 0;

    double _min_range = - 1.0;
    double _max_range = 1.0;

    std::size_t _size = 500;
};

#endif //DATA_SOURCE_HPP
