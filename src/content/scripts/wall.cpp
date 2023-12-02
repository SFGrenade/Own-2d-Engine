#include "content/scripts/wall.h"

#include "engine/sdlwindow.h"


SFG::Content::Wall::Wall() : _base_(), logger_( spdlog::get( "Content_Wall" ) ), rendering_( true ), wallRect_() {}

SFG::Content::Wall::~Wall() {}

void SFG::Content::Wall::start() {
  _base_::start();
}

void SFG::Content::Wall::frame_update( SDL_Renderer* renderer ) {
  _base_::frame_update( renderer );

  if( !this->rendering_ ) {
    return;
  }

  uint8_t origR, origG, origB, origA;
  SDL_GetRenderDrawColor( renderer, &origR, &origG, &origB, &origA );
  SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255 );
  SDL_RenderFillRect( renderer, &wallRect_ );
  SDL_SetRenderDrawColor( renderer, origR, origG, origB, origA );
}

void SFG::Content::Wall::input_update( SDL_Event const& input ) {
  _base_::input_update( input );
}

void SFG::Content::Wall::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  _base_::logic_update( deltaTime );

  this->wallRect_.x = static_cast< int >( this->position_.x );
  this->wallRect_.y = static_cast< int >( this->position_.y );
  this->wallRect_.w = static_cast< int >( this->size_.x );
  this->wallRect_.h = static_cast< int >( this->size_.y );
}

void SFG::Content::Wall::network_update() {
  _base_::network_update();
}

void SFG::Content::Wall::end() {
  _base_::end();
}
