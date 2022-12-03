#pragma once

// Including our headers
#include "../globals.h"

namespace SFG {
class Script;
class ScriptHandler {
   private:
    static spdlogger logger;
    static std::vector<Script*> scripts;

   public:
    static void Initialize();
    static void UpdateScriptsFrame();
    static void UpdateScriptsLogicFrame();
    static void Destroy();
    template <class T>
    static T* AddScript();
};
}  // namespace SFG
