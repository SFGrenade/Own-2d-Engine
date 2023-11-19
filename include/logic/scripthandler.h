#pragma once

#include <vector>

// Including our headers
#include "_globals/spdlogInclude.h"
#include "logic/script.h"

namespace SFG {
class ScriptHandler {
  private:
  void deleteScript( Script* script );

  public:
  ScriptHandler();
  ~ScriptHandler();

  void UpdateScriptsFrame();
  void UpdateScriptsLogicFrame();
  template < class T >
  std::shared_ptr< T > AddScript();

  private:
  spdlogger logger_;
  std::vector< std::shared_ptr< Script > > scripts_;
};
}  // namespace SFG
