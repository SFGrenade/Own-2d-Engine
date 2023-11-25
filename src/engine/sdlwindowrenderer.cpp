#include "engine/sdlwindowrenderer.h"

#include <chrono>
#include <thread>

#include "_globals/moreChrono.h"
#include "engine/sdlwindow.h"


SFG::Engine::SdlWindowRenderer::SdlWindowRenderer( SFG::Engine::SdlWindow* sdlWindow )
    : logger_( spdlog::get( "Engine_SdlWindowRenderer" ) ),
      sdlWindow_( sdlWindow ),
      done_( false ),
      doneMutex_(),
      sdlFont_( TTF_OpenFont( R"(./Resources/Fonts/NotoSansMono-Regular.ttf)", 18 ) ),
      debugInfoTopLeft_( { "Hi 1", true, nullptr, SDL_Rect( 0, 0, 0, 0 ) } ),
      debugInfoTopRight_( { "Hi 2", true, nullptr, SDL_Rect( 0, 0, 0, 0 ) } ),
      debugInfoBottomLeft_( { "Hi 3", true, nullptr, SDL_Rect( 0, 0, 0, 0 ) } ),
      debugInfoBottomRight_( { "Hi 4", true, nullptr, SDL_Rect( 0, 0, 0, 0 ) } ),
      sdlRenderer_( nullptr ) {
  this->logger_->trace( fmt::runtime( "SdlWindowRenderer( sdlWindow = {:p} )" ), static_cast< void* >( sdlWindow ) );

  this->logger_->trace( fmt::runtime( "SdlWindowRenderer()~" ) );
}

SFG::Engine::SdlWindowRenderer::~SdlWindowRenderer() {
  this->logger_->trace( fmt::runtime( "~SdlWindowRenderer()" ) );

  if( this->sdlFont_ ) {
    TTF_CloseFont( this->sdlFont_ );
    this->sdlFont_ = nullptr;
  }
  if( this->debugInfoTopLeft_.texture_ ) {
    SDL_DestroyTexture( this->debugInfoTopLeft_.texture_ );
  }
  if( this->debugInfoTopRight_.texture_ ) {
    SDL_DestroyTexture( this->debugInfoTopRight_.texture_ );
  }
  if( this->debugInfoBottomLeft_.texture_ ) {
    SDL_DestroyTexture( this->debugInfoBottomLeft_.texture_ );
  }
  if( this->debugInfoBottomRight_.texture_ ) {
    SDL_DestroyTexture( this->debugInfoBottomRight_.texture_ );
  }
  if( this->sdlRenderer_ ) {
    SDL_DestroyRenderer( this->sdlRenderer_ );
    this->sdlRenderer_ = nullptr;
  }

  this->logger_->trace( fmt::runtime( "~SdlWindowRenderer()~" ) );
}

void SFG::Engine::SdlWindowRenderer::initialize_sdl_renderer( int32_t rendererIndex, SDL_RendererFlags flags ) {
  this->logger_->trace( fmt::runtime( "initialize_sdl_renderer( rendererIndex = {:d}, flags = 0b{:0>32b} )" ),
                        rendererIndex,
                        static_cast< uint32_t >( flags ) );

  if( this->sdlRenderer_ ) {
    this->logger_->trace( fmt::runtime( "initialize_sdl_renderer()~" ) );
    return;
  }
  this->sdlRenderer_ = SDL_CreateRenderer( this->sdlWindow_->get_sdl_window(), rendererIndex, static_cast< uint32_t >( flags ) );

  SDL_SetRenderDrawColor( this->sdlRenderer_, 0, 0, 0, SDL_ALPHA_OPAQUE );

  this->logger_->trace( fmt::runtime( "initialize_sdl_renderer()~" ) );
}

void SFG::Engine::SdlWindowRenderer::signal_quit() {
  this->logger_->trace( fmt::runtime( "signal_quit()" ) );

  this->doneMutex_.lock();
  this->done_ = true;
  this->doneMutex_.unlock();

  this->logger_->trace( fmt::runtime( "signal_quit()~" ) );
}

