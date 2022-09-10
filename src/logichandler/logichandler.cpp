#include "logichandler.h"

#include "../performance/performance.h"

namespace SFG {
    LogicHandler* LogicHandler::instance = nullptr;

    LogicHandler::LogicHandler()
        : timers()
        , quitFlag(nullptr) {
    }

    LogicHandler::~LogicHandler() {
        for (auto timer : timers) {
            delete timer;
        }
        timers.clear();
    }

    LogicHandler* LogicHandler::GetInstance() {
        if (!LogicHandler::instance) LogicHandler::instance = new LogicHandler();
        return LogicHandler::instance;
    }

    void LogicHandler::UpdateLogic() {
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
    }

    void LogicHandler::SetQuitFlag(bool* quitFlag) {
        if (quitFlag == nullptr) return;
        this->quitFlag = quitFlag;
    }

    void LogicHandler::StartLogic() {
        logicThread = std::thread(UpdateLogic);
    }

    void LogicHandler::StopLogic() {
        logicThread.join();
    }

    void LogicHandler::AddTimer(TimerCallback callback, std::chrono::nanoseconds interval, bool returnInterval) {
        Timer* timer = new Timer(callback, interval, returnInterval);
        timers.push_back(timer);
    }
}
