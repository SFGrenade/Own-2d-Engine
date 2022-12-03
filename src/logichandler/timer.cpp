#include "timer.h"

#include <iostream>

namespace SFG {
Timer::Timer(TimerCallback callback, std::chrono::nanoseconds interval,
             bool returnInterval)
    : timerCallback(callback),
      timerInterval(interval),
      timerCurrent(interval),
      timerReturnInterval(returnInterval) {
    spdlog::trace("Timer::Timer()");
    spdlog::trace("Timer::Timer()~");
}

Timer::~Timer() {
    spdlog::trace("Timer::~Timer()");
    spdlog::trace("Timer::~Timer()~");
}

void Timer::UpdateTimer(std::chrono::nanoseconds duration) {
    // spdlog::trace("Timer::UpdateTimer(std::chrono::nanoseconds duration =
    // {})", duration.count()); // todo too many log lines
    this->timerCurrent -= duration;
    if (this->timerCurrent <= 0ns) {
        if (timerReturnInterval) {
            this->timerCallback(
                std::chrono::duration_cast<std::chrono::secondsLongDouble>(
                    this->timerInterval - this->timerCurrent));
        } else {
            this->timerCallback({});
        }
        this->timerCurrent = this->timerInterval;
    }
    // spdlog::trace("Timer::UpdateTimer()~"); // todo too many log lines
}
}  // namespace SFG
