#include "SFG/Own2dEngine/Engine/performance.h"

namespace SFG {
namespace Own2dEngine {
namespace Engine {

SFG::Own2dEngine::Logger::spdlogger Performance::logger_ = nullptr;
std::map< std::string, std::chrono::high_resolution_clock::time_point > Performance::timingPoints_;

void Performance::startProgram() {
  if( Performance::logger_ == nullptr ) {
    Performance::logger_ = SFG::Own2dEngine::Logger::LoggerFactory::get_logger( "Performance" );
  }

  Performance::startTiming( "Program" );
}

std::chrono::secondsLongDouble Performance::endProgram() {
  if( Performance::logger_ == nullptr ) {
    Performance::logger_ = SFG::Own2dEngine::Logger::LoggerFactory::get_logger( "Performance" );
  }

  std::chrono::secondsLongDouble duration = Performance::endTiming( "Program" );
  Performance::timingPoints_.clear();
  return duration;
}

void Performance::startTiming( std::string const& category ) {
  if( Performance::logger_ == nullptr ) {
    Performance::logger_ = SFG::Own2dEngine::Logger::LoggerFactory::get_logger( "Performance" );
  }

  Performance::timingPoints_.insert_or_assign( category, std::chrono::high_resolution_clock::now() );
}

std::chrono::secondsLongDouble Performance::endTiming( std::string const& category ) {
  if( Performance::logger_ == nullptr ) {
    Performance::logger_ = SFG::Own2dEngine::Logger::LoggerFactory::get_logger( "Performance" );
  }

  std::chrono::secondsLongDouble duration = SFG::Own2dEngine::Utils::getDurationSinceLast( Performance::timingPoints_[category] );
  Performance::logger_->trace( "{:s} took {:.7Lf} seconds", category, duration.count() );
  return duration;
}

}  // namespace Engine
}  // namespace Own2dEngine
}  // namespace SFG
