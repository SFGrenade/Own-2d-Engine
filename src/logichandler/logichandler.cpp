#include "logichandler/logichandler.h"

#include "performance/performance.h"

namespace SFG {
spdlogger LogicHandler::logger = nullptr;
std::vector< std::shared_ptr< Timer > > LogicHandler::timers = std::vector< std::shared_ptr< Timer > >();
std::shared_ptr< bool > LogicHandler::quitFlag = nullptr;
std::thread LogicHandler::logicThread;

void LogicHandler::deleteTimer( Timer* ptr ) {
  LogicHandler::logger->trace( "deleteTimer(Timer* ptr = {:p})", static_cast< void* >( ptr ) );
  delete ptr;
}

void LogicHandler::UpdateLogic() {
  LogicHandler::logger->trace( "UpdateLogic()" );
  std::chrono::time_point< std::chrono::high_resolution_clock > old = std::chrono::high_resolution_clock::now();
  std::chrono::time_point< std::chrono::high_resolution_clock > now;
  std::chrono::nanoseconds duration;
  while( !( *LogicHandler::quitFlag ) ) {
    now = std::chrono::high_resolution_clock::now();
    duration = now - old;
    for( auto timer : LogicHandler::timers ) {
      timer->UpdateTimer( duration );
    }
    Performance::AddLogicLoop();
    old = now;
  }
  LogicHandler::logger->trace( "UpdateLogic()~" );
}

void LogicHandler::Initialize() {
  LogicHandler::logger = spdlog::get( "LogicHandler" );
  LogicHandler::logger->trace( "Initialize()" );
  LogicHandler::logger->trace( "Initialize()~" );
}

void LogicHandler::Destroy() {
  LogicHandler::logger->trace( "Destroy()" );
  LogicHandler::timers.clear();
  LogicHandler::logger->trace( "Destroy()~" );
}

void LogicHandler::SetQuitFlag( std::shared_ptr< bool > newQuitFlag ) {
  LogicHandler::logger->trace( "SetQuitFlag(std::shared_ptr<bool> quitFlag = {:p})", static_cast< void* >( newQuitFlag.get() ) );
  if( newQuitFlag )
    LogicHandler::quitFlag = newQuitFlag;
  LogicHandler::logger->trace( "SetQuitFlag()~" );
}

void LogicHandler::StartLogic() {
  LogicHandler::logger->trace( "StartLogic()" );
  LogicHandler::logicThread = std::thread( UpdateLogic );
  LogicHandler::logger->trace( "StartLogic()~" );
}

void LogicHandler::StopLogic() {
  LogicHandler::logger->trace( "StopLogic()" );
  LogicHandler::logicThread.join();
  LogicHandler::logger->trace( "StopLogic()~" );
}

void LogicHandler::AddTimer( TimerCallback callback, std::chrono::nanoseconds interval, bool returnInterval ) {
  LogicHandler::logger->trace( "AddTimer(TimerCallback callback, std::chrono::nanoseconds interval = {:d}, bool returnInterval = {})",
                               interval.count(),
                               returnInterval );
  if( callback ) {
    std::shared_ptr< Timer > timer = std::shared_ptr< Timer >( new Timer( callback, interval, returnInterval ), LogicHandler::deleteTimer );
    LogicHandler::timers.push_back( timer );
  }
  LogicHandler::logger->trace( "AddTimer()~" );
}
}  // namespace SFG
