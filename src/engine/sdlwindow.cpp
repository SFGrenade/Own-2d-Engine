#include "engine/sdlwindow.h"

#include <chrono>
#include <thread>

#include "_globals/moreChrono.h"
#include "engine/logiccontroller.h"
#include "engine/performancecontroller.h"
#include "engine/scriptmanager.h"
#include "engine/sdlengine.h"
#include "engine/sdlwindowrenderer.h"


SFG::Engine::SdlWindow::SdlWindow( SdlEngine* sdlEngine )
    : logger_( spdlog::get( "Engine_SdlWindow" ) ),
      sdlEngine_( sdlEngine ),
      sdlInputQueue_(),
      sdlInputQueueMutex_(),
      sdlWindow_( nullptr ),
      sdlWindowId_( 0 ),
      performanceController_( new SFG::Engine::PerformanceController( this ) ),
      scriptManager_( new SFG::Engine::ScriptManager( this ) ),
      sdlRenderer_( nullptr ),
      sdlRendererThread_(),
      logicController_( nullptr ),
      logicControllerThread_(),
      title_( "" ),
      x_( SDL_WINDOWPOS_CENTERED ),
      y_( SDL_WINDOWPOS_CENTERED ),
      width_( 800 ),
      height_( 600 ),
      flags_( static_cast< SDL_WindowFlags >( 0 ) ) {
  this->logger_->trace( fmt::runtime( "SdlWindow( sdlEngine = {:p} )" ), static_cast< void* >( sdlEngine ) );

  this->logger_->trace( fmt::runtime( "SdlWindow()~" ) );
}

SFG::Engine::SdlWindow::~SdlWindow() {
  this->logger_->trace( fmt::runtime( "~SdlWindow()" ) );

  sdlRendererThread_.join();
  if( this->sdlRenderer_ ) {
    delete this->sdlRenderer_;
    this->sdlRenderer_ = nullptr;
  }
  logicControllerThread_.join();
  if( this->logicController_ ) {
    delete this->logicController_;
    this->logicController_ = nullptr;
  }
  if( this->scriptManager_ ) {
    delete this->scriptManager_;
    this->scriptManager_ = nullptr;
  }
  if( this->performanceController_ ) {
    delete this->performanceController_;
    this->performanceController_ = nullptr;
  }
  if( this->sdlWindow_ ) {
    SDL_DestroyWindow( this->sdlWindow_ );
    this->sdlWindow_ = nullptr;
  }

  this->logger_->trace( fmt::runtime( "~SdlWindow()~" ) );
}

void SFG::Engine::SdlWindow::initialize_sdl_window() {
  this->logger_->trace( fmt::runtime( "initialize_sdl_window()" ) );

  if( this->sdlWindow_ ) {
    this->logger_->trace( fmt::runtime( "initialize_sdl_window()~" ) );
    return;
  }
  this->sdlWindow_ = SDL_CreateWindow( this->title_.c_str(), this->x_, this->y_, this->width_, this->height_, static_cast< uint32_t >( this->flags_ ) );
  this->sdlWindowId_ = SDL_GetWindowID( this->sdlWindow_ );
  this->flags_ = static_cast< SDL_WindowFlags >( SDL_GetWindowFlags( this->sdlWindow_ ) );

  this->logger_->trace( fmt::runtime( "initialize_sdl_window()~" ) );
}

SFG::Engine::SdlWindowRenderer* SFG::Engine::SdlWindow::initialize_window_renderer( std::string const& renderer, SDL_RendererFlags flags ) {
  this->logger_->trace( fmt::runtime( "initialize_window_renderer( renderer = \"{:s}\", flags = 0b{:0>32b} )" ), renderer, static_cast< uint32_t >( flags ) );

  if( this->sdlRenderer_ ) {
    this->logger_->trace( fmt::runtime( "initialize_window_renderer()~" ) );
    return this->sdlRenderer_;
  }

  this->sdlRenderer_ = new SFG::Engine::SdlWindowRenderer( this );

  std::string wantedRenderer = renderer;
  this->sdlRendererThread_ = std::thread( [this, wantedRenderer, flags]() {
    int32_t rendererIndex = -1;
    std::vector< std::string > availableRenderers = this->sdlEngine_->get_renderer_names();
    int32_t i = 0;
    for( std::string availableRenderer : availableRenderers ) {
      if( wantedRenderer == availableRenderer ) {
        rendererIndex = i;
        break;
      }
      ++i;
    }

    this->sdlRenderer_->initialize_sdl_renderer( rendererIndex, flags );
    this->sdlRenderer_->run_loop();
  } );

  this->logger_->trace( fmt::runtime( "initialize_window_renderer()~" ) );
  return this->sdlRenderer_;
}

