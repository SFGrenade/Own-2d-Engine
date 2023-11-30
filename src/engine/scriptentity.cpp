#include "engine/scriptentity.h"

#include "engine/script.h"
#include "engine/sdlwindow.h"


SFG::Engine::ScriptEntity::ScriptEntity()
    : _base_(), position_( SFG::Engine::Vector2( 0.0L, 0.0L ) ), size_( SFG::Engine::Vector2( 0.0L, 0.0L ) ), velocity_( SFG::Engine::Vector2( 0.0L, 0.0L ) ) {}

SFG::Engine::ScriptEntity::~ScriptEntity() {}

void SFG::Engine::ScriptEntity::start() {
  _base_::start();
}

void SFG::Engine::ScriptEntity::frame_update( SDL_Renderer* renderer ) {
  _base_::frame_update( renderer );
}

void SFG::Engine::ScriptEntity::input_update( SDL_Event const& input ) {
  _base_::input_update( input );
}

void SFG::Engine::ScriptEntity::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  _base_::logic_update( deltaTime );

  this->position_.x += this->velocity_.x * deltaTime.count();
  this->position_.y += this->velocity_.y * deltaTime.count();
}

void SFG::Engine::ScriptEntity::network_update() {
  _base_::network_update();
}

void SFG::Engine::ScriptEntity::end() {
  _base_::end();
}
