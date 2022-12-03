#include "logichandler.h"

#include "../performance/performance.h"

namespace SFG {
spdlogger LogicHandler::logger = nullptr;
std::vector<Timer*> LogicHandler::timers = std::vector<Timer*>();
bool* LogicHandler::quitFlag = nullptr;
std::thread LogicHandler::logicThread;

void LogicHandler::Initialize() {
    LogicHandler::logger = spdlog::get("LogicHandler");
    LogicHandler::logger->trace("LogicHandler::Initialize()");
    LogicHandler::logger->trace("LogicHandler::Initialize()~");
}

void LogicHandler::Destroy() {
    LogicHandler::logger->trace("LogicHandler::Destroy()");
    for (auto timer : LogicHandler::timers) {
        delete timer;
    }
    LogicHandler::timers.clear();
    LogicHandler::logger->trace("LogicHandler::Destroy()~");
}

void LogicHandler::UpdateLogic() {
    LogicHandler::logger->trace("LogicHandler::UpdateLogic()");
    std::chrono::time_point<std::chrono::high_resolution_clock> old =
        std::chrono::high_resolution_clock::now();
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
    LogicHandler::logger->trace("LogicHandler::UpdateLogic()~");
}

void LogicHandler::SetQuitFlag(bool* newQuitFlag) {
    LogicHandler::logger->trace(
        "LogicHandler::SetQuitFlag(bool* quitFlag = {})", (void*)newQuitFlag);
    if (newQuitFlag) LogicHandler::quitFlag = newQuitFlag;
    LogicHandler::logger->trace("LogicHandler::SetQuitFlag()~");
}

void LogicHandler::StartLogic() {
    LogicHandler::logger->trace("LogicHandler::StartLogic()");
    LogicHandler::logicThread = std::thread(UpdateLogic);
    LogicHandler::logger->trace("LogicHandler::StartLogic()~");
}

void LogicHandler::StopLogic() {
    LogicHandler::logger->trace("LogicHandler::StopLogic()");
    LogicHandler::logicThread.join();
    LogicHandler::logger->trace("LogicHandler::StopLogic()~");
}

void LogicHandler::AddTimer(TimerCallback callback,
                            std::chrono::nanoseconds interval,
                            bool returnInterval) {
    LogicHandler::logger->trace(
        "LogicHandler::AddTimer(TimerCallback callback, "
        "std::chrono::nanoseconds interval = {}, bool returnInterval = {})",
        interval.count(), returnInterval);
    if (callback) {
        Timer* timer = new Timer(callback, interval, returnInterval);
        LogicHandler::timers.push_back(timer);
    }
    LogicHandler::logger->trace("LogicHandler::AddTimer()~");
}
}  // namespace SFG
