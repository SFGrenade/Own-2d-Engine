#include "engine/logiccontroller.h"

#include <thread>

#include "_globals/moreChrono.h"
#include "engine/loggerfactory.h"
#include "engine/performancecontroller.h"
#include "engine/scriptmanager.h"
#include "engine/sdlwindow.h"
#include "engine/sdlwindowrenderer.h"


SFG::Engine::LogicController::LogicController( SFG::Engine::SdlWindow* sdlWindow )
    : logger_( SFG::Engine::LoggerFactory::get_logger( "Engine_LogicController" ) ), sdlWindow_( sdlWindow ), done_( false ), doneMutex_() {
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

  this->doneMutex_.lock();
  while( !this->done_ ) {
    this->doneMutex_.unlock();

    secondsPerLoop = getDurationSinceLast( timePointStart );

    this->sdlWindow_->get_script_manager()->logic_update( secondsPerLoop );

    this->sdlWindow_->get_performance_controller()->incLogicLoops();

    this->doneMutex_.lock();
  }
  this->doneMutex_.unlock();

  this->logger_->trace( fmt::runtime( "run_loop()~" ) );
}
