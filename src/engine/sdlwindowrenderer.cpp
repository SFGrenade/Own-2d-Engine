#include "engine/sdlwindowrenderer.h"

#include <chrono>
#include <thread>

#include "_globals/moreChrono.h"
#include "engine/sdlwindow.h"


SFG::Engine::SdlWindowRenderer::SdlWindowRenderer( SFG::Engine::SdlWindow* sdlWindow )
    : logger_( spdlog::get( "Engine_SdlWindowRenderer" ) ), sdlWindow_( sdlWindow ), done_( false ), doneMutex_(), sdlRenderer_() {
  this->logger_->trace( fmt::runtime( "SdlWindowRenderer( sdlWindow = {:p} )" ), static_cast< void* >( sdlWindow ) );

  this->logger_->trace( fmt::runtime( "SdlWindowRenderer()~" ) );
}

SFG::Engine::SdlWindowRenderer::~SdlWindowRenderer() {
  this->logger_->trace( fmt::runtime( "~SdlWindowRenderer()" ) );

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

  SDL_SetRenderDrawColor( this->sdlRenderer_,
                          1 << ( this->sdlWindow_->get_sdl_window_id() ),
                          1 << ( this->sdlWindow_->get_sdl_window_id() ),
                          1 << ( this->sdlWindow_->get_sdl_window_id() ),
                          SDL_ALPHA_OPAQUE );

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

    SDL_RenderClear( this->sdlRenderer_ );

    // Frame thingies to draw

    SDL_RenderPresent( this->sdlRenderer_ );

    this->doneMutex_.lock();
  }
  this->doneMutex_.unlock();
  // this->sdlWindow_->destroy_renderer( this->sdlRenderer_ );

  this->logger_->trace( fmt::runtime( "run_loop()~" ) );
}
