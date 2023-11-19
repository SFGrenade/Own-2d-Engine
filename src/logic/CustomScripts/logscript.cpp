#include "logic/CustomScripts/logscript.h"

namespace SFG {
LogScript::LogScript() : logger( spdlog::get( "LogScript" ) ) {
  logger->trace( fmt::runtime( "LogScript()" ) );

  logger->trace( fmt::runtime( "LogScript()~" ) );
}

void LogScript::Start() {
  logger->trace( fmt::runtime( "Start()" ) );

  logger->trace( fmt::runtime( "Start()~" ) );
}

void LogScript::UpdateFrame() {
  logger->trace( fmt::runtime( "UpdateFrame()" ) );

  logger->trace( fmt::runtime( "UpdateFrame()~" ) );
}

void LogScript::UpdateLogicFrame() {
  logger->trace( fmt::runtime( "UpdateLogicFrame()" ) );

  logger->trace( fmt::runtime( "UpdateLogicFrame()~" ) );
}

void LogScript::End() {
  logger->trace( fmt::runtime( "End()" ) );

  logger->trace( fmt::runtime( "End()~" ) );
}
}  // namespace SFG
