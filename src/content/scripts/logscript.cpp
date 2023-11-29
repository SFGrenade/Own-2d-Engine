#include "content/scripts/logscript.h"

#include "engine/sdlwindow.h"


SFG::Content::LogScript::LogScript( SFG::Engine::SdlWindow* sdlWindow ) : SFG::Engine::Script( sdlWindow ), logger_( spdlog::get( "Content_LogScript" ) ) {
  this->logger_->trace( fmt::runtime( "LogScript( sdlWindow = {:p} )" ), static_cast< void* >( sdlWindow ) );

  this->logger_->trace( fmt::runtime( "LogScript()~" ) );
}

SFG::Content::LogScript::~LogScript() {
  this->logger_->trace( fmt::runtime( "~LogScript()" ) );

  this->logger_->trace( fmt::runtime( "~LogScript()~" ) );
}

void SFG::Content::LogScript::start() {
  SFG::Engine::Script::start();

  this->logger_->trace( fmt::runtime( "start()" ) );

  this->logger_->trace( fmt::runtime( "start()~" ) );
}

void SFG::Content::LogScript::frame_update( SDL_Renderer* renderer ) {
  SFG::Engine::Script::frame_update( renderer );

  this->logger_->trace( fmt::runtime( "frame_update( renderer = {:p} )" ), static_cast< void const* >( renderer ) );

  this->logger_->trace( fmt::runtime( "frame_update()~" ) );
}

void SFG::Content::LogScript::input_update( SDL_Event const& input ) {
  SFG::Engine::Script::input_update( input );

  this->logger_->trace( fmt::runtime( "input_update( input = {:s} )" ), SDL_EventType_to_string( static_cast< SDL_EventType >( input.type ) ) );

  this->logger_->trace( fmt::runtime( "input_update()~" ) );
}

void SFG::Content::LogScript::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  SFG::Engine::Script::logic_update( deltaTime );

  this->logger_->trace( fmt::runtime( "logic_update( deltaTime = {:.8F} )" ), deltaTime.count() );

  this->logger_->trace( fmt::runtime( "logic_update()~" ) );
}

void SFG::Content::LogScript::fixed_update() {
  SFG::Engine::Script::fixed_update();

  this->logger_->trace( fmt::runtime( "fixed_update()" ) );

  this->logger_->trace( fmt::runtime( "fixed_update()~" ) );
}

void SFG::Content::LogScript::network_update() {
  SFG::Engine::Script::network_update();

  this->logger_->trace( fmt::runtime( "network_update()" ) );

  this->logger_->trace( fmt::runtime( "network_update()~" ) );
}

void SFG::Content::LogScript::end() {
  SFG::Engine::Script::end();

  this->logger_->trace( fmt::runtime( "end()" ) );

  this->logger_->trace( fmt::runtime( "end()~" ) );
}