SFG::Engine::LogicController* SFG::Engine::SdlWindow::initialize_logic_controller() {
  this->logger_->trace( fmt::runtime( "initialize_logic_controller()" ) );

  if( this->logicController_ ) {
    this->logger_->trace( fmt::runtime( "initialize_logic_controller()~" ) );
    return this->logicController_;
  }

  this->logicController_ = new SFG::Engine::LogicController( this );

  this->logicControllerThread_ = std::thread( [this]() { this->logicController_->run_loop(); } );

  this->logger_->trace( fmt::runtime( "initialize_logic_controller()~" ) );
  return this->logicController_;
}

void SFG::Engine::SdlWindow::add_input( SDL_Event const& e ) {
  this->logger_->trace( fmt::runtime( "add_input( event type = {:s} )" ), SDL_EventType_to_string( static_cast< SDL_EventType >( e.type ) ) );

  sdlInputQueueMutex_.lock();
  sdlInputQueue_.push( e );
  sdlInputQueueMutex_.unlock();

  this->logger_->trace( fmt::runtime( "add_input()~" ) );
}

void SFG::Engine::SdlWindow::run_input_loop() {
  this->logger_->trace( fmt::runtime( "run_input_loop()" ) );

  SDL_Event e;
  bool done = false;
  bool hasInputs;
  while( !done ) {
    sdlInputQueueMutex_.lock();
    hasInputs = !sdlInputQueue_.empty();
    sdlInputQueueMutex_.unlock();
    while( hasInputs ) {
      sdlInputQueueMutex_.lock();
      e = sdlInputQueue_.front();
      sdlInputQueue_.pop();
      sdlInputQueueMutex_.unlock();
      this->logger_->trace( fmt::runtime( "run_input_loop - event type = {:s}" ), SDL_EventType_to_string( static_cast< SDL_EventType >( e.type ) ) );
      switch( e.type ) {
        case SDL_EventType::SDL_QUIT:
          done = true;
          break;
        case SDL_EventType::SDL_WINDOWEVENT:
          this->logger_->trace( fmt::runtime( "run_input_loop - e.window = {:s}" ),
                                SDL_WindowEventID_to_string( static_cast< SDL_WindowEventID >( e.window.event ) ) );
          switch( e.window.event ) {
            case SDL_WindowEventID::SDL_WINDOWEVENT_CLOSE:
              SDL_HideWindow( this->sdlWindow_ );
              done = true;
              break;
            case SDL_WindowEventID::SDL_WINDOWEVENT_HIDDEN:
              SDL_HideWindow( this->sdlWindow_ );
              break;
            case SDL_WindowEventID::SDL_WINDOWEVENT_SHOWN:
              SDL_ShowWindow( this->sdlWindow_ );
              break;
            default:
              break;
          }
          break;
        case SDL_EventType::SDL_KEYDOWN:
          break;
        case SDL_EventType::SDL_KEYUP:
          break;
        case SDL_EventType::SDL_TEXTEDITING:
          break;
        case SDL_EventType::SDL_TEXTEDITING_EXT:
          break;
        case SDL_EventType::SDL_TEXTINPUT:
          break;
        case SDL_EventType::SDL_MOUSEMOTION:
          break;
        case SDL_EventType::SDL_MOUSEBUTTONDOWN:
          break;
        case SDL_EventType::SDL_MOUSEBUTTONUP:
          break;
        case SDL_EventType::SDL_MOUSEWHEEL:
          break;
        case SDL_EventType::SDL_FINGERMOTION:
          break;
        case SDL_EventType::SDL_FINGERDOWN:
          break;
        case SDL_EventType::SDL_FINGERUP:
          break;
        case SDL_EventType::SDL_DROPBEGIN:
          break;
        case SDL_EventType::SDL_DROPFILE:
          break;
        case SDL_EventType::SDL_DROPTEXT:
          break;
        case SDL_EventType::SDL_DROPCOMPLETE:
          break;
        default:
          if( e.type <= SDL_EventType::SDL_USEREVENT && e.type < SDL_EventType::SDL_LASTEVENT ) {
          }
          break;
      }
      this->scriptManager_->input_update( e );

      sdlInputQueueMutex_.lock();
      hasInputs = !sdlInputQueue_.empty();
      sdlInputQueueMutex_.unlock();
    }

    this->performanceController_->incInputLoops();
  }
  this->sdlRenderer_->signal_quit();
  this->logicController_->signal_quit();
  this->sdlEngine_->destroy_window( this->sdlWindowId_ );

  this->logger_->trace( fmt::runtime( "run_input_loop()~" ) );
}

