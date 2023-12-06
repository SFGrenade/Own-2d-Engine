#include "content/scripts/bouncer.h"

#include "engine/loggerfactory.h"
#include "engine/sdlwindow.h"


SFG::Content::Bouncer::Bouncer()
    : _base_(),
      logger_( SFG::Engine::LoggerFactory::get_logger( "Content_Bouncer" ) ),
      rendering_( true ),
      bouncerRect_(),
      bouncerTextureTouching_( nullptr ),
      bouncerTextureColliding_( nullptr ),
      bouncerTextureFree_( nullptr ),
      moving_( SFG::Engine::Vector2( 0.0L, 0.0L ) ) {}

SFG::Content::Bouncer::~Bouncer() {
  if( this->bouncerTextureTouching_ ) {
    this->logger_->trace( fmt::runtime( "~Bouncer - destroying bouncerTextureTouching" ) );
    SDL_DestroyTexture( this->bouncerTextureTouching_ );
    this->bouncerTextureTouching_ = nullptr;
  }
  if( this->bouncerTextureColliding_ ) {
    this->logger_->trace( fmt::runtime( "~Bouncer - destroying bouncerTextureColliding" ) );
    SDL_DestroyTexture( this->bouncerTextureColliding_ );
    this->bouncerTextureColliding_ = nullptr;
  }
  if( this->bouncerTextureFree_ ) {
    this->logger_->trace( fmt::runtime( "~Bouncer - destroying bouncerTextureFree" ) );
    SDL_DestroyTexture( this->bouncerTextureFree_ );
    this->bouncerTextureFree_ = nullptr;
  }
}

void SFG::Content::Bouncer::frame_update( SDL_Renderer* renderer ) {
  _base_::frame_update( renderer );

  if( !this->rendering_ ) {
    return;
  }
  if( this->bouncerTextureTouching_ == nullptr ) {
    SDL_Surface* txtsfc = SDL_CreateRGBSurfaceWithFormat( 0, 1, 1, 32, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888 );
    if( txtsfc == nullptr ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateRGBSurfaceWithFormat: {:s}" ), TTF_GetError() );
    } else {
      static_cast< uint32_t* >( txtsfc->pixels )[0] = 0xFFFF7FFF;
      this->bouncerTextureTouching_ = SDL_CreateTextureFromSurface( renderer, txtsfc );
      if( !this->bouncerTextureTouching_ ) {
        this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateTextureFromSurface: {:s}" ), SDL_GetError() );
      }
      SDL_FreeSurface( txtsfc );
    }
  }
  if( this->bouncerTextureColliding_ == nullptr ) {
    SDL_Surface* txtsfc = SDL_CreateRGBSurfaceWithFormat( 0, 1, 1, 32, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888 );
    if( txtsfc == nullptr ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateRGBSurfaceWithFormat: {:s}" ), TTF_GetError() );
    } else {
      static_cast< uint32_t* >( txtsfc->pixels )[0] = 0xFFFFFFFF;
      this->bouncerTextureColliding_ = SDL_CreateTextureFromSurface( renderer, txtsfc );
      if( !this->bouncerTextureColliding_ ) {
        this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateTextureFromSurface: {:s}" ), SDL_GetError() );
      }
      SDL_FreeSurface( txtsfc );
    }
  }
  if( this->bouncerTextureFree_ == nullptr ) {
    SDL_Surface* txtsfc = SDL_CreateRGBSurfaceWithFormat( 0, 1, 1, 32, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888 );
    if( txtsfc == nullptr ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateRGBSurfaceWithFormat: {:s}" ), TTF_GetError() );
    } else {
      static_cast< uint32_t* >( txtsfc->pixels )[0] = 0xFFFF00FF;
      this->bouncerTextureFree_ = SDL_CreateTextureFromSurface( renderer, txtsfc );
      if( !this->bouncerTextureFree_ ) {
        this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateTextureFromSurface: {:s}" ), SDL_GetError() );
      }
      SDL_FreeSurface( txtsfc );
    }
  }

  if( this->isTouching_ ) {
    if( SDL_RenderCopy( renderer, this->bouncerTextureTouching_, NULL, &( this->bouncerRect_ ) ) != 0 ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_RenderCopy: {:s}" ), SDL_GetError() );
    }
  } else if( this->isColliding_ ) {
    if( SDL_RenderCopy( renderer, this->bouncerTextureColliding_, NULL, &( this->bouncerRect_ ) ) != 0 ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_RenderCopy: {:s}" ), SDL_GetError() );
    }
  } else {
    if( SDL_RenderCopy( renderer, this->bouncerTextureFree_, NULL, &( this->bouncerRect_ ) ) != 0 ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_RenderCopy: {:s}" ), SDL_GetError() );
    }
  }
}

void SFG::Content::Bouncer::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  this->velocity_.set_x( this->moving_.x() );
  this->velocity_.set_y( this->moving_.y() );

  _base_::logic_update( deltaTime );

  if( this->isTouching_ ) {
    if( this->velocity_.x() == 0.0L ) {
      // horizontal collision
      this->moving_.set_x( -this->moving_.x() );
    }
    if( this->velocity_.y() == 0.0L ) {
      // vertical collision
      this->moving_.set_y( -this->moving_.y() );
    }
  }

  this->bouncerRect_.x = static_cast< int >( this->position_.x() );
  this->bouncerRect_.y = static_cast< int >( this->position_.y() );
  this->bouncerRect_.w = static_cast< int >( this->size_.x() );
  this->bouncerRect_.h = static_cast< int >( this->size_.y() );
}

void SFG::Content::Bouncer::set_speed( long double x, long double y ) {
  this->moving_.set_x( x );
  this->moving_.set_y( y );
}
