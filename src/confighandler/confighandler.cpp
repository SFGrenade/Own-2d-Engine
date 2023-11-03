#include "confighandler/confighandler.h"

namespace SFG {
spdlogger ConfigHandler::logger = nullptr;

void ConfigHandler::Initialize() {
  ConfigHandler::logger = spdlog::get( "ConfigHandler" );
  ConfigHandler::logger->trace( "Initialize()" );

  ConfigHandler::logger->trace( "Initialize()~" );
}

void ConfigHandler::Destroy() {
  ConfigHandler::logger->trace( "Destroy()" );

  ConfigHandler::logger->trace( "Destroy()~" );
}
}  // namespace SFG
