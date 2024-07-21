
#include "delta.hpp"

void Delta::start() {
    this->_last = std::chrono::steady_clock::now();
}

double Delta::delta() {
    auto const now = std::chrono::steady_clock::now();
    std::chrono::duration<double> const duration{now - this->_last};

    _last = now;
    return duration.count();
}

double Delta::value() const {
    auto const now = std::chrono::steady_clock::now();
    std::chrono::duration<double> const duration{now - this->_last};

    return duration.count();
}

