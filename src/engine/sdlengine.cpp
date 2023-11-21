#include "engine/sdlengine.h"

#include <chrono>
#include <thread>

#include "_globals/moreChrono.h"
#include "engine/sdlwindow.h"

SFG::Engine::SdlEngine::SdlEngine( uint32_t sdlInitFlags, IMG_InitFlags imgInitFlags, MIX_InitFlags mixInitFlags )
    : logger_( spdlog::get( "Engine_SdlEngine" ) ),
      windows_(),
      windowsMutex_(),
      windowIdsToDestroy_(),
      windowIdsToDestroyMutex_(),
      closedWindowIds_(),
      closedWindowIdsMutex_() {
  this->logger_->trace( fmt::runtime( "SdlEngine( sdlInitFlags = 0b{:0>32b}, imgInitFlags = 0b{:0>32b}, mixInitFlags = 0b{:0>32b} )" ),
                        sdlInitFlags,
                        static_cast< uint32_t >( imgInitFlags ),
                        static_cast< uint32_t >( mixInitFlags ) );

  if( SDL_Init( sdlInitFlags ) != 0 ) {
    throw new std::runtime_error( std::string( "SDL_Init error: " ) + std::string( SDL_GetError() ) );
  }
  if( IMG_Init( imgInitFlags ) == 0 ) {
    throw new std::runtime_error( std::string( "IMG_Init error" ) );
  }
  if( Mix_Init( mixInitFlags ) == 0 ) {
    throw new std::runtime_error( std::string( "Mix_Init error" ) );
  }
  if( TTF_Init() != 0 ) {
    throw new std::runtime_error( std::string( "TTF_Init error: " ) + std::string( TTF_GetError() ) );
  }

  this->logger_->trace( fmt::runtime( "SdlEngine()~" ) );
}

SFG::Engine::SdlEngine::~SdlEngine() {
  this->logger_->trace( fmt::runtime( "~SdlEngine()" ) );

  this->windowsMutex_.lock();
  for( std::pair< uint32_t, SFG::Engine::SdlWindow* > window : this->windows_ ) {
    delete window.second;
  }
  this->windows_.clear();
  this->windowsMutex_.unlock();
  this->closedWindowIdsMutex_.lock();
  this->closedWindowIds_.clear();
  this->closedWindowIdsMutex_.unlock();

  TTF_Quit();
  Mix_Quit();
  IMG_Quit();
  SDL_Quit();

  this->logger_->trace( fmt::runtime( "~SdlEngine()~" ) );
}

SFG::Engine::SdlWindow* SFG::Engine::SdlEngine::add_window( std::string const& title,
                                                            uint32_t width,
                                                            uint32_t height,
                                                            SDL_WindowFlags flags,
                                                            uint32_t x,
                                                            uint32_t y ) {
  this->logger_->trace( fmt::runtime( "add_window( title: \"{:s}\", width: {:d}, height: {:d}, flags: 0b{:0>32b}, x: {:d}, y: {:d} )" ),
                        title,
                        width,
                        height,
                        static_cast< uint32_t >( flags ),
                        x,
                        y );

  SFG::Engine::SdlWindow* ret = new SFG::Engine::SdlWindow( this );
  ret->set_title( title );
  ret->set_x( x );
  ret->set_y( y );
  ret->set_width( width );
  ret->set_height( height );
  ret->set_flags( flags );

  uint32_t window_id = ret->initialize_sdl_window();

  this->logger_->trace( fmt::runtime( "add_window - window_id = {:d}" ), window_id );
  this->windowsMutex_.lock();
  this->windows_[window_id] = ret;
  this->windowsMutex_.unlock();

  this->logger_->trace( fmt::runtime( "add_window()~" ) );
  return ret;
}

void SFG::Engine::SdlEngine::destroy_window( uint32_t windowId ) {
  this->logger_->trace( fmt::runtime( "destroy_window( windowId = {:d} )" ), windowId );

  this->windowIdsToDestroyMutex_.lock();
  this->windowIdsToDestroy_.push( windowId );
  this->windowIdsToDestroyMutex_.unlock();

  this->logger_->trace( fmt::runtime( "destroy_window()~" ) );
}

