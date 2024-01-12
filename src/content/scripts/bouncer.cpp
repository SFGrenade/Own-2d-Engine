#include "content/scripts/bouncer.h"

#include "engine/loggerfactory.h"
#include "engine/sdlwindow.h"


SFG::Content::Bouncer::Bouncer()
    : _base_(),
      logger_( SFG::Engine::LoggerFactory::get_logger( "Content_Bouncer" ) ),
      rendering_( true ),
      bouncerRect_(),
      bouncerTexture_( nullptr ),
      moving_( SFG::Engine::Vector2( 0.0L, 0.0L ) ) {}

SFG::Content::Bouncer::~Bouncer() {
  if( this->bouncerTexture_ ) {
    this->logger_->trace( fmt::runtime( "~Bouncer - destroying bouncerTextureFree" ) );
    SDL_DestroyTexture( this->bouncerTexture_ );
    this->bouncerTexture_ = nullptr;
  }
}

void SFG::Content::Bouncer::frame_update( SDL_Renderer* renderer ) {
  _base_::frame_update( renderer );

  if( !this->rendering_ ) {
    return;
  }
  if( this->bouncerTexture_ == nullptr ) {
    SDL_Surface* txtsfc = SDL_CreateRGBSurfaceWithFormat( 0, 1, 1, 32, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888 );
    if( txtsfc == nullptr ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateRGBSurfaceWithFormat: {:s}" ), TTF_GetError() );
    } else {
      static_cast< uint32_t* >( txtsfc->pixels )[0] = 0xFFFF00FF;
      this->bouncerTexture_ = SDL_CreateTextureFromSurface( renderer, txtsfc );
      if( !this->bouncerTexture_ ) {
        this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateTextureFromSurface: {:s}" ), SDL_GetError() );
      }
      SDL_FreeSurface( txtsfc );
    }
  }

  if( SDL_RenderCopy( renderer, this->bouncerTexture_, NULL, &( this->bouncerRect_ ) ) != 0 ) {
    this->logger_->error( fmt::runtime( "frame_update - Error when SDL_RenderCopy: {:s}" ), SDL_GetError() );
  }
}

void SFG::Content::Bouncer::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  this->velocity_.set_x( this->moving_.x() );
  this->velocity_.set_y( this->moving_.y() );

  _base_::logic_update( deltaTime );

  this->bouncerRect_.x = static_cast< int >( this->position_.x() );
  this->bouncerRect_.y = static_cast< int >( this->position_.y() );
  this->bouncerRect_.w = static_cast< int >( this->size_.x() );
  this->bouncerRect_.h = static_cast< int >( this->size_.y() );
}

void SFG::Content::Bouncer::interact( SFG::Engine::ScriptCollider const* contact, SFG::Engine::InteractSide interactionSide ) {
  switch( interactionSide ) {
    case SFG::Engine::InteractSide::Top:
    case SFG::Engine::InteractSide::Bottom:
      this->moving_.set_y( -this->moving_.y() );
      break;
    case SFG::Engine::InteractSide::Left:
    case SFG::Engine::InteractSide::Right:
      this->moving_.set_x( -this->moving_.x() );
      break;
  }
}

void SFG::Content::Bouncer::set_speed( long double x, long double y ) {
  this->moving_.set_x( x );
  this->moving_.set_y( y );
}
