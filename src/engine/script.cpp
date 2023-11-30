#include "engine/script.h"

#include "engine/sdlwindow.h"


SFG::Engine::Script::Script() : sdlWindow_( nullptr ) {}

SFG::Engine::Script::~Script() {}

void SFG::Engine::Script::start() {}

void SFG::Engine::Script::frame_update( SDL_Renderer* ) {}

void SFG::Engine::Script::input_update( SDL_Event const& ) {}

void SFG::Engine::Script::logic_update( std::chrono::secondsLongDouble const& ) {}

void SFG::Engine::Script::network_update() {}

void SFG::Engine::Script::end() {}

void SFG::Engine::Script::set_sdlWindow( SFG::Engine::SdlWindow* sdlWindow ) {
  this->sdlWindow_ = sdlWindow;
}
