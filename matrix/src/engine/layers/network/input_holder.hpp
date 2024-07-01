
#ifndef INPUT_HOLDER_H
#define INPUT_HOLDER_H

#include "network_object.hpp"

class InputHolder final : public NetworkObject {
public:

    explicit InputHolder(const double v) : state(v) {}
    ~InputHolder() override;

    void update(double) override;
    [[nodiscard]] double value() const override;

    void setValue(double);

private:

    double state = 0;
};


#endif //INPUT_HOLDER_H