SDL_Window* SFG::Engine::SdlWindow::get_sdl_window() const {
  // this->logger_->trace( fmt::runtime( "get_sdl_window()" ) );

  // this->logger_->trace( fmt::runtime( "get_sdl_window()~" ) );
  return this->sdlWindow_;
}

uint32_t SFG::Engine::SdlWindow::get_sdl_window_id() const {
  // this->logger_->trace( fmt::runtime( "get_sdl_window_id()" ) );

  // this->logger_->trace( fmt::runtime( "get_sdl_window_id()~" ) );
  return this->sdlWindowId_;
}

SFG::Engine::PerformanceController* SFG::Engine::SdlWindow::get_performance_controller() const {
  // this->logger_->trace( fmt::runtime( "get_performance_controller()" ) );

  // this->logger_->trace( fmt::runtime( "get_performance_controller()~" ) );
  return this->performanceController_;
}

SFG::Engine::ScriptManager* SFG::Engine::SdlWindow::get_script_manager() const {
  // this->logger_->trace( fmt::runtime( "get_script_manager()" ) );

  // this->logger_->trace( fmt::runtime( "get_script_manager()~" ) );
  return this->scriptManager_;
}

SFG::Engine::SdlWindowRenderer* SFG::Engine::SdlWindow::get_window_renderer() const {
  // this->logger_->trace( fmt::runtime( "get_window_renderer()" ) );

  // this->logger_->trace( fmt::runtime( "get_window_renderer()~" ) );
  return this->sdlRenderer_;
}

SFG::Engine::LogicController* SFG::Engine::SdlWindow::get_logic_controller() const {
  // this->logger_->trace( fmt::runtime( "get_logic_controller()" ) );

  // this->logger_->trace( fmt::runtime( "get_logic_controller()~" ) );
  return this->logicController_;
}

std::string SFG::Engine::SdlWindow::get_title() const {
  this->logger_->trace( fmt::runtime( "get_title()" ) );

  this->logger_->trace( fmt::runtime( "get_title()~" ) );
  return this->title_;
}

void SFG::Engine::SdlWindow::set_title( std::string const& new_title ) {
  this->logger_->trace( fmt::runtime( "set_title( new_title = \"{:s}\" )" ), new_title );

  this->title_ = new_title;
  if( this->sdlWindow_ ) {
    SDL_SetWindowTitle( this->sdlWindow_, this->title_.c_str() );
  }

  this->logger_->trace( fmt::runtime( "set_title()~" ) );
}

uint32_t SFG::Engine::SdlWindow::get_x() const {
  this->logger_->trace( fmt::runtime( "get_x()" ) );

  this->logger_->trace( fmt::runtime( "get_x()~" ) );
  return this->x_;
}

void SFG::Engine::SdlWindow::set_x( uint32_t new_x ) {
  this->logger_->trace( fmt::runtime( "set_x( new_x = {:d} )" ), new_x );

  this->x_ = new_x;
  if( this->sdlWindow_ ) {
    SDL_SetWindowPosition( this->sdlWindow_, this->x_, this->y_ );
  }

  this->logger_->trace( fmt::runtime( "set_x()~" ) );
}

uint32_t SFG::Engine::SdlWindow::get_y() const {
  this->logger_->trace( fmt::runtime( "get_y()" ) );

  this->logger_->trace( fmt::runtime( "get_y()~" ) );
  return this->y_;
}

void SFG::Engine::SdlWindow::set_y( uint32_t new_y ) {
  this->logger_->trace( fmt::runtime( "set_y( new_y = {:d} )" ), new_y );

  this->y_ = new_y;
  if( this->sdlWindow_ ) {
    SDL_SetWindowPosition( this->sdlWindow_, this->x_, this->y_ );
  }

  this->logger_->trace( fmt::runtime( "set_y()~" ) );
}

uint32_t SFG::Engine::SdlWindow::get_width() const {
  this->logger_->trace( fmt::runtime( "get_width()" ) );

  this->logger_->trace( fmt::runtime( "get_width()~" ) );
  return this->width_;
}

