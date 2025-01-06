//
// Created by thornhill on 06/01/25.
//

#pragma once

#include <losses.hpp>
#include <network_object.hpp>


template<typename T, std::enable_if_t<std::is_base_of_v<LossFunction, T>>* = nullptr>
class Loss final : public NetworkObject {
public:

    template <typename... Args>
    explicit Loss(Args ...nodes) {
        for (const auto &node :
            std::initializer_list<std::shared_ptr<NetworkObject>>({nodes...})) {
            this->addInput(node);
        }
    }

    ~Loss() override = default;

    void update(double delta) override {
        if (this->_inputs.size() < 2) {
            throw std::invalid_argument("loss update: less than 2 inputs");
        }

        const auto target = this->getNetworkObjectAt(0);

        for (int i = 1; i < this->_inputs.size(); ++i) {
            const auto input = this->getNetworkObjectAt(i);

            const auto loss = T::compute(input->value(), target->value());

            input->backwards(loss);
            target->backwards(loss);

            this->_state = loss;
        }
    };
    [[nodiscard]] double value() const override { return _state; };


private:

    double _state = 0;
};

