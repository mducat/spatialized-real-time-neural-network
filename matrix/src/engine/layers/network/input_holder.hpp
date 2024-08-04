
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

    std::size_t cursor = 0;
    double counter = 0;
    double timeStep = 1.0;
    double state = 0;
    std::vector<double> values;
};


#endif //INPUT_HOLDER_H
