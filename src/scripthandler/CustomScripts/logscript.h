#pragma once

// Including our headers
#include "../../globals.h"

#include "../script.h"

namespace SFG {
    class LogScript : public Script
    {
    protected:
        // some things
    public:
        LogScript() {
            spdlog::trace("LogScript::LogScript()");
            spdlog::trace("LogScript::LogScript()~");
        }
        virtual void Start() {
            spdlog::trace("LogScript::Start()");
            spdlog::trace("LogScript::Start()~");
        }
        virtual void UpdateFrame() {
            spdlog::trace("LogScript::UpdateFrame()");
            spdlog::trace("LogScript::UpdateFrame()~");
        }
        virtual void UpdateLogicFrame() {
            spdlog::trace("LogScript::UpdateLogicFrame()");
            spdlog::trace("LogScript::UpdateLogicFrame()~");
        }
        virtual void End() {
            spdlog::trace("LogScript::End()");
            spdlog::trace("LogScript::End()~");
        }
    };
}
