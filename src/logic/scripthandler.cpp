#include "logic/scripthandler.h"

#include "logic/CustomScripts/logscript.h"
#include "logic/script.h"

namespace SFG {
void ScriptHandler::deleteScript( Script* ptr ) {
  this->logger_->trace( fmt::runtime( "deleteScript( ptr = {} )" ), static_cast< void* >( ptr ) );

  ptr->End();
  delete ptr;

  this->logger_->trace( fmt::runtime( "deleteScript()~" ) );
}

ScriptHandler::ScriptHandler() : logger_( spdlog::get( "ScriptHandler" ) ), scripts_() {
  this->logger_->trace( fmt::runtime( "ScriptHandler()" ) );

  this->logger_->trace( fmt::runtime( "ScriptHandler()~" ) );
}

ScriptHandler::~ScriptHandler() {
  this->logger_->trace( fmt::runtime( "~ScriptHandler()" ) );

  this->scripts_.clear();

  this->logger_->trace( fmt::runtime( "~ScriptHandler()~" ) );
}

void ScriptHandler::UpdateScriptsFrame() {
  // this->logger_->trace( fmt::runtime( "UpdateScriptsFrame()" ) );

  for( std::shared_ptr< Script > script : this->scripts_ ) {
    script->UpdateFrame();
  }

  // this->logger_->trace( fmt::runtime( "UpdateScriptsFrame()~" ) );
}

void ScriptHandler::UpdateScriptsLogicFrame() {
  // this->logger_->trace( fmt::runtime( "UpdateScriptsLogicFrame()" ) );

  for( std::shared_ptr< Script > script : this->scripts_ ) {
    script->UpdateLogicFrame();
  }

  // this->logger_->trace( fmt::runtime( "UpdateScriptsLogicFrame()~" ) );
}

template < class T >
std::shared_ptr< T > ScriptHandler::AddScript() {
  static_assert( std::is_base_of< Script, T >::value, "Class need to be inherited from `SFG::Script`!" );
  this->logger_->trace( fmt::runtime( "AddScript()" ) );

  std::shared_ptr< T > script = std::shared_ptr< T >( new T(), [this]( Script* ptr ) { this->deleteScript( ptr ); } );
  this->scripts_.push_back( script );
  script->Start();

  this->logger_->trace( fmt::runtime( "AddScript()~" ) );
  return script;
}
}  // namespace SFG

template std::shared_ptr< SFG::LogScript > SFG::ScriptHandler::AddScript< SFG::LogScript >();
