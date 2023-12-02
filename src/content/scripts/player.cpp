#include "content/scripts/player.h"

#include "engine/sdlwindow.h"


SFG::Content::Player::Player()
    : _base_(),
      logger_( spdlog::get( "Content_Player" ) ),
      rendering_( true ),
      pressesUp_( false ),
      pressesDown_( false ),
      pressesLeft_( false ),
      pressesRight_( false ),
      playerRect_() {}

SFG::Content::Player::~Player() {}

void SFG::Content::Player::start() {
  _base_::start();
}

void SFG::Content::Player::frame_update( SDL_Renderer* renderer ) {
  _base_::frame_update( renderer );

  if( !this->rendering_ ) {
    return;
  }

  uint8_t origR, origG, origB, origA;
  SDL_GetRenderDrawColor( renderer, &origR, &origG, &origB, &origA );
  SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
  SDL_RenderFillRect( renderer, &playerRect_ );
  SDL_SetRenderDrawColor( renderer, origR, origG, origB, origA );
}

void SFG::Content::Player::input_update( SDL_Event const& input ) {
  _base_::input_update( input );

  if( input.type == SDL_EventType::SDL_KEYDOWN ) {
    if( ( input.key.repeat == 0 ) && ( input.key.keysym.sym == SDL_KeyCode::SDLK_UP || input.key.keysym.sym == SDL_KeyCode::SDLK_w ) ) {
      this->pressesUp_ = true;
    } else if( ( input.key.repeat == 0 ) && ( input.key.keysym.sym == SDL_KeyCode::SDLK_DOWN || input.key.keysym.sym == SDL_KeyCode::SDLK_s ) ) {
      this->pressesDown_ = true;
    } else if( ( input.key.repeat == 0 ) && ( input.key.keysym.sym == SDL_KeyCode::SDLK_LEFT || input.key.keysym.sym == SDL_KeyCode::SDLK_a ) ) {
      this->pressesLeft_ = true;
    } else if( ( input.key.repeat == 0 ) && ( input.key.keysym.sym == SDL_KeyCode::SDLK_RIGHT || input.key.keysym.sym == SDL_KeyCode::SDLK_d ) ) {
      this->pressesRight_ = true;
    }
  } else if( input.type == SDL_EventType::SDL_KEYUP ) {
    if( ( input.key.repeat == 0 ) && ( input.key.keysym.sym == SDL_KeyCode::SDLK_UP || input.key.keysym.sym == SDL_KeyCode::SDLK_w ) ) {
      this->pressesUp_ = false;
    } else if( ( input.key.repeat == 0 ) && ( input.key.keysym.sym == SDL_KeyCode::SDLK_DOWN || input.key.keysym.sym == SDL_KeyCode::SDLK_s ) ) {
      this->pressesDown_ = false;
    } else if( ( input.key.repeat == 0 ) && ( input.key.keysym.sym == SDL_KeyCode::SDLK_LEFT || input.key.keysym.sym == SDL_KeyCode::SDLK_a ) ) {
      this->pressesLeft_ = false;
    } else if( ( input.key.repeat == 0 ) && ( input.key.keysym.sym == SDL_KeyCode::SDLK_RIGHT || input.key.keysym.sym == SDL_KeyCode::SDLK_d ) ) {
      this->pressesRight_ = false;
    }
  }
}

void SFG::Content::Player::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  if( this->pressesUp_ ) {
    this->velocity_.y = -500.0L;
  } else if( this->pressesDown_ ) {
    this->velocity_.y = +500.0L;
  } else {
    this->velocity_.y = 0.0L;
  }
  if( this->pressesLeft_ ) {
    this->velocity_.x = -500.0L;
  } else if( this->pressesRight_ ) {
    this->velocity_.x = +500.0L;
  } else {
    this->velocity_.x = 0.0L;
  }

  _base_::logic_update( deltaTime );

  this->playerRect_.x = static_cast< int >( this->position_.x );
  this->playerRect_.y = static_cast< int >( this->position_.y );
  this->playerRect_.w = static_cast< int >( this->size_.x );
  this->playerRect_.h = static_cast< int >( this->size_.y );
}

void SFG::Content::Player::network_update() {
  _base_::network_update();
}

void SFG::Content::Player::end() {
  _base_::end();
}