void SFG::Engine::SdlWindow::set_width( uint32_t new_width ) {
  this->logger_->trace( fmt::runtime( "set_width( new_width = {:d} )" ), new_width );

  this->width_ = new_width;
  if( this->sdlWindow_ ) {
    SDL_SetWindowSize( this->sdlWindow_, this->width_, this->height_ );
  }

  this->logger_->trace( fmt::runtime( "set_width()~" ) );
}

uint32_t SFG::Engine::SdlWindow::get_height() const {
  this->logger_->trace( fmt::runtime( "get_height()" ) );

  this->logger_->trace( fmt::runtime( "get_height()~" ) );
  return this->height_;
}

void SFG::Engine::SdlWindow::set_height( uint32_t new_height ) {
  this->logger_->trace( fmt::runtime( "set_height( new_height = {:d} )" ), new_height );

  this->height_ = new_height;
  if( this->sdlWindow_ ) {
    SDL_SetWindowSize( this->sdlWindow_, this->width_, this->height_ );
  }

  this->logger_->trace( fmt::runtime( "set_height()~" ) );
}

SDL_WindowFlags SFG::Engine::SdlWindow::get_flags() {
  this->logger_->trace( fmt::runtime( "get_flags()" ) );

  if( this->sdlWindow_ ) {
    this->flags_ = static_cast< SDL_WindowFlags >( SDL_GetWindowFlags( this->sdlWindow_ ) );
  }

  this->logger_->trace( fmt::runtime( "get_flags()~" ) );
  return this->flags_;
}

