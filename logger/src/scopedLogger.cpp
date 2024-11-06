#include "SFG/Own2dEngine/Logger/scopedLogger.h"

namespace SFG {
namespace Own2dEngine {
namespace Logger {

ScopedLogger::ScopedLogger( spdlogger const& logger, std::string const& startMessage, std::string const& stopMessage )
    : logger_( logger ), startMessage_( startMessage ), stopMessage_( stopMessage ) {
  this->logger_->trace( fmt::runtime( this->startMessage_ ) );
}

ScopedLogger::~ScopedLogger() {
  this->logger_->trace( fmt::runtime( this->stopMessage_ ) );
}

}  // namespace Logger
}  // namespace Own2dEngine
}  // namespace SFG
