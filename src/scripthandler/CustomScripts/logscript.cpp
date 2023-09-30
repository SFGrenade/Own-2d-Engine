#include "scripthandler/CustomScripts/logscript.h"

namespace SFG {
LogScript::LogScript() : logger( spdlog::get( "LogScript" ) ) {
  logger->trace( "LogScript()" );
  logger->trace( "LogScript()~" );
}

void LogScript::Start() {
  logger->trace( "Start()" );
  logger->trace( "Start()~" );
}

void LogScript::UpdateFrame() {
  logger->trace( "UpdateFrame()" );
  logger->trace( "UpdateFrame()~" );
}

void LogScript::UpdateLogicFrame() {
  logger->trace( "UpdateLogicFrame()" );
  logger->trace( "UpdateLogicFrame()~" );
}

void LogScript::End() {
  logger->trace( "End()" );
  logger->trace( "End()~" );
}
}  // namespace SFG