void SFG::Engine::SdlWindow::set_flags( SDL_WindowFlags new_flags ) {
  this->logger_->trace( fmt::runtime( "set_flags( new_flags = 0b{:0>32b} )" ), static_cast< uint32_t >( new_flags ) );

  if( this->sdlWindow_ ) {
    this->flags_ = static_cast< SDL_WindowFlags >( SDL_GetWindowFlags( this->sdlWindow_ ) );

    SDL_WindowFlags changedFlags = static_cast< SDL_WindowFlags >( this->flags_ ^ new_flags );
    this->logger_->trace( fmt::runtime( "set_flags - changedFlags = 0b{:0>32b}" ), static_cast< uint32_t >( changedFlags ) );

    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_FULLSCREEN ) == SDL_WindowFlags::SDL_WINDOW_FULLSCREEN ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_FULLSCREEN ) {
        SDL_SetWindowFullscreen( this->sdlWindow_, SDL_WindowFlags::SDL_WINDOW_FULLSCREEN );
      } else {
        SDL_SetWindowFullscreen( this->sdlWindow_, 0 );
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_OPENGL ) == SDL_WindowFlags::SDL_WINDOW_OPENGL ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_OPENGL ) {
      } else {
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_SHOWN ) == SDL_WindowFlags::SDL_WINDOW_SHOWN ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_SHOWN ) {
        SDL_ShowWindow( this->sdlWindow_ );
      } else {
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_HIDDEN ) == SDL_WindowFlags::SDL_WINDOW_HIDDEN ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_HIDDEN ) {
        SDL_HideWindow( this->sdlWindow_ );
      } else {
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_BORDERLESS ) == SDL_WindowFlags::SDL_WINDOW_BORDERLESS ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_BORDERLESS ) {
        SDL_SetWindowBordered( this->sdlWindow_, SDL_bool::SDL_FALSE );
      } else {
        SDL_SetWindowBordered( this->sdlWindow_, SDL_bool::SDL_TRUE );
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_RESIZABLE ) == SDL_WindowFlags::SDL_WINDOW_RESIZABLE ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_RESIZABLE ) {
        SDL_SetWindowResizable( this->sdlWindow_, SDL_bool::SDL_TRUE );
      } else {
        SDL_SetWindowResizable( this->sdlWindow_, SDL_bool::SDL_FALSE );
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_MINIMIZED ) == SDL_WindowFlags::SDL_WINDOW_MINIMIZED ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_MINIMIZED ) {
        SDL_MinimizeWindow( this->sdlWindow_ );
      } else {
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_MAXIMIZED ) == SDL_WindowFlags::SDL_WINDOW_MAXIMIZED ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_MAXIMIZED ) {
        SDL_MaximizeWindow( this->sdlWindow_ );
      } else {
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_MOUSE_GRABBED ) == SDL_WindowFlags::SDL_WINDOW_MOUSE_GRABBED ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_MOUSE_GRABBED ) {
        SDL_SetWindowMouseGrab( this->sdlWindow_, SDL_bool::SDL_TRUE );
      } else {
        SDL_SetWindowMouseGrab( this->sdlWindow_, SDL_bool::SDL_FALSE );
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_INPUT_FOCUS ) == SDL_WindowFlags::SDL_WINDOW_INPUT_FOCUS ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_INPUT_FOCUS ) {
        SDL_SetWindowInputFocus( this->sdlWindow_ );
      } else {
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_MOUSE_FOCUS ) == SDL_WindowFlags::SDL_WINDOW_MOUSE_FOCUS ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_MOUSE_FOCUS ) {
        SDL_SetWindowInputFocus( this->sdlWindow_ );
      } else {
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_FULLSCREEN_DESKTOP ) == SDL_WindowFlags::SDL_WINDOW_FULLSCREEN_DESKTOP ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_FULLSCREEN_DESKTOP ) {
        SDL_SetWindowFullscreen( this->sdlWindow_, SDL_WindowFlags::SDL_WINDOW_FULLSCREEN_DESKTOP );
      } else {
        SDL_SetWindowFullscreen( this->sdlWindow_, 0 );
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_FOREIGN ) == SDL_WindowFlags::SDL_WINDOW_FOREIGN ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_FOREIGN ) {
      } else {
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI ) == SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI ) {
      } else {
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_MOUSE_CAPTURE ) == SDL_WindowFlags::SDL_WINDOW_MOUSE_CAPTURE ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_MOUSE_CAPTURE ) {
        SDL_CaptureMouse( SDL_bool::SDL_TRUE );
      } else {
        SDL_CaptureMouse( SDL_bool::SDL_FALSE );
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_ALWAYS_ON_TOP ) == SDL_WindowFlags::SDL_WINDOW_ALWAYS_ON_TOP ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_ALWAYS_ON_TOP ) {
        SDL_SetWindowAlwaysOnTop( this->sdlWindow_, SDL_bool::SDL_TRUE );
      } else {
        SDL_SetWindowAlwaysOnTop( this->sdlWindow_, SDL_bool::SDL_FALSE );
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_SKIP_TASKBAR ) == SDL_WindowFlags::SDL_WINDOW_SKIP_TASKBAR ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_SKIP_TASKBAR ) {
      } else {
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_UTILITY ) == SDL_WindowFlags::SDL_WINDOW_UTILITY ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_UTILITY ) {
      } else {
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_TOOLTIP ) == SDL_WindowFlags::SDL_WINDOW_TOOLTIP ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_TOOLTIP ) {
      } else {
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_POPUP_MENU ) == SDL_WindowFlags::SDL_WINDOW_POPUP_MENU ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_POPUP_MENU ) {
      } else {
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_KEYBOARD_GRABBED ) == SDL_WindowFlags::SDL_WINDOW_KEYBOARD_GRABBED ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_KEYBOARD_GRABBED ) {
        SDL_SetWindowKeyboardGrab( this->sdlWindow_, SDL_bool::SDL_TRUE );
      } else {
        SDL_SetWindowKeyboardGrab( this->sdlWindow_, SDL_bool::SDL_FALSE );
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_VULKAN ) == SDL_WindowFlags::SDL_WINDOW_VULKAN ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_VULKAN ) {
      } else {
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_METAL ) == SDL_WindowFlags::SDL_WINDOW_METAL ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_METAL ) {
      } else {
      }
    }
    if( ( changedFlags & SDL_WindowFlags::SDL_WINDOW_INPUT_GRABBED ) == SDL_WindowFlags::SDL_WINDOW_INPUT_GRABBED ) {
      if( new_flags & SDL_WindowFlags::SDL_WINDOW_INPUT_GRABBED ) {
        SDL_SetWindowGrab( this->sdlWindow_, SDL_bool::SDL_TRUE );
      } else {
        SDL_SetWindowGrab( this->sdlWindow_, SDL_bool::SDL_FALSE );
      }
    }

    if( this->sdlWindow_ ) {
      this->flags_ = static_cast< SDL_WindowFlags >( SDL_GetWindowFlags( this->sdlWindow_ ) );
    }
  } else {
    this->flags_ = new_flags;
  }

  this->logger_->trace( fmt::runtime( "set_flags()~" ) );
}
