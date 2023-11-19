#include "logic/logichandler.h"

namespace SFG {
void LogicHandler::deleteTimer( Timer* ptr ) {
  this->logger_->trace( fmt::runtime( "deleteTimer( ptr = {:p} )" ), static_cast< void* >( ptr ) );

  delete ptr;

  this->logger_->trace( fmt::runtime( "deleteTimer()~" ) );
}

LogicHandler::LogicHandler() : logger_( spdlog::get( "LogicHandler" ) ), old_(), timers_() {
  this->logger_->trace( fmt::runtime( "LogicHandler()" ) );

  this->logger_->trace( fmt::runtime( "LogicHandler()~" ) );
}

LogicHandler::~LogicHandler() {
  this->logger_->trace( fmt::runtime( "~LogicHandler()" ) );

  this->timers_.clear();

  this->logger_->trace( fmt::runtime( "~LogicHandler()~" ) );
}

void LogicHandler::StartLogic() {
  this->logger_->trace( fmt::runtime( "StartLogic()" ) );

  this->logger_->trace( fmt::runtime( "StartLogic()~" ) );
}

void LogicHandler::UpdateLogic() {
  // this->logger_->trace( fmt::runtime( "UpdateLogic()" ) );

  std::chrono::time_point< std::chrono::high_resolution_clock > now = std::chrono::high_resolution_clock::now();
  std::chrono::nanoseconds duration = now - this->old_;
  for( auto timer : this->timers_ ) {
    timer->UpdateTimer( duration );
  }
  this->old_ = now;

  // this->logger_->trace( fmt::runtime( "UpdateLogic()~" ) );
}

void LogicHandler::AddTimer( TimerCallback callback, std::chrono::nanoseconds interval, bool returnInterval ) {
  this->logger_->trace( fmt::runtime( "AddTimer( callback, interval = {:d}, returnInterval = {} )" ), interval.count(), returnInterval );

  if( callback ) {
    std::shared_ptr< Timer > timer
        = std::shared_ptr< Timer >( new Timer( callback, interval, returnInterval ), [this]( Timer* ptr ) { this->deleteTimer( ptr ); } );
    this->timers_.push_back( timer );
  }

  this->logger_->trace( fmt::runtime( "AddTimer()~" ) );
}
}  // namespace SFG
