#pragma once

// Including our headers
#include "../globals.h"

#include "timer.h"

#include <functional>
#include <thread>
#include <vector>

namespace SFG {
    class LogicHandler
    {
    private:
        static LogicHandler* instance;
        std::vector<Timer*> timers;
        bool* quitFlag;
        std::thread logicThread;
        LogicHandler(/* args */);
        static void UpdateLogic();
    public:
        ~LogicHandler();
        static LogicHandler* GetInstance();

        void SetQuitFlag(bool* quitFlag);
        void StartLogic();
        void StopLogic();
        void AddTimer(TimerCallback callback, std::chrono::nanoseconds interval, bool returnInterval);
        //void RegisterWindowEvent(WindowEventCallback callback);
    };
}
