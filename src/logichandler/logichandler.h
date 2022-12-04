#pragma once

// Including our headers
#include "../globals.h"
#include "timer.h"

namespace SFG {
class LogicHandler {
   private:
    static spdlogger logger;
    static std::vector<Timer*> timers;
    static bool* quitFlag;
    static std::thread logicThread;
    static void UpdateLogic();

   public:
    static void Initialize();
    static void Destroy();

    static void SetQuitFlag(bool* quitFlag);
    static void StartLogic();
    static void StopLogic();
    static void AddTimer(TimerCallback callback, std::chrono::nanoseconds interval, bool returnInterval);
};
}  // namespace SFG
