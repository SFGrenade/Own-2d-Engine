#include "input/inputhandler.h"

namespace SFG {
spdlogger InputHandler::logger = nullptr;
SDL_Event InputHandler::sdlEvent;
std::vector< KeyCallback > InputHandler::keyDownCallbacks = std::vector< KeyCallback >();
std::vector< KeyCallback > InputHandler::keyUpCallbacks = std::vector< KeyCallback >();
std::vector< MouseButtonCallback > InputHandler::mouseButtonCallbacks = std::vector< MouseButtonCallback >();
std::vector< MouseMotionCallback > InputHandler::mouseMotionCallbacks = std::vector< MouseMotionCallback >();
std::vector< MouseWheelCallback > InputHandler::mouseWheelCallbacks = std::vector< MouseWheelCallback >();
std::vector< QuitEventCallback > InputHandler::quitEventCallbacks = std::vector< QuitEventCallback >();
std::vector< WindowEventCallback > InputHandler::windowEventCallbacks = std::vector< WindowEventCallback >();

void InputHandler::Initialize() {
  InputHandler::logger = spdlog::get( "InputHandler" );
  InputHandler::logger->trace( fmt::runtime( "Initialize()" ) );
  InputHandler::logger->trace( fmt::runtime( "Initialize()~" ) );
}

void InputHandler::Destroy() {
  InputHandler::logger->trace( fmt::runtime( "Destroy()" ) );
  InputHandler::logger->trace( fmt::runtime( "Destroy()~" ) );
}

void InputHandler::CheckInputs() {
  // todo: too many log lines
  // InputHandler::logger->trace( fmt::runtime( "CheckInputs()" ) );
  while( SDL_PollEvent( &InputHandler::sdlEvent ) != 0 ) {
    switch( InputHandler::sdlEvent.type ) {
      case SDL_QUIT:
        for( auto callback : InputHandler::quitEventCallbacks )
          callback();
        break;
      case SDL_WINDOWEVENT:
        for( auto callback : InputHandler::windowEventCallbacks )
          callback( InputHandler::sdlEvent.window );
        break;
      case SDL_KEYDOWN:
        for( auto callback : InputHandler::keyDownCallbacks )
          callback( InputHandler::sdlEvent.key );
        break;
      case SDL_KEYUP:
        for( auto callback : InputHandler::keyUpCallbacks )
          callback( InputHandler::sdlEvent.key );
        break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        for( auto callback : InputHandler::mouseButtonCallbacks )
          callback( InputHandler::sdlEvent.button );
        break;
      case SDL_MOUSEMOTION:
        for( auto callback : InputHandler::mouseMotionCallbacks )
          callback( InputHandler::sdlEvent.motion );
        break;
      case SDL_MOUSEWHEEL:
        for( auto callback : InputHandler::mouseWheelCallbacks )
          callback( InputHandler::sdlEvent.wheel );
        break;
      default:
        break;
    }
  }
  // todo: too many log lines
  // InputHandler::logger->trace( fmt::runtime( "CheckInputs()~" ) );
}

void InputHandler::RegisterKeyDownEvent( KeyCallback callback ) {
  InputHandler::logger->trace( fmt::runtime( "RegisterKeyDownEvent(KeyCallback callback)" ) );
  if( callback )
    InputHandler::keyDownCallbacks.push_back( callback );
  InputHandler::logger->trace( fmt::runtime( "RegisterKeyDownEvent()~" ) );
}

void InputHandler::RegisterKeyUpEvent( KeyCallback callback ) {
  InputHandler::logger->trace( fmt::runtime( "RegisterKeyUpEvent(KeyCallback callback)" ) );
  if( callback )
    InputHandler::keyUpCallbacks.push_back( callback );
  InputHandler::logger->trace( fmt::runtime( "RegisterKeyUpEvent()~" ) );
}

void InputHandler::RegisterMouseButtonEvent( MouseButtonCallback callback ) {
  InputHandler::logger->trace( fmt::runtime( "RegisterMouseButtonEvent(MouseButtonCallback callback)" ) );
  if( callback )
    InputHandler::mouseButtonCallbacks.push_back( callback );
  InputHandler::logger->trace( fmt::runtime( "RegisterMouseButtonEvent()~" ) );
}

void InputHandler::RegisterMouseMotionEvent( MouseMotionCallback callback ) {
  InputHandler::logger->trace( fmt::runtime( "RegisterMouseMotionEvent(MouseMotionCallback callback)" ) );
  if( callback )
    InputHandler::mouseMotionCallbacks.push_back( callback );
  InputHandler::logger->trace( fmt::runtime( "RegisterMouseMotionEvent()~" ) );
}

void InputHandler::RegisterMouseWheelEvent( MouseWheelCallback callback ) {
  InputHandler::logger->trace( fmt::runtime( "RegisterMouseWheelEvent(MouseWheelCallback callback)" ) );
  if( callback )
    InputHandler::mouseWheelCallbacks.push_back( callback );
  InputHandler::logger->trace( fmt::runtime( "RegisterMouseWheelEvent()~" ) );
}

void InputHandler::RegisterQuitEvent( QuitEventCallback callback ) {
  InputHandler::logger->trace( fmt::runtime( "RegisterQuitEvent(QuitEventCallback callback)" ) );
  if( callback )
    InputHandler::quitEventCallbacks.push_back( callback );
  InputHandler::logger->trace( fmt::runtime( "RegisterQuitEvent()~" ) );
}

void InputHandler::RegisterWindowEvent( WindowEventCallback callback ) {
  InputHandler::logger->trace( fmt::runtime( "RegisterWindowEvent(WindowEventCallback callback)" ) );
  if( callback )
    InputHandler::windowEventCallbacks.push_back( callback );
  InputHandler::logger->trace( fmt::runtime( "RegisterWindowEvent()~" ) );
}
}  // namespace SFG
