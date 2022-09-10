#include "logichandler.h"

#include "../performance/performance.h"

namespace SFG {
    LogicHandler* LogicHandler::instance = nullptr;

    LogicHandler::LogicHandler()
        : timers()
        , quitFlag(nullptr) {
        spdlog::trace("LogicHandler::LogicHandler()");
        spdlog::trace("LogicHandler::LogicHandler()~");
    }

    LogicHandler::~LogicHandler() {
        spdlog::trace("LogicHandler::~LogicHandler()");
        for (auto timer : timers) {
            delete timer;
        }
        timers.clear();
        spdlog::trace("LogicHandler::~LogicHandler()~");
    }

    LogicHandler* LogicHandler::GetInstance() {
        spdlog::trace("LogicHandler::GetInstance()");
        if (!LogicHandler::instance) LogicHandler::instance = new LogicHandler();
        spdlog::trace("LogicHandler::GetInstance()~");
        return LogicHandler::instance;
    }

    void LogicHandler::UpdateLogic() {
        spdlog::trace("LogicHandler::UpdateLogic()");
        std::chrono::time_point<std::chrono::high_resolution_clock> old = std::chrono::high_resolution_clock::now();
        std::chrono::time_point<std::chrono::high_resolution_clock> now;
        std::chrono::nanoseconds duration;
        while (!(*LogicHandler::instance->quitFlag)) {
            now = std::chrono::high_resolution_clock::now();
            duration = now - old;
            for (auto timer : LogicHandler::instance->timers) {
                timer->UpdateTimer(duration);
            }
            Performance::AddLogicLoop();
            old = now;
        }
        spdlog::trace("LogicHandler::UpdateLogic()~");
    }

    void LogicHandler::SetQuitFlag(bool* quitFlag) {
        spdlog::trace("LogicHandler::SetQuitFlag(bool* quitFlag = {})", (void*) quitFlag);
        if (quitFlag) this->quitFlag = quitFlag;
        spdlog::trace("LogicHandler::SetQuitFlag()~");
    }

    void LogicHandler::StartLogic() {
        spdlog::trace("LogicHandler::StartLogic()");
        logicThread = std::thread(UpdateLogic);
        spdlog::trace("LogicHandler::StartLogic()~");
    }

    void LogicHandler::StopLogic() {
        spdlog::trace("LogicHandler::StopLogic()");
        logicThread.join();
        spdlog::trace("LogicHandler::StopLogic()~");
    }

    void LogicHandler::AddTimer(TimerCallback callback, std::chrono::nanoseconds interval, bool returnInterval) {
        spdlog::trace("LogicHandler::AddTimer(TimerCallback callback, std::chrono::nanoseconds interval = {}, bool returnInterval = {})", interval.count(), returnInterval);
        if (callback) {
            Timer* timer = new Timer(callback, interval, returnInterval);
            timers.push_back(timer);
        }
        spdlog::trace("LogicHandler::AddTimer()~");
    }
}
