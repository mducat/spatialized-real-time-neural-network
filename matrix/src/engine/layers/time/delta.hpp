
#ifndef DELTA_H
#define DELTA_H

#include <chrono>

class Delta {
public:

    void start();
    [[nodiscard]] double delta();

    // doesn't reset
    [[nodiscard]] double value() const;

private:

    std::chrono::time_point<std::chrono::steady_clock> _last;

};

#endif //DELTA_H
