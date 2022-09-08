#pragma once

// Including our headers
#include "../globals.h"

#include <chrono>
#include <functional>
#include <vector>

namespace SFG {
    typedef std::function<void()> TimerCallback;

    class Timer
    {
    private:
        TimerCallback timerCallback;
        std::chrono::nanoseconds timerInterval;
        std::chrono::nanoseconds timerCurrent;
    public:
        Timer(TimerCallback callback, std::chrono::nanoseconds interval);
        ~Timer();

        void UpdateTimer(std::chrono::nanoseconds duration);
    };
}
