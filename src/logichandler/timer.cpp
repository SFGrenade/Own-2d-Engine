#include "timer.h"
#include <iostream>

using namespace std::chrono_literals;

namespace SFG {
    Timer::Timer(TimerCallback callback, std::chrono::nanoseconds interval, bool returnInterval)
        : timerCallback(callback)
        , timerInterval(interval)
        , timerCurrent(interval)
        , timerReturnInterval(returnInterval) {
    }

    Timer::~Timer() {
    }

    void Timer::UpdateTimer(std::chrono::nanoseconds duration) {
        this->timerCurrent -= duration;
        if (this->timerCurrent <= 0ns) {
            if (timerReturnInterval) {
                this->timerCallback(std::chrono::duration_cast<std::chrono::secondsLongDouble>(this->timerInterval - this->timerCurrent));
            }
            else {
                this->timerCallback({});
            }
            this->timerCurrent = this->timerInterval;
        }
    }
}
