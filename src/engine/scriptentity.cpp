#include "engine/scriptentity.h"

#include "engine/script.h"
#include "engine/sdlwindow.h"


SFG::Engine::ScriptEntity::ScriptEntity()
    : _base_(), position_( SFG::Engine::Vector2( 0.0L, 0.0L ) ), size_( SFG::Engine::Vector2( 0.0L, 0.0L ) ), velocity_( SFG::Engine::Vector2( 0.0L, 0.0L ) ) {}

SFG::Engine::ScriptEntity::~ScriptEntity() {}

void SFG::Engine::ScriptEntity::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  _base_::logic_update( deltaTime );

  this->position_.x += this->velocity_.x * deltaTime.count();
  this->position_.y += this->velocity_.y * deltaTime.count();
}

void SFG::Engine::ScriptEntity::set_position( long double x, long double y ) {
  this->position_.x = x;
  this->position_.y = y;
}

void SFG::Engine::ScriptEntity::set_size( long double w, long double h ) {
  this->size_.x = w;
  this->size_.y = h;
}

void SFG::Engine::ScriptEntity::set_velocity( long double x, long double y ) {
  this->velocity_.x = x;
  this->velocity_.y = y;
}
