#include "scripthandler.h"
#include "script.h"
#include "CustomScripts/logscript.h"

#include <type_traits>

namespace SFG {
    std::vector<Script*> ScriptHandler::scripts = std::vector<Script*>();

    void ScriptHandler::Initialize() {
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
        for (Script* script : ScriptHandler::scripts) {
            script->End();
            delete script;
        }
    }

    template <class T>
    T* ScriptHandler::AddScript()
    {
        static_assert(std::is_base_of<Script, T>::value, "class need to be inherited from SFG::Script");
        spdlog::trace("ScriptHandler::AddScript()");
        Script* script = (Script*) new T();
        ScriptHandler::scripts.push_back(script);
        script->Start();
        spdlog::trace("ScriptHandler::AddScript()~");
        return (T*) script;
    }
}

template SFG::LogScript* SFG::ScriptHandler::AddScript<SFG::LogScript>();
