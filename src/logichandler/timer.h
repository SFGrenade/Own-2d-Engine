#pragma once

// Including our headers
#include "../globals.h"

#include <chrono>
#include <functional>
#include <vector>

namespace SFG {
    //typedef std::function<void(SDL_WindowEvent& window)> WindowEventCallback;

    class Timer
    {
    private:
        //std::vector<WindowEventCallback> windowEventCallbacks;
    public:
        Timer(/* args */);
        ~Timer();

        void UpdateTimer();
    };
}
