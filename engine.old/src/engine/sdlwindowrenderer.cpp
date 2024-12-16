#include "engine/sdlwindowrenderer.h"

#include <thread>

#include "_globals/moreChrono.h"
#include "engine/loggerfactory.h"
#include "engine/performancecontroller.h"
#include "engine/scriptmanager.h"
#include "engine/sdlwindow.h"


SFG::Engine::SdlWindowRenderer::SdlWindowRenderer( SFG::Engine::SdlWindow* sdlWindow )
    : logger_( SFG::Engine::LoggerFactory::get_logger( "Engine_SdlWindowRenderer" ) ),
      sdlWindow_( sdlWindow ),
      done_( false ),
      doneMutex_(),
      sdlRenderer_( nullptr ) {
  this->logger_->trace( fmt::runtime( "SdlWindowRenderer( sdlWindow = {:p} )" ), static_cast< void* >( sdlWindow ) );

  this->logger_->trace( fmt::runtime( "SdlWindowRenderer()~" ) );
}

SFG::Engine::SdlWindowRenderer::~SdlWindowRenderer() {
  this->logger_->trace( fmt::runtime( "~SdlWindowRenderer()" ) );

  if( this->sdlRenderer_ ) {
    this->logger_->trace( fmt::runtime( "~SdlWindowRenderer - destroying renderer" ) );
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
  if( this->sdlRenderer_ == nullptr ) {
    this->logger_->error( fmt::runtime( "initialize_sdl_renderer - Error when SDL_CreateRenderer: {:s}" ), SDL_GetError() );
  }

  if( SDL_SetRenderDrawColor( this->sdlRenderer_, 32, 32, 32, SDL_ALPHA_OPAQUE ) != 0 ) {
    this->logger_->error( fmt::runtime( "initialize_sdl_renderer - Error when SDL_SetRenderDrawColor: {:s}" ), SDL_GetError() );
  }

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

    if( SDL_RenderClear( this->sdlRenderer_ ) != 0 ) {
      this->logger_->error( fmt::runtime( "run_loop - Error when SDL_RenderClear: {:s}" ), SDL_GetError() );
    }

    // Frame thingies to draw
    this->sdlWindow_->get_script_manager()->frame_update( this->sdlRenderer_ );

    SDL_RenderPresent( this->sdlRenderer_ );

    this->sdlWindow_->get_performance_controller()->incRenderLoops();

    this->doneMutex_.lock();
  }
  this->doneMutex_.unlock();

  this->logger_->trace( fmt::runtime( "run_loop()~" ) );
}
