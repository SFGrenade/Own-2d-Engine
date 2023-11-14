#include "logic/scripthandler.h"

#include "logic/CustomScripts/logscript.h"
#include "logic/script.h"

namespace SFG {
spdlogger ScriptHandler::logger = nullptr;
std::vector< std::shared_ptr< Script > > ScriptHandler::scripts = std::vector< std::shared_ptr< Script > >();

void ScriptHandler::deleteScript( Script* script ) {
  script->End();
  delete script;
}

void ScriptHandler::Initialize() {
  ScriptHandler::logger = spdlog::get( "ScriptHandler" );
  ScriptHandler::logger->trace( fmt::runtime( "Initialize()" ) );
  ScriptHandler::logger->trace( fmt::runtime( "Initialize()~" ) );
}

void ScriptHandler::UpdateScriptsFrame() {
  for( std::shared_ptr< Script > script : ScriptHandler::scripts ) {
    script->UpdateFrame();
  }
}

void ScriptHandler::UpdateScriptsLogicFrame() {
  for( std::shared_ptr< Script > script : ScriptHandler::scripts ) {
    script->UpdateLogicFrame();
  }
}

void ScriptHandler::Destroy() {
  ScriptHandler::logger->trace( fmt::runtime( "Destroy()" ) );
  ScriptHandler::scripts.clear();
  ScriptHandler::logger->trace( fmt::runtime( "Destroy()~" ) );
}

template < class T >
std::shared_ptr< T > ScriptHandler::AddScript() {
  static_assert( std::is_base_of< Script, T >::value, "Class need to be inherited from `SFG::Script`!" );
  ScriptHandler::logger->trace( fmt::runtime( "AddScript()" ) );
  std::shared_ptr< T > script = std::shared_ptr< T >( new T(), deleteScript );
  ScriptHandler::scripts.push_back( script );
  script->Start();
  ScriptHandler::logger->trace( fmt::runtime( "AddScript()~" ) );
  return script;
}
}  // namespace SFG

template std::shared_ptr< SFG::LogScript > SFG::ScriptHandler::AddScript< SFG::LogScript >();