void SFG::Engine::SdlWindowRenderer::run_loop() {
  this->logger_->trace( fmt::runtime( "run_loop()" ) );

  this->doneMutex_.lock();
  while( !this->done_ ) {
    this->doneMutex_.unlock();

    /*
     * SDL_Rect( 0, 0, 0, 0 )
     * SDL_Rect( this->sdlWindow_->get_width(), 0, 0, 0 )
     * SDL_Rect( 0, this->sdlWindow_->get_height(), 0, 0 )
     * SDL_Rect( this->sdlWindow_->get_width(), this->sdlWindow_->get_height(), 0, 0 )
     */
    if( this->debugInfoTopLeft_.drawNew_ ) {
      this->renderDebugInfo( this->debugInfoTopLeft_ );
      this->debugInfoTopLeft_.textureRect_.x = 0;
      this->debugInfoTopLeft_.textureRect_.y = 0;
    }
    if( this->debugInfoTopRight_.drawNew_ ) {
      this->renderDebugInfo( this->debugInfoTopRight_ );
      this->debugInfoTopRight_.textureRect_.x = this->sdlWindow_->get_width() - this->debugInfoTopRight_.textureRect_.w;
      this->debugInfoTopRight_.textureRect_.y = 0;
    }
    if( this->debugInfoBottomLeft_.drawNew_ ) {
      this->renderDebugInfo( this->debugInfoBottomLeft_ );
      this->debugInfoBottomLeft_.textureRect_.x = 0;
      this->debugInfoBottomLeft_.textureRect_.y = this->sdlWindow_->get_height() - this->debugInfoBottomLeft_.textureRect_.h;
    }
    if( this->debugInfoBottomRight_.drawNew_ ) {
      this->renderDebugInfo( this->debugInfoBottomRight_ );
      this->debugInfoBottomRight_.textureRect_.x = this->sdlWindow_->get_width() - this->debugInfoBottomRight_.textureRect_.w;
      this->debugInfoBottomRight_.textureRect_.y = this->sdlWindow_->get_height() - this->debugInfoBottomRight_.textureRect_.h;
    }

    SDL_RenderClear( this->sdlRenderer_ );

    // Frame thingies to draw

    if( this->debugInfoTopLeft_.textureRect_.w > 0 && this->debugInfoTopLeft_.textureRect_.h > 0 && this->debugInfoTopLeft_.texture_ ) {
      SDL_RenderCopy( this->sdlRenderer_, this->debugInfoTopLeft_.texture_, NULL, &( this->debugInfoTopLeft_.textureRect_ ) );
    }
    if( this->debugInfoTopRight_.textureRect_.w > 0 && this->debugInfoTopRight_.textureRect_.h > 0 && this->debugInfoTopRight_.texture_ ) {
      SDL_RenderCopy( this->sdlRenderer_, this->debugInfoTopRight_.texture_, NULL, &( this->debugInfoTopRight_.textureRect_ ) );
    }
    if( this->debugInfoBottomLeft_.textureRect_.w > 0 && this->debugInfoBottomLeft_.textureRect_.h > 0 && this->debugInfoBottomLeft_.texture_ ) {
      SDL_RenderCopy( this->sdlRenderer_, this->debugInfoBottomLeft_.texture_, NULL, &( this->debugInfoBottomLeft_.textureRect_ ) );
    }
    if( this->debugInfoBottomRight_.textureRect_.w > 0 && this->debugInfoBottomRight_.textureRect_.h > 0 && this->debugInfoBottomRight_.texture_ ) {
      SDL_RenderCopy( this->sdlRenderer_, this->debugInfoBottomRight_.texture_, NULL, &( this->debugInfoBottomRight_.textureRect_ ) );
    }

    SDL_RenderPresent( this->sdlRenderer_ );

    this->doneMutex_.lock();
  }
  this->doneMutex_.unlock();
  // this->sdlWindow_->destroy_renderer( this->sdlRenderer_ );

  this->logger_->trace( fmt::runtime( "run_loop()~" ) );
}

void SFG::Engine::SdlWindowRenderer::set_debugInfo_topLeft( std::string const& debugInfo ) {
  this->logger_->trace( fmt::runtime( "set_debugInfo_topLeft( debugInfo = \"{:s}\" )" ), debugInfo );

  this->debugInfoTopLeft_.message_ = debugInfo;
  this->debugInfoTopLeft_.drawNew_ = true;

  this->logger_->trace( fmt::runtime( "set_debugInfo_topLeft()~" ) );
}

void SFG::Engine::SdlWindowRenderer::set_debugInfo_topRight( std::string const& debugInfo ) {
  this->logger_->trace( fmt::runtime( "set_debugInfo_topRight( debugInfo = \"{:s}\" )" ), debugInfo );

  this->debugInfoTopRight_.message_ = debugInfo;
  this->debugInfoTopRight_.drawNew_ = true;

  this->logger_->trace( fmt::runtime( "set_debugInfo_topRight()~" ) );
}

void SFG::Engine::SdlWindowRenderer::set_debugInfo_bottomLeft( std::string const& debugInfo ) {
  this->logger_->trace( fmt::runtime( "set_debugInfo_bottomLeft( debugInfo = \"{:s}\" )" ), debugInfo );

  this->debugInfoBottomLeft_.message_ = debugInfo;
  this->debugInfoBottomLeft_.drawNew_ = true;

  this->logger_->trace( fmt::runtime( "set_debugInfo_bottomLeft()~" ) );
}

void SFG::Engine::SdlWindowRenderer::set_debugInfo_bottomRight( std::string const& debugInfo ) {
  this->logger_->trace( fmt::runtime( "set_debugInfo_bottomRight( debugInfo = \"{:s}\" )" ), debugInfo );

  this->debugInfoBottomRight_.message_ = debugInfo;
  this->debugInfoBottomRight_.drawNew_ = true;

  this->logger_->trace( fmt::runtime( "set_debugInfo_bottomRight()~" ) );
}

void SFG::Engine::SdlWindowRenderer::renderDebugInfo( DebugInfoStruct& debugInfo ) {
  this->logger_->trace( fmt::runtime( "renderDebugInfo( debugInfo )" ) );

  SDL_Surface* txtsfc = TTF_RenderUTF8_Blended_Wrapped( this->sdlFont_,
                                                        debugInfo.message_.c_str(),
                                                        SDL_Color( 255, 255, 255, SDL_ALPHA_OPAQUE ),
                                                        this->sdlWindow_->get_width() );
  if( debugInfo.texture_ ) {
    SDL_DestroyTexture( debugInfo.texture_ );
  }
  debugInfo.texture_ = SDL_CreateTextureFromSurface( this->sdlRenderer_, txtsfc );
  debugInfo.textureRect_.w = txtsfc->w;
  debugInfo.textureRect_.h = txtsfc->h;
  SDL_FreeSurface( txtsfc );
  debugInfo.drawNew_ = false;

  this->logger_->trace( fmt::runtime( "renderDebugInfo()~" ) );
}
