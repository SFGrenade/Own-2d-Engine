#pragma once

#include <vector>

// Including our headers
#include "_globals/spdlogInclude.h"
#include "scripthandler/script.h"

namespace SFG {
class ScriptHandler {
  private:
  static spdlogger logger;
  static std::vector< std::shared_ptr< Script > > scripts;
  static void deleteScript( Script* script );

  public:
  static void Initialize();
  static void UpdateScriptsFrame();
  static void UpdateScriptsLogicFrame();
  static void Destroy();
  template < class T >
  static std::shared_ptr< T > AddScript();
};
}  // namespace SFG
