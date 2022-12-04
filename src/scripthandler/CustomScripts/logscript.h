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
        logger->trace("LogScript()");
        logger->trace("LogScript()~");
    }
    virtual void Start() {
        logger->trace("Start()");
        logger->trace("Start()~");
    }
    virtual void UpdateFrame() {
        logger->trace("UpdateFrame()");
        logger->trace("UpdateFrame()~");
    }
    virtual void UpdateLogicFrame() {
        logger->trace("UpdateLogicFrame()");
        logger->trace("UpdateLogicFrame()~");
    }
    virtual void End() {
        logger->trace("End()");
        logger->trace("End()~");
    }
};
}  // namespace SFG
