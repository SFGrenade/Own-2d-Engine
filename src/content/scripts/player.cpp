#include "content/scripts/player.h"

#include "engine/loggerfactory.h"
#include "engine/sdlwindow.h"


SFG::Content::Player::Player()
    : _base_(),
      logger_( SFG::Engine::LoggerFactory::get_logger( "Content_Player" ) ),
      rendering_( true ),
      pressesUp_( false ),
      pressesDown_( false ),
      pressesLeft_( false ),
      pressesRight_( false ),
      playerRect_(),
      playerTextureTouching_( nullptr ),
      playerTextureColliding_( nullptr ),
      playerTextureFree_( nullptr ) {}

SFG::Content::Player::~Player() {
  if( this->playerTextureTouching_ ) {
    this->logger_->trace( fmt::runtime( "~Player - destroying playerTextureTouching" ) );
    SDL_DestroyTexture( this->playerTextureTouching_ );
    this->playerTextureTouching_ = nullptr;
  }
  if( this->playerTextureColliding_ ) {
    this->logger_->trace( fmt::runtime( "~Player - destroying playerTextureColliding" ) );
    SDL_DestroyTexture( this->playerTextureColliding_ );
    this->playerTextureColliding_ = nullptr;
  }
  if( this->playerTextureFree_ ) {
    this->logger_->trace( fmt::runtime( "~Player - destroying playerTextureFree" ) );
    SDL_DestroyTexture( this->playerTextureFree_ );
    this->playerTextureFree_ = nullptr;
  }
}

void SFG::Content::Player::frame_update( SDL_Renderer* renderer ) {
  _base_::frame_update( renderer );

  if( !this->rendering_ ) {
    return;
  }
  if( this->playerTextureTouching_ == nullptr ) {
    SDL_Surface* txtsfc = SDL_CreateRGBSurfaceWithFormat( 0, 1, 1, 32, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888 );
    if( txtsfc == nullptr ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateRGBSurfaceWithFormat: {:s}" ), TTF_GetError() );
    } else {
      static_cast< uint32_t* >( txtsfc->pixels )[0] = 0xFF007FFF;
      this->playerTextureTouching_ = SDL_CreateTextureFromSurface( renderer, txtsfc );
      if( !this->playerTextureTouching_ ) {
        this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateTextureFromSurface: {:s}" ), SDL_GetError() );
      }
      SDL_FreeSurface( txtsfc );
    }
  }
  if( this->playerTextureColliding_ == nullptr ) {
    SDL_Surface* txtsfc = SDL_CreateRGBSurfaceWithFormat( 0, 1, 1, 32, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888 );
    if( txtsfc == nullptr ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateRGBSurfaceWithFormat: {:s}" ), TTF_GetError() );
    } else {
      static_cast< uint32_t* >( txtsfc->pixels )[0] = 0xFF00FFFF;
      this->playerTextureColliding_ = SDL_CreateTextureFromSurface( renderer, txtsfc );
      if( !this->playerTextureColliding_ ) {
        this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateTextureFromSurface: {:s}" ), SDL_GetError() );
      }
      SDL_FreeSurface( txtsfc );
    }
  }
  if( this->playerTextureFree_ == nullptr ) {
    SDL_Surface* txtsfc = SDL_CreateRGBSurfaceWithFormat( 0, 1, 1, 32, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888 );
    if( txtsfc == nullptr ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateRGBSurfaceWithFormat: {:s}" ), TTF_GetError() );
    } else {
      static_cast< uint32_t* >( txtsfc->pixels )[0] = 0xFF0000FF;
      this->playerTextureFree_ = SDL_CreateTextureFromSurface( renderer, txtsfc );
      if( !this->playerTextureFree_ ) {
        this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateTextureFromSurface: {:s}" ), SDL_GetError() );
      }
      SDL_FreeSurface( txtsfc );
    }
  }

  if( this->isTouching_ ) {
    if( SDL_RenderCopy( renderer, this->playerTextureTouching_, NULL, &( this->playerRect_ ) ) != 0 ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_RenderCopy: {:s}" ), SDL_GetError() );
    }
  } else if( this->isColliding_ ) {
    if( SDL_RenderCopy( renderer, this->playerTextureColliding_, NULL, &( this->playerRect_ ) ) != 0 ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_RenderCopy: {:s}" ), SDL_GetError() );
    }
  } else {
    if( SDL_RenderCopy( renderer, this->playerTextureFree_, NULL, &( this->playerRect_ ) ) != 0 ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_RenderCopy: {:s}" ), SDL_GetError() );
    }
  }
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