void SFG::Engine::SdlEngine::run_input_loop() {
  this->logger_->trace( fmt::runtime( "run_input_loop()" ) );

  SDL_Event e;
  std::queue< SDL_Event > tempEventQueue;
  bool done = false;
  while( !done ) {
    this->windowIdsToDestroyMutex_.lock();
    while( !this->windowIdsToDestroy_.empty() ) {
      uint32_t windowId = this->windowIdsToDestroy_.front();
      this->windowIdsToDestroy_.pop();
      this->windowIdsToDestroyMutex_.unlock();

      this->windowsMutex_.lock();
      if( this->windows_.contains( windowId ) ) {
        delete this->windows_[windowId];
        this->windows_.erase( windowId );
        this->closedWindowIdsMutex_.lock();
        this->closedWindowIds_.insert( windowId );
        this->closedWindowIdsMutex_.unlock();
      }
      this->windowsMutex_.unlock();

      this->windowIdsToDestroyMutex_.lock();
    }
    this->windowIdsToDestroyMutex_.unlock();

    this->windowsMutex_.lock();
    if( this->windows_.size() <= 0 && this->closedWindowIds_.size() > 0 ) {
      // if windows were closed and no windows are open, we are done
      done = true;
    }
    this->windowsMutex_.unlock();

    while( SDL_PollEvent( &e ) ) {
      // this->logger_->trace( fmt::runtime( "run_input_loop - event type = {:s}" ), SDL_EventType_to_string( static_cast< SDL_EventType >( e.type ) ) );
      if( !run_input_loop( e, &done ) ) {
        SDL_PushEvent( &e );
      }
    }
  }

  this->logger_->trace( fmt::runtime( "run_input_loop()~" ) );
}

