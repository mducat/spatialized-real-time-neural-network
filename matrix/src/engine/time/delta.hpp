
#ifndef DELTA_H
#define DELTA_H

#include <chrono>

class Delta {
public:

    void start();
    void reset();
    [[nodiscard]] double delta();

    [[nodiscard]] double uptime() const;
    // doesn't step like delta()
    [[nodiscard]] double value() const;

private:

    std::chrono::time_point<std::chrono::steady_clock> _last;
    std::chrono::time_point<std::chrono::steady_clock> _first;

};

#endif //DELTA_H
