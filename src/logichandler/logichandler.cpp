#include "logichandler.h"

#include "../performance/performance.h"

using namespace std::chrono_literals;

namespace SFG {
    LogicHandler* LogicHandler::instance = nullptr;

    LogicHandler::LogicHandler(/* args */)
        : timers()
        , quitFlag(nullptr) {
    }

    LogicHandler::~LogicHandler() {
    }

    LogicHandler* LogicHandler::GetInstance() {
        if (!LogicHandler::instance) LogicHandler::instance = new LogicHandler();
        return LogicHandler::instance;
    }

    void LogicHandler::UpdateLogic() {
        while (!(*LogicHandler::instance->quitFlag)) {
            for (auto timer : LogicHandler::instance->timers) {
                timer.UpdateTimer();
            }
            Performance::AddLogicLoop();
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

    //void LogicHandler::RegisterWindowEvent(WindowEventCallback callback) {
    //}
}
