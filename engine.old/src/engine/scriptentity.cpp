#include "engine/scriptentity.h"

#include "engine/script.h"
#include "engine/sdlwindow.h"


SFG::Engine::ScriptEntity::ScriptEntity()
    : _base_(), position_( SFG::Engine::Vector2( 0.0L, 0.0L ) ), size_( SFG::Engine::Vector2( 0.0L, 0.0L ) ), velocity_( SFG::Engine::Vector2( 0.0L, 0.0L ) ) {}

SFG::Engine::ScriptEntity::~ScriptEntity() {}

void SFG::Engine::ScriptEntity::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  _base_::logic_update( deltaTime );

  this->position_ += this->velocity_ * deltaTime.count();
}

void SFG::Engine::ScriptEntity::set_position( long double x, long double y ) {
  this->position_.set_x( x );
  this->position_.set_y( y );
}

void SFG::Engine::ScriptEntity::set_size( long double w, long double h ) {
  this->size_.set_x( w );
  this->size_.set_y( h );
}

void SFG::Engine::ScriptEntity::set_velocity( long double x, long double y ) {
  this->velocity_.set_x( x );
  this->velocity_.set_y( y );
}

long double SFG::Engine::ScriptEntity::get_top() {
  return this->position_.y();
}

long double SFG::Engine::ScriptEntity::get_bottom() {
  return this->position_.y() + this->size_.y();
}

long double SFG::Engine::ScriptEntity::get_left() {
  return this->position_.x();
}

long double SFG::Engine::ScriptEntity::get_right() {
  return this->position_.x() + this->size_.x();
}

SFG::Engine::Vector2 SFG::Engine::ScriptEntity::get_middle() {
  return SFG::Engine::Vector2( this->position_ + ( this->size_ / 2.0L ) );
}

long double SFG::Engine::ScriptEntity::get_moved_top( std::chrono::secondsLongDouble const& deltaTime ) {
  return this->position_.y() + ( deltaTime.count() * this->velocity_.y() );
}

long double SFG::Engine::ScriptEntity::get_moved_bottom( std::chrono::secondsLongDouble const& deltaTime ) {
  return this->position_.y() + this->size_.y() + ( deltaTime.count() * this->velocity_.y() );
}

long double SFG::Engine::ScriptEntity::get_moved_left( std::chrono::secondsLongDouble const& deltaTime ) {
  return this->position_.x() + ( deltaTime.count() * this->velocity_.x() );
}

long double SFG::Engine::ScriptEntity::get_moved_right( std::chrono::secondsLongDouble const& deltaTime ) {
  return this->position_.x() + this->size_.x() + ( deltaTime.count() * this->velocity_.x() );
}

SFG::Engine::Vector2 SFG::Engine::ScriptEntity::get_moved_middle( std::chrono::secondsLongDouble const& deltaTime ) {
  return SFG::Engine::Vector2( this->position_ + ( this->size_ / 2.0L ) + ( this->velocity_ * deltaTime.count() ) );
}
