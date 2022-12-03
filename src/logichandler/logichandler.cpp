#include "logichandler.h"

#include "../performance/performance.h"

namespace SFG {
std::vector<Timer*> LogicHandler::timers = std::vector<Timer*>();
bool* LogicHandler::quitFlag = nullptr;
std::thread LogicHandler::logicThread;

void LogicHandler::Initialize() {
    spdlog::trace("LogicHandler::Initialize()");
    spdlog::trace("LogicHandler::Initialize()~");
}

void LogicHandler::Destroy() {
    spdlog::trace("LogicHandler::Destroy()");
    for (auto timer : LogicHandler::timers) {
        delete timer;
    }
    LogicHandler::timers.clear();
    spdlog::trace("LogicHandler::Destroy()~");
}

void LogicHandler::UpdateLogic() {
    spdlog::trace("LogicHandler::UpdateLogic()");
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
    spdlog::trace("LogicHandler::UpdateLogic()~");
}

void LogicHandler::SetQuitFlag(bool* quitFlag) {
    spdlog::trace("LogicHandler::SetQuitFlag(bool* quitFlag = {})",
                  (void*)quitFlag);
    if (quitFlag) LogicHandler::quitFlag = quitFlag;
    spdlog::trace("LogicHandler::SetQuitFlag()~");
}

void LogicHandler::StartLogic() {
    spdlog::trace("LogicHandler::StartLogic()");
    LogicHandler::logicThread = std::thread(UpdateLogic);
    spdlog::trace("LogicHandler::StartLogic()~");
}

void LogicHandler::StopLogic() {
    spdlog::trace("LogicHandler::StopLogic()");
    LogicHandler::logicThread.join();
    spdlog::trace("LogicHandler::StopLogic()~");
}

void LogicHandler::AddTimer(TimerCallback callback,
                            std::chrono::nanoseconds interval,
                            bool returnInterval) {
    spdlog::trace(
        "LogicHandler::AddTimer(TimerCallback callback, "
        "std::chrono::nanoseconds interval = {}, bool returnInterval = {})",
        interval.count(), returnInterval);
    if (callback) {
        Timer* timer = new Timer(callback, interval, returnInterval);
        LogicHandler::timers.push_back(timer);
    }
    spdlog::trace("LogicHandler::AddTimer()~");
}
}  // namespace SFG
