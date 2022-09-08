#include "timer.h"
#include <iostream>

using namespace std::chrono_literals;

namespace SFG {
    Timer::Timer(TimerCallback callback, std::chrono::nanoseconds interval)
        : timerCallback(callback)
        , timerInterval(interval)
        , timerCurrent(interval) {
    }

    Timer::~Timer() {
    }

    void Timer::UpdateTimer(std::chrono::nanoseconds duration) {
        this->timerCurrent = this->timerCurrent - duration;
        if (this->timerCurrent <= 0ns) {
            this->timerCallback();
            this->timerCurrent = this->timerInterval;
        }
    }
}
