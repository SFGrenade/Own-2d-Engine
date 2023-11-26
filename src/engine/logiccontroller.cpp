#include "engine/logiccontroller.h"

#include <thread>

#include "_globals/moreChrono.h"
#include "engine/performancecontroller.h"
#include "engine/sdlwindow.h"
#include "engine/sdlwindowrenderer.h"


SFG::Engine::LogicController::LogicController( SFG::Engine::SdlWindow* sdlWindow )
    : logger_( spdlog::get( "Engine_LogicController" ) ), sdlWindow_( sdlWindow ), done_( false ), doneMutex_() {
  this->logger_->trace( fmt::runtime( "LogicController( sdlWindow = {:p} )" ), static_cast< void* >( sdlWindow ) );

  this->logger_->trace( fmt::runtime( "LogicController()~" ) );
}

SFG::Engine::LogicController::~LogicController() {
  this->logger_->trace( fmt::runtime( "~LogicController()" ) );

  this->logger_->trace( fmt::runtime( "~LogicController()~" ) );
}

void SFG::Engine::LogicController::signal_quit() {
  this->logger_->trace( fmt::runtime( "signal_quit()" ) );

  this->doneMutex_.lock();
  this->done_ = true;
  this->doneMutex_.unlock();

  this->logger_->trace( fmt::runtime( "signal_quit()~" ) );
}

void SFG::Engine::LogicController::run_loop() {
  this->logger_->trace( fmt::runtime( "run_loop()" ) );

  std::chrono::high_resolution_clock::time_point timePointStart = std::chrono::high_resolution_clock::now();
  std::chrono::secondsLongDouble secondsPerLoop;
  std::chrono::secondsLongDouble countDown1s = std::chrono::duration_cast< std::chrono::secondsLongDouble >( 1.0s );

  this->doneMutex_.lock();
  while( !this->done_ ) {
    this->doneMutex_.unlock();

    secondsPerLoop = getDurationSinceLast( timePointStart );

    countDown1s -= secondsPerLoop;
    if( countDown1s.count() <= 0 ) {
      this->sdlWindow_->get_window_renderer()->set_debugInfo_topLeft(
          fmt::format( fmt::runtime( "{:.1F} fps" ), this->sdlWindow_->get_performance_controller()->getRenderLoops() ) );
      this->sdlWindow_->get_window_renderer()->set_debugInfo_topRight(
          fmt::format( fmt::runtime( "{:.1F} llps" ), this->sdlWindow_->get_performance_controller()->getLogicLoops() ) );
      this->sdlWindow_->get_window_renderer()->set_debugInfo_bottomLeft(
          fmt::format( fmt::runtime( "{:.1F} ilps" ), this->sdlWindow_->get_performance_controller()->getInputLoops() ) );
      this->sdlWindow_->get_window_renderer()->set_debugInfo_bottomRight(
          fmt::format( fmt::runtime( "{:.1F} nlps" ), this->sdlWindow_->get_performance_controller()->getNetworkLoops() ) );

      countDown1s = std::chrono::duration_cast< std::chrono::secondsLongDouble >( 1.0s );
    }

    this->sdlWindow_->get_performance_controller()->incLogicLoops();

    this->doneMutex_.lock();
  }
  this->doneMutex_.unlock();

  this->logger_->trace( fmt::runtime( "run_loop()~" ) );
}
