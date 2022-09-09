#pragma once

// Including our headers
#include "../globals.h"

#include <functional>
#include <optional>
#include <vector>

namespace SFG {
    typedef std::function<void(std::optional<std::chrono::secondsLongDouble> interval)> TimerCallback;

    class Timer
    {
    private:
        TimerCallback timerCallback;
        std::chrono::nanoseconds timerInterval;
        std::chrono::nanoseconds timerCurrent;
        bool timerReturnInterval;
    public:
        Timer(TimerCallback callback, std::chrono::nanoseconds interval, bool returnInterval);
        ~Timer();

        void UpdateTimer(std::chrono::nanoseconds duration);
    };
}
