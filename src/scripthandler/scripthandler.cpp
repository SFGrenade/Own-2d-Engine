#include "scripthandler.h"

#include <type_traits>

#include "CustomScripts/logscript.h"
#include "script.h"

namespace SFG {
spdlogger ScriptHandler::logger = nullptr;
std::vector<Script*> ScriptHandler::scripts = std::vector<Script*>();

void ScriptHandler::Initialize() {
    ScriptHandler::logger = spdlog::get("ScriptHandler");
    ScriptHandler::logger->trace("ScriptHandler::Initialize()");
    ScriptHandler::logger->trace("ScriptHandler::Initialize()~");
}

void ScriptHandler::UpdateScriptsFrame() {
    for (Script* script : ScriptHandler::scripts) {
        script->UpdateFrame();
    }
}

void ScriptHandler::UpdateScriptsLogicFrame() {
    for (Script* script : ScriptHandler::scripts) {
        script->UpdateLogicFrame();
    }
}

void ScriptHandler::Destroy() {
    ScriptHandler::logger->trace("ScriptHandler::Destroy()");
    for (Script* script : ScriptHandler::scripts) {
        script->End();
        delete script;
    }
    ScriptHandler::logger->trace("ScriptHandler::Destroy()~");
}

template <class T>
T* ScriptHandler::AddScript() {
    static_assert(std::is_base_of<Script, T>::value, "class need to be inherited from SFG::Script");
    ScriptHandler::logger->trace("ScriptHandler::AddScript()");
    Script* script = (Script*)new T();
    ScriptHandler::scripts.push_back(script);
    script->Start();
    ScriptHandler::logger->trace("ScriptHandler::AddScript()~");
    return (T*)script;
}
}  // namespace SFG

template SFG::LogScript* SFG::ScriptHandler::AddScript<SFG::LogScript>();
