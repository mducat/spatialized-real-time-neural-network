
#ifndef INPUT_HOLDER_H
#define INPUT_HOLDER_H

#include "object.hpp"

class InputHolder final : public Object {
public:

    explicit InputHolder(const double v) : state(v) {}

    LayerType getDestinationLayer() override;

    void update() override;
    [[nodiscard]] double value() const override;

    void setValue(double);

private:

    double state = 0;
};


#endif //INPUT_HOLDER_H
