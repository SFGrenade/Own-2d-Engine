#include "logic/timer.h"

namespace SFG {
Timer::Timer( TimerCallback callback, std::chrono::nanoseconds interval, bool returnInterval )
    : logger( spdlog::get( "Timer" ) ), timerCallback( callback ), timerInterval( interval ), timerCurrent( interval ), timerReturnInterval( returnInterval ) {
  logger->trace( fmt::runtime( "Timer()" ) );
  logger->trace( fmt::runtime( "Timer()~" ) );
}

Timer::~Timer() {
  logger->trace( fmt::runtime( "~Timer()" ) );
  logger->trace( fmt::runtime( "~Timer()~" ) );
}

void Timer::UpdateTimer( std::chrono::nanoseconds duration ) {
  // todo too many log lines
  // logger->trace( fmt::runtime( "UpdateTimer(std::chrono::nanoseconds duration = {:d})" ), duration.count() );
  this->timerCurrent -= duration;
  if( this->timerCurrent <= 0ns ) {
    if( timerReturnInterval ) {
      this->timerCallback( std::chrono::duration_cast< std::chrono::secondsLongDouble >( this->timerInterval - this->timerCurrent ) );
    } else {
      this->timerCallback( {} );
    }
    this->timerCurrent = this->timerInterval;
  }
  // todo too many log lines
  // logger->trace( fmt::runtime( "UpdateTimer()~" ));
}
}  // namespace SFG
