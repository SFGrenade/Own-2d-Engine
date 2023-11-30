#include "content/scripts/logscript.h"

#include "engine/sdlwindow.h"


SFG::Content::LogScript::LogScript() : _base_(), logger_( spdlog::get( "Content_LogScript" ) ) {
  this->logger_->trace( fmt::runtime( "LogScript()" ) );

  this->logger_->trace( fmt::runtime( "LogScript()~" ) );
}

SFG::Content::LogScript::~LogScript() {
  this->logger_->trace( fmt::runtime( "~LogScript()" ) );

  this->logger_->trace( fmt::runtime( "~LogScript()~" ) );
}

void SFG::Content::LogScript::start() {
  _base_::start();

  this->logger_->trace( fmt::runtime( "start()" ) );

  this->logger_->trace( fmt::runtime( "start()~" ) );
}

void SFG::Content::LogScript::frame_update( SDL_Renderer* renderer ) {
  _base_::frame_update( renderer );

  this->logger_->trace( fmt::runtime( "frame_update( renderer = {:p} )" ), static_cast< void const* >( renderer ) );

  this->logger_->trace( fmt::runtime( "frame_update()~" ) );
}

void SFG::Content::LogScript::input_update( SDL_Event const& input ) {
  _base_::input_update( input );

  this->logger_->trace( fmt::runtime( "input_update( input = {:s} )" ), SDL_EventType_to_string( static_cast< SDL_EventType >( input.type ) ) );

  this->logger_->trace( fmt::runtime( "input_update()~" ) );
}

void SFG::Content::LogScript::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  _base_::logic_update( deltaTime );

  this->logger_->trace( fmt::runtime( "logic_update( deltaTime = {:.8F} )" ), deltaTime.count() );

  this->logger_->trace( fmt::runtime( "logic_update()~" ) );
}

void SFG::Content::LogScript::network_update() {
  _base_::network_update();

  this->logger_->trace( fmt::runtime( "network_update()" ) );

  this->logger_->trace( fmt::runtime( "network_update()~" ) );
}

void SFG::Content::LogScript::end() {
  _base_::end();

  this->logger_->trace( fmt::runtime( "end()" ) );

  this->logger_->trace( fmt::runtime( "end()~" ) );
}
