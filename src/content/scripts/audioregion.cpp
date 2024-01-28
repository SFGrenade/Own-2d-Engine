#include "content/scripts/audioregion.h"

#include "content/scripts/player.h"
#include "engine/loggerfactory.h"
#include "engine/sdlaudio.h"
#include "engine/sdlengine.h"
#include "engine/sdlwindow.h"


SFG::Content::AudioRegion::AudioRegion()
    : _base_(),
      logger_( SFG::Engine::LoggerFactory::get_logger( "Content_AudioRegion" ) ),
      rendering_( true ),
      audioRegionRect_(),
      audioRegionTexture_( nullptr ),
      audioFilePath_( "" ) {
  this->isStatic_ = true;
  this->isTrigger_ = true;
}

SFG::Content::AudioRegion::~AudioRegion() {
  if( this->audioRegionTexture_ ) {
    this->logger_->trace( fmt::runtime( "~AudioRegion - destroying audioRegionTextureFree" ) );
    SDL_DestroyTexture( this->audioRegionTexture_ );
    this->audioRegionTexture_ = nullptr;
  }
}

void SFG::Content::AudioRegion::frame_update( SDL_Renderer* renderer ) {
  _base_::frame_update( renderer );

  if( !this->rendering_ ) {
    return;
  }
  if( this->audioRegionTexture_ == nullptr ) {
    SDL_Surface* txtsfc = SDL_CreateRGBSurfaceWithFormat( 0, 1, 1, 32, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888 );
    if( txtsfc == nullptr ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateRGBSurfaceWithFormat: {:s}" ), TTF_GetError() );
    } else {
      static_cast< uint32_t* >( txtsfc->pixels )[0] = 0x0000FFFF;
      this->audioRegionTexture_ = SDL_CreateTextureFromSurface( renderer, txtsfc );
      if( !this->audioRegionTexture_ ) {
        this->logger_->error( fmt::runtime( "frame_update - Error when SDL_CreateTextureFromSurface: {:s}" ), SDL_GetError() );
      }
      SDL_FreeSurface( txtsfc );
    }
  }

  if( SDL_RenderCopy( renderer, this->audioRegionTexture_, NULL, &( this->audioRegionRect_ ) ) != 0 ) {
    this->logger_->error( fmt::runtime( "frame_update - Error when SDL_RenderCopy: {:s}" ), SDL_GetError() );
  }
}

void SFG::Content::AudioRegion::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  _base_::logic_update( deltaTime );

  this->audioRegionRect_.x = static_cast< int >( this->position_.x() );
  this->audioRegionRect_.y = static_cast< int >( this->position_.y() );
  this->audioRegionRect_.w = static_cast< int >( this->size_.x() );
  this->audioRegionRect_.h = static_cast< int >( this->size_.y() );
}

void SFG::Content::AudioRegion::interact( SFG::Engine::ScriptCollider const* contact, SFG::Engine::InteractSide interactionSide ) {
  if( dynamic_cast< SFG::Content::Player const* >( contact ) == nullptr ) {
    return;
  }
  if( audioFilePath_ == "" ) {
    return;
  }
  SFG::Engine::SdlEngine::get_instance()->get_audio()->play_background_music( this->audioFilePath_ );
}

void SFG::Content::AudioRegion::set_audio_file_path( std::string const& audioFilePath ) {
  this->audioFilePath_ = audioFilePath;
}
