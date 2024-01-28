#include "content/scripts/wall.h"

#include "engine/loggerfactory.h"
#include "engine/sdlwindow.h"


SFG::Content::Wall::Wall()
    : _base_(), logger_( SFG::Engine::LoggerFactory::get_logger( "Content_Wall" ) ), rendering_( true ), wallRect_(), wallTexture_( nullptr ) {
  this->isStatic_ = true;
  this->isTrigger_ = false;
}

SFG::Content::Wall::~Wall() {
  if( this->wallTexture_ ) {
    this->logger_->trace( fmt::runtime( "~Wall - destroying wallTextureFree" ) );
    SDL_DestroyTexture( this->wallTexture_ );
    this->wallTexture_ = nullptr;
  }
}

void SFG::Content::Wall::frame_update( SDL_Renderer* renderer ) {
  _base_::frame_update( renderer );

  if( !this->rendering_ ) {
    return;
  }
  if( this->wallTexture_ == nullptr ) {
    SDL_Surface* txtsfc = SDL_CreateRGBSurfaceWithFormat( 0, 1, 1, 32, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888 );
    if( txtsfc == nullptr ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateRGBSurfaceWithFormat: {:s}" ), TTF_GetError() );
    } else {
      static_cast< uint32_t* >( txtsfc->pixels )[0] = 0x00FF00FF;
      this->wallTexture_ = SDL_CreateTextureFromSurface( renderer, txtsfc );
      if( !this->wallTexture_ ) {
        this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateTextureFromSurface: {:s}" ), SDL_GetError() );
      }
      SDL_FreeSurface( txtsfc );
    }
  }

  if( SDL_RenderCopy( renderer, this->wallTexture_, NULL, &( this->wallRect_ ) ) != 0 ) {
    this->logger_->error( fmt::runtime( "frame_update - Error when SDL_RenderCopy: {:s}" ), SDL_GetError() );
  }
}

void SFG::Content::Wall::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  _base_::logic_update( deltaTime );

  this->wallRect_.x = static_cast< int >( this->position_.x() );
  this->wallRect_.y = static_cast< int >( this->position_.y() );
  this->wallRect_.w = static_cast< int >( this->size_.x() );
  this->wallRect_.h = static_cast< int >( this->size_.y() );
}
