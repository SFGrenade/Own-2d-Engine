#include "engine/performancecontroller.h"

#include "engine/loggerfactory.h"


SFG::Engine::PerformanceController::PerformanceController( SFG::Engine::SdlWindow* sdlWindow )
    : logger_( SFG::Engine::LoggerFactory::get_logger( "Engine_PerformanceController" ) ), sdlWindow_( sdlWindow ) {
  this->logger_->trace( fmt::runtime( "PerformanceController( sdlWindow = {:p} )" ), static_cast< void* >( sdlWindow ) );

  this->logger_->trace( fmt::runtime( "PerformanceController()~" ) );
}

SFG::Engine::PerformanceController::~PerformanceController() {
  this->logger_->trace( fmt::runtime( "~PerformanceController()" ) );

  this->logger_->trace( fmt::runtime( "~PerformanceController()~" ) );
}

void SFG::Engine::PerformanceController::incInputLoops() {
  // this->logger_->trace( fmt::runtime( "incInputLoops()" ) );

  ++this->counterInputLoops_;

  // this->logger_->trace( fmt::runtime( "incInputLoops()~" ) );
}

long double SFG::Engine::PerformanceController::getInputLoops() {
  // this->logger_->trace( fmt::runtime( "getInputLoops()" ) );

  std::chrono::secondsLongDouble durationSinceLast = getDurationSinceLast( this->counterInputLoopsTimePoint_ );
  long double ret = this->counterInputLoops_ / durationSinceLast.count();
  this->counterInputLoops_ = 0;

  // this->logger_->trace( fmt::runtime( "getInputLoops()~" ) );
  return ret;
}

void SFG::Engine::PerformanceController::incLogicLoops() {
  // this->logger_->trace( fmt::runtime( "incLogicLoops()" ) );

  ++this->counterLogicLoops_;

  // this->logger_->trace( fmt::runtime( "incLogicLoops()~" ) );
}

long double SFG::Engine::PerformanceController::getLogicLoops() {
  // this->logger_->trace( fmt::runtime( "getLogicLoops()" ) );

  std::chrono::secondsLongDouble durationSinceLast = getDurationSinceLast( this->counterLogicLoopsTimePoint_ );
  long double ret = this->counterLogicLoops_ / durationSinceLast.count();
  this->counterLogicLoops_ = 0;

  // this->logger_->trace( fmt::runtime( "getLogicLoops()~" ) );
  return ret;
}

void SFG::Engine::PerformanceController::incNetworkLoops() {
  // this->logger_->trace( fmt::runtime( "incNetworkLoops()" ) );

  ++this->counterNetworkLoops_;

  // this->logger_->trace( fmt::runtime( "incNetworkLoops()~" ) );
}

long double SFG::Engine::PerformanceController::getNetworkLoops() {
  // this->logger_->trace( fmt::runtime( "getNetworkLoops()" ) );

  std::chrono::secondsLongDouble durationSinceLast = getDurationSinceLast( this->counterNetworkLoopsTimePoint_ );
  long double ret = this->counterNetworkLoops_ / durationSinceLast.count();
  this->counterNetworkLoops_ = 0;

  // this->logger_->trace( fmt::runtime( "getNetworkLoops()~" ) );
  return ret;
}

void SFG::Engine::PerformanceController::incRenderLoops() {
  // this->logger_->trace( fmt::runtime( "incRenderLoops()" ) );

  ++this->counterRenderLoops_;

  // this->logger_->trace( fmt::runtime( "incRenderLoops()~" ) );
}

long double SFG::Engine::PerformanceController::getRenderLoops() {
  // this->logger_->trace( fmt::runtime( "getRenderLoops()" ) );

  std::chrono::secondsLongDouble durationSinceLast = getDurationSinceLast( this->counterRenderLoopsTimePoint_ );
  long double ret = this->counterRenderLoops_ / durationSinceLast.count();
  this->counterRenderLoops_ = 0;

  // this->logger_->trace( fmt::runtime( "getRenderLoops()~" ) );
  return ret;
}
