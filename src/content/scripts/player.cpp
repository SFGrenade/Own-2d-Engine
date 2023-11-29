#include "content/scripts/player.h"

#include "engine/sdlwindow.h"


SFG::Content::Player::Player( SFG::Engine::SdlWindow* sdlWindow )
    : SFG::Engine::Script( sdlWindow ),
      logger_( spdlog::get( "Content_Player" ) ),
      rendering_( true ),
      positionX_( 0 ),
      positionY_( 0 ),
      speed_( 500.0L ),
      playerRect_( SDL_Rect( 0, 0, 50, 50 ) ),
      inputUp_( false ),
      inputDown_( false ),
      inputLeft_( false ),
      inputRight_( false ) {}

SFG::Content::Player::~Player() {}

void SFG::Content::Player::start() {
  SFG::Engine::Script::start();
}

void SFG::Content::Player::frame_update( SDL_Renderer* renderer ) {
  SFG::Engine::Script::frame_update( renderer );

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
  SFG::Engine::Script::input_update( input );

  if( input.type == SDL_EventType::SDL_KEYDOWN ) {
    if( ( input.key.repeat == 0 ) && ( input.key.keysym.sym == SDL_KeyCode::SDLK_UP || input.key.keysym.sym == SDL_KeyCode::SDLK_w ) ) {
      this->inputUp_ = true;
    } else if( ( input.key.repeat == 0 ) && ( input.key.keysym.sym == SDL_KeyCode::SDLK_DOWN || input.key.keysym.sym == SDL_KeyCode::SDLK_s ) ) {
      this->inputDown_ = true;
    } else if( ( input.key.repeat == 0 ) && ( input.key.keysym.sym == SDL_KeyCode::SDLK_LEFT || input.key.keysym.sym == SDL_KeyCode::SDLK_a ) ) {
      this->inputLeft_ = true;
    } else if( ( input.key.repeat == 0 ) && ( input.key.keysym.sym == SDL_KeyCode::SDLK_RIGHT || input.key.keysym.sym == SDL_KeyCode::SDLK_d ) ) {
      this->inputRight_ = true;
    }
  } else if( input.type == SDL_EventType::SDL_KEYUP ) {
    if( ( input.key.repeat == 0 ) && ( input.key.keysym.sym == SDL_KeyCode::SDLK_UP || input.key.keysym.sym == SDL_KeyCode::SDLK_w ) ) {
      this->inputUp_ = false;
    } else if( ( input.key.repeat == 0 ) && ( input.key.keysym.sym == SDL_KeyCode::SDLK_DOWN || input.key.keysym.sym == SDL_KeyCode::SDLK_s ) ) {
      this->inputDown_ = false;
    } else if( ( input.key.repeat == 0 ) && ( input.key.keysym.sym == SDL_KeyCode::SDLK_LEFT || input.key.keysym.sym == SDL_KeyCode::SDLK_a ) ) {
      this->inputLeft_ = false;
    } else if( ( input.key.repeat == 0 ) && ( input.key.keysym.sym == SDL_KeyCode::SDLK_RIGHT || input.key.keysym.sym == SDL_KeyCode::SDLK_d ) ) {
      this->inputRight_ = false;
    }
  }
}

void SFG::Content::Player::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  SFG::Engine::Script::logic_update( deltaTime );

  if( this->inputUp_ ) {
    this->positionY_ -= this->speed_ * deltaTime.count();
  }
  if( this->inputDown_ ) {
    this->positionY_ += this->speed_ * deltaTime.count();
  }
  if( this->inputLeft_ ) {
    this->positionX_ -= this->speed_ * deltaTime.count();
  }
  if( this->inputRight_ ) {
    this->positionX_ += this->speed_ * deltaTime.count();
  }
  this->playerRect_.x = this->positionX_;
  this->playerRect_.y = this->positionY_;
}

void SFG::Content::Player::fixed_update() {
  SFG::Engine::Script::fixed_update();
}

void SFG::Content::Player::network_update() {
  SFG::Engine::Script::network_update();
}

void SFG::Content::Player::end() {
  SFG::Engine::Script::end();
}
