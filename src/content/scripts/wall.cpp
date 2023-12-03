#include "content/scripts/wall.h"

#include "engine/loggerfactory.h"
#include "engine/sdlwindow.h"


SFG::Content::Wall::Wall()
    : _base_(),
      logger_( SFG::Engine::LoggerFactory::get_logger( "Content_Wall" ) ),
      rendering_( true ),
      wallRect_(),
      wallTextureCollision_( nullptr ),
      wallTextureNoCollision_( nullptr ) {}

SFG::Content::Wall::~Wall() {
  if( this->wallTextureCollision_ ) {
    this->logger_->trace( fmt::runtime( "~Wall - destroying wallTextureCollision" ) );
    SDL_DestroyTexture( this->wallTextureCollision_ );
    this->wallTextureCollision_ = nullptr;
  }
  if( this->wallTextureNoCollision_ ) {
    this->logger_->trace( fmt::runtime( "~Wall - destroying wallTextureNoCollision" ) );
    SDL_DestroyTexture( this->wallTextureNoCollision_ );
    this->wallTextureNoCollision_ = nullptr;
  }
}

void SFG::Content::Wall::frame_update( SDL_Renderer* renderer ) {
  _base_::frame_update( renderer );

  if( !this->rendering_ ) {
    return;
  }
  if( this->wallTextureCollision_ == nullptr ) {
    SDL_Surface* txtsfc = SDL_CreateRGBSurfaceWithFormat( 0, 1, 1, 32, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888 );
    if( txtsfc == nullptr ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateRGBSurface: {:s}" ), TTF_GetError() );
    } else {
      static_cast< uint32_t* >( txtsfc->pixels )[0] = 0x00FFFFFF;
      this->wallTextureCollision_ = SDL_CreateTextureFromSurface( renderer, txtsfc );
      if( !this->wallTextureCollision_ ) {
        this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateTextureFromSurface: {:s}" ), SDL_GetError() );
      }
      SDL_FreeSurface( txtsfc );
    }
  }
  if( this->wallTextureNoCollision_ == nullptr ) {
    SDL_Surface* txtsfc = SDL_CreateRGBSurfaceWithFormat( 0, 1, 1, 32, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888 );
    if( txtsfc == nullptr ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateRGBSurface: {:s}" ), TTF_GetError() );
    } else {
      static_cast< uint32_t* >( txtsfc->pixels )[0] = 0x00FF00FF;
      this->wallTextureNoCollision_ = SDL_CreateTextureFromSurface( renderer, txtsfc );
      if( !this->wallTextureNoCollision_ ) {
        this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateTextureFromSurface: {:s}" ), SDL_GetError() );
      }
      SDL_FreeSurface( txtsfc );
    }
  }

  if( this->enteredCollidersFromAbove_.size() > 0 || this->enteredCollidersFromBelow_.size() > 0 || this->enteredCollidersFromLeft_.size() > 0
      || this->enteredCollidersFromRight_.size() > 0 ) {
    if( SDL_RenderCopy( renderer, this->wallTextureCollision_, NULL, &( this->wallRect_ ) ) != 0 ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_RenderCopy: {:s}" ), SDL_GetError() );
    }
  } else {
    if( SDL_RenderCopy( renderer, this->wallTextureNoCollision_, NULL, &( this->wallRect_ ) ) != 0 ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_RenderCopy: {:s}" ), SDL_GetError() );
    }
  }
}

void SFG::Content::Wall::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  _base_::logic_update( deltaTime );

  this->wallRect_.x = static_cast< int >( this->position_.x );
  this->wallRect_.y = static_cast< int >( this->position_.y );
  this->wallRect_.w = static_cast< int >( this->size_.x );
  this->wallRect_.h = static_cast< int >( this->size_.y );
}
