#pragma once

// Including our headers
#include "../globals.h"

namespace SFG {
class Script;
class ScriptHandler {
   private:
    static spdlogger logger;
    static std::vector<std::shared_ptr<Script>> scripts;
    static void deleteScript(Script* script);

   public:
    static void Initialize();
    static void UpdateScriptsFrame();
    static void UpdateScriptsLogicFrame();
    static void Destroy();
    template <class T>
    static std::shared_ptr<T> AddScript();
};
}  // namespace SFG
