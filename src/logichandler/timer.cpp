#include "timer.h"

#include <iostream>

namespace SFG {
Timer::Timer(TimerCallback callback, std::chrono::nanoseconds interval,
             bool returnInterval)
    : logger(spdlog::get("Timer")),
      timerCallback(callback),
      timerInterval(interval),
      timerCurrent(interval),
      timerReturnInterval(returnInterval) {
    logger->trace("Timer::Timer()");
    logger->trace("Timer::Timer()~");
}

Timer::~Timer() {
    logger->trace("Timer::~Timer()");
    logger->trace("Timer::~Timer()~");
}

void Timer::UpdateTimer(std::chrono::nanoseconds duration) {
    // logger->trace("Timer::UpdateTimer(std::chrono::nanoseconds duration =
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
    // logger->trace("Timer::UpdateTimer()~"); // todo too many log lines
}
}  // namespace SFG
