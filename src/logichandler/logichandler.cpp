#include "logichandler.h"

#include "../performance/performance.h"

namespace SFG {
spdlogger LogicHandler::logger = nullptr;
std::vector<Timer*> LogicHandler::timers = std::vector<Timer*>();
bool* LogicHandler::quitFlag = nullptr;
std::thread LogicHandler::logicThread;

void LogicHandler::Initialize() {
    LogicHandler::logger = spdlog::get("LogicHandler");
    LogicHandler::logger->trace("Initialize()");
    LogicHandler::logger->trace("Initialize()~");
}

void LogicHandler::Destroy() {
    LogicHandler::logger->trace("Destroy()");
    for (auto timer : LogicHandler::timers) {
        delete timer;
    }
    LogicHandler::timers.clear();
    LogicHandler::logger->trace("Destroy()~");
}

void LogicHandler::UpdateLogic() {
    LogicHandler::logger->trace("UpdateLogic()");
    std::chrono::time_point<std::chrono::high_resolution_clock> old = std::chrono::high_resolution_clock::now();
    std::chrono::time_point<std::chrono::high_resolution_clock> now;
    std::chrono::nanoseconds duration;
    while (!(*LogicHandler::quitFlag)) {
        now = std::chrono::high_resolution_clock::now();
        duration = now - old;
        for (auto timer : LogicHandler::timers) {
            timer->UpdateTimer(duration);
        }
        Performance::AddLogicLoop();
        old = now;
    }
    LogicHandler::logger->trace("UpdateLogic()~");
}

void LogicHandler::SetQuitFlag(bool* newQuitFlag) {
    LogicHandler::logger->trace("SetQuitFlag(bool* quitFlag = {:p})", static_cast<void*>(newQuitFlag));
    if (newQuitFlag) LogicHandler::quitFlag = newQuitFlag;
    LogicHandler::logger->trace("SetQuitFlag()~");
}

void LogicHandler::StartLogic() {
    LogicHandler::logger->trace("StartLogic()");
    LogicHandler::logicThread = std::thread(UpdateLogic);
    LogicHandler::logger->trace("StartLogic()~");
}

void LogicHandler::StopLogic() {
    LogicHandler::logger->trace("StopLogic()");
    LogicHandler::logicThread.join();
    LogicHandler::logger->trace("StopLogic()~");
}

void LogicHandler::AddTimer(TimerCallback callback, std::chrono::nanoseconds interval, bool returnInterval) {
    LogicHandler::logger->trace("AddTimer(TimerCallback callback, std::chrono::nanoseconds interval = {:d}, bool returnInterval = {})", interval.count(),
                                returnInterval);
    if (callback) {
        Timer* timer = new Timer(callback, interval, returnInterval);
        LogicHandler::timers.push_back(timer);
    }
    LogicHandler::logger->trace("AddTimer()~");
}
}  // namespace SFG
