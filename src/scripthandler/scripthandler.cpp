#include "scripthandler.h"

#include "CustomScripts/logscript.h"
#include "script.h"

namespace SFG {
spdlogger ScriptHandler::logger = nullptr;
std::vector<Script*> ScriptHandler::scripts = std::vector<Script*>();

void ScriptHandler::Initialize() {
    ScriptHandler::logger = spdlog::get("ScriptHandler");
    ScriptHandler::logger->trace("Initialize()");
    ScriptHandler::logger->trace("Initialize()~");
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
    ScriptHandler::logger->trace("Destroy()");
    for (Script* script : ScriptHandler::scripts) {
        script->End();
        delete script;
    }
    ScriptHandler::logger->trace("Destroy()~");
}

template <class T>
T* ScriptHandler::AddScript() {
    static_assert(std::is_base_of<Script, T>::value, "Class need to be inherited from `SFG::Script`!");
    ScriptHandler::logger->trace("AddScript()");
    Script* script = (Script*)new T();
    ScriptHandler::scripts.push_back(script);
    script->Start();
    ScriptHandler::logger->trace("AddScript()~");
    return (T*)script;
}
}  // namespace SFG

template SFG::LogScript* SFG::ScriptHandler::AddScript<SFG::LogScript>();
