#include "SFG/Own2dEngine/Game/performance.h"

namespace SFG {
namespace Own2dEngine {
namespace Game {

SFG::Own2dEngine::Logger::spdlogger Performance::logger_ = nullptr;
std::mutex Performance::timingPointsMutex_;
std::map< std::string, Performance::PerformanceData > Performance::timingPoints_;

void Performance::init() {
  Performance::logger_ = SFG::Own2dEngine::Logger::LoggerFactory::get_logger( "Performance" );
  Performance::logger_->trace( "init()" );

  Performance::logger_->trace( "init()~" );
}

void Performance::startProgram() {
  Performance::startTiming( "Program" );
}

std::chrono::secondsLongDouble Performance::endProgram() {
  std::chrono::secondsLongDouble duration = Performance::endTiming( "Program" );
  {
    // std::scoped_lock _( Performance::timingPointsMutex_ );  // todo: fixme: mutex
    Performance::timingPoints_.clear();
  }
  return duration;
}

void Performance::startTiming( std::string const& category ) {
  bool hasCategory = false;
  {
    // std::scoped_lock _( Performance::timingPointsMutex_ );  // todo: fixme: mutex
    hasCategory = Performance::timingPoints_.contains( category );
  }
  if( hasCategory ) {
    // category already present
    // std::scoped_lock _( Performance::timingPointsMutex_ );  // todo: fixme: mutex
    Performance::PerformanceData& data = Performance::timingPoints_[category];
    if( !data.initialized ) {
      std::ofstream file( fmt::format( "{:s}.csv", category ), std::ios::trunc );
      fmt::print( file, "\"duration\";\"per second\"\n" );
      data.initialized = true;
    }
    data.timingPoint = std::chrono::high_resolution_clock::now();
  } else {
    // category not yet present
    Performance::PerformanceData data;

    std::ofstream file( fmt::format( "{:s}.csv", category ), std::ios::trunc );
    fmt::print( file, "\"duration\";\"per second\"\n" );
    data.initialized = true;
    data.timingPoint = std::chrono::high_resolution_clock::now();
    {
      // std::scoped_lock _( Performance::timingPointsMutex_ );  // todo: fixme: mutex
      Performance::timingPoints_.insert_or_assign( category, data );
    }
  }
}

std::chrono::secondsLongDouble Performance::endTiming( std::string const& category ) {
  std::chrono::secondsLongDouble duration( 0.0 );
  {
    // std::scoped_lock _( Performance::timingPointsMutex_ );  // todo: fixme: mutex
    Performance::PerformanceData& data = Performance::timingPoints_[category];
    duration = SFG::Own2dEngine::Utils::getDurationSinceLast( data.timingPoint );
  }

  long double durationVal = duration.count();
  long double perSecond = 1.0L / durationVal;

  std::ofstream file( fmt::format( "{:s}.csv", category ), std::ios::app );
  fmt::print( file, "\"{:.7Lf}\";\"{:.7Lf}\"\n", durationVal, perSecond );

  return duration;
}

}  // namespace Game
}  // namespace Own2dEngine
}  // namespace SFG
