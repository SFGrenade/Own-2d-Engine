#pragma once

// Including our headers
#include "../../globals.h"
#include "../script.h"

namespace SFG {
class LogScript : public Script {
   private:
    spdlogger logger;

   public:
    LogScript() {
        logger = spdlog::get("LogScript");
        logger->trace("LogScript::LogScript()");
        logger->trace("LogScript::LogScript()~");
    }
    virtual void Start() {
        logger->trace("LogScript::Start()");
        logger->trace("LogScript::Start()~");
    }
    virtual void UpdateFrame() {
        logger->trace("LogScript::UpdateFrame()");
        logger->trace("LogScript::UpdateFrame()~");
    }
    virtual void UpdateLogicFrame() {
        logger->trace("LogScript::UpdateLogicFrame()");
        logger->trace("LogScript::UpdateLogicFrame()~");
    }
    virtual void End() {
        logger->trace("LogScript::End()");
        logger->trace("LogScript::End()~");
    }
};
}  // namespace SFG
