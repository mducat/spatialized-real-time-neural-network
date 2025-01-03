
#ifndef INPUT_HOLDER_H
#define INPUT_HOLDER_H

#include <vector>

#include "network_object.hpp"

class InputHolder final : public NetworkObject {
public:

    explicit InputHolder(std::vector<double>);
    explicit InputHolder(double v);
    ~InputHolder() override;

    void update(double) override;
    [[nodiscard]] double value() const override;

    void setValue(double);
    void setValues(std::vector<double>);

    void setTimeStep(double);

private:

    std::size_t _cursor = 0;
    double _counter = 0;
    double _time_step = 1.0;
    double _state = 0;
    std::vector<double> _values;
};


#endif //INPUT_HOLDER_H
