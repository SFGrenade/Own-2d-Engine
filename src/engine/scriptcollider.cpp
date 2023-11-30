#include "engine/scriptcollider.h"

#include "engine/sdlwindow.h"


SFG::Engine::ScriptCollider::ScriptCollider() : _base_(), isTrigger_( false ) {}

SFG::Engine::ScriptCollider::~ScriptCollider() {}

void SFG::Engine::ScriptCollider::start() {
  _base_::start();
}

void SFG::Engine::ScriptCollider::frame_update( SDL_Renderer* renderer ) {
  _base_::frame_update( renderer );
}

void SFG::Engine::ScriptCollider::input_update( SDL_Event const& input ) {
  _base_::input_update( input );
}

void SFG::Engine::ScriptCollider::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  _base_::logic_update( deltaTime );
}

void SFG::Engine::ScriptCollider::network_update() {
  _base_::network_update();
}

void SFG::Engine::ScriptCollider::end() {
  _base_::end();
}