bool SFG::Engine::SdlEngine::run_input_loop( SDL_Event const& e, bool* donePtr ) {
  this->logger_->trace( fmt::runtime( "run_input_loop( e = {:s}, donePtr = {:p} )" ),
                        SDL_EventType_to_string( static_cast< SDL_EventType >( e.type ) ),
                        static_cast< void* >( donePtr ) );

  switch( e.type ) {
    case SDL_EventType::SDL_QUIT:
      // ( *donePtr ) = true;
      this->windowsMutex_.lock();
      for( std::pair< uint32_t, SFG::Engine::SdlWindow* > window : this->windows_ ) {
        window.second->add_input( e );
      }
      this->windowsMutex_.unlock();
      return true;
    case SDL_EventType::SDL_WINDOWEVENT:
      this->logger_->trace( fmt::runtime( "run_input_loop - e.window {:s} window id = {:d}" ),
                            SDL_WindowEventID_to_string( static_cast< SDL_WindowEventID >( e.window.event ) ),
                            e.window.windowID );
      if( this->has_window( e.window.windowID ) ) {
        this->get_window( e.window.windowID )->add_input( e );
        return true;
      } else if( this->closedWindowIds_.contains( e.window.windowID ) ) {
        return true;
      } else {
        return false;
      }
    case SDL_EventType::SDL_KEYDOWN:
    case SDL_EventType::SDL_KEYUP:
      this->logger_->trace( fmt::runtime( "run_input_loop - e.key {:d} window id = {:d}" ), e.key.type, e.key.windowID );
      if( this->has_window( e.key.windowID ) ) {
        this->get_window( e.key.windowID )->add_input( e );
        return true;
      } else if( this->closedWindowIds_.contains( e.window.windowID ) ) {
        return true;
      } else {
        return false;
      }
    case SDL_EventType::SDL_TEXTEDITING:
      this->logger_->trace( fmt::runtime( "run_input_loop - e.edit {:d} window id = {:d}" ), e.edit.type, e.edit.windowID );
      if( this->has_window( e.edit.windowID ) ) {
        this->get_window( e.edit.windowID )->add_input( e );
        return true;
      } else if( this->closedWindowIds_.contains( e.window.windowID ) ) {
        return true;
      } else {
        return false;
      }
    case SDL_EventType::SDL_TEXTEDITING_EXT:
      this->logger_->trace( fmt::runtime( "run_input_loop - e.editExt {:d} window id = {:d}" ), e.editExt.type, e.editExt.windowID );
      if( this->has_window( e.editExt.windowID ) ) {
        this->get_window( e.editExt.windowID )->add_input( e );
        return true;
      } else if( this->closedWindowIds_.contains( e.window.windowID ) ) {
        return true;
      } else {
        return false;
      }
    case SDL_EventType::SDL_TEXTINPUT:
      this->logger_->trace( fmt::runtime( "run_input_loop - e.text {:d} window id = {:d}" ), e.text.type, e.text.windowID );
      if( this->has_window( e.text.windowID ) ) {
        this->get_window( e.text.windowID )->add_input( e );
        return true;
      } else if( this->closedWindowIds_.contains( e.window.windowID ) ) {
        return true;
      } else {
        return false;
      }
    case SDL_EventType::SDL_MOUSEMOTION:
      this->logger_->trace( fmt::runtime( "run_input_loop - e.motion {:d} window id = {:d}" ), e.motion.type, e.motion.windowID );
      if( this->has_window( e.motion.windowID ) ) {
        this->get_window( e.motion.windowID )->add_input( e );
        return true;
      } else if( this->closedWindowIds_.contains( e.window.windowID ) ) {
        return true;
      } else {
        return false;
      }
    case SDL_EventType::SDL_MOUSEBUTTONDOWN:
    case SDL_EventType::SDL_MOUSEBUTTONUP:
      this->logger_->trace( fmt::runtime( "run_input_loop - e.button {:d} window id = {:d}" ), e.button.type, e.button.windowID );
      if( this->has_window( e.button.windowID ) ) {
        this->get_window( e.button.windowID )->add_input( e );
        return true;
      } else if( this->closedWindowIds_.contains( e.window.windowID ) ) {
        return true;
      } else {
        return false;
      }
    case SDL_EventType::SDL_MOUSEWHEEL:
      this->logger_->trace( fmt::runtime( "run_input_loop - e.wheel {:d} window id = {:d}" ), e.wheel.type, e.wheel.windowID );
      if( this->has_window( e.wheel.windowID ) ) {
        this->get_window( e.wheel.windowID )->add_input( e );
        return true;
      } else if( this->closedWindowIds_.contains( e.window.windowID ) ) {
        return true;
      } else {
        return false;
      }
    case SDL_EventType::SDL_FINGERMOTION:
    case SDL_EventType::SDL_FINGERDOWN:
    case SDL_EventType::SDL_FINGERUP:
      this->logger_->trace( fmt::runtime( "run_input_loop - e.tfinger {:d} window id = {:d}" ), e.tfinger.type, e.tfinger.windowID );
      if( this->has_window( e.tfinger.windowID ) ) {
        this->get_window( e.tfinger.windowID )->add_input( e );
        return true;
      } else if( this->closedWindowIds_.contains( e.window.windowID ) ) {
        return true;
      } else {
        return false;
      }
    case SDL_EventType::SDL_DROPBEGIN:
    case SDL_EventType::SDL_DROPFILE:
    case SDL_EventType::SDL_DROPTEXT:
    case SDL_EventType::SDL_DROPCOMPLETE:
      this->logger_->trace( fmt::runtime( "run_input_loop - e.drop {:d} window id = {:d}" ), e.drop.type, e.drop.windowID );
      if( this->has_window( e.drop.windowID ) ) {
        this->get_window( e.drop.windowID )->add_input( e );
        return true;
      } else if( this->closedWindowIds_.contains( e.window.windowID ) ) {
        return true;
      } else {
        return false;
      }
    default:
      if( e.type >= SDL_EventType::SDL_USEREVENT && e.type < SDL_EventType::SDL_LASTEVENT ) {
        this->logger_->trace( fmt::runtime( "run_input_loop - e.user {:d} window id = {:d}" ), e.user.type, e.user.windowID );
        if( this->has_window( e.user.windowID ) ) {
          this->get_window( e.user.windowID )->add_input( e );
          return true;
        } else if( this->closedWindowIds_.contains( e.window.windowID ) ) {
          return true;
        }
        return false;
      } else {
        return true;
      }
      break;
  }

  // this->logger_->trace( fmt::runtime( "run_input_loop()~" ) );
  return true;
}

bool SFG::Engine::SdlEngine::has_window( uint32_t windowId ) {
  this->logger_->trace( fmt::runtime( "has_window( windowId = {:d} )" ), windowId );

  this->windowsMutex_.lock();
  bool ret = this->windows_.contains( windowId );
  this->windowsMutex_.unlock();

  this->logger_->trace( fmt::runtime( "has_window()~" ) );
  return ret;
}

SFG::Engine::SdlWindow* SFG::Engine::SdlEngine::get_window( uint32_t windowId ) {
  this->logger_->trace( fmt::runtime( "get_window( windowId = {:d} )" ), windowId );

  SFG::Engine::SdlWindow* ret = nullptr;
  this->windowsMutex_.lock();
  if( this->windows_.contains( windowId ) ) {
    ret = this->windows_[windowId];
  }
  this->windowsMutex_.unlock();

  this->logger_->trace( fmt::runtime( "get_window()~" ) );
  return ret;
}
