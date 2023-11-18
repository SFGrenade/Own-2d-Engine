#include "input/inputhandler.h"

namespace SFG {
InputHandler::InputHandler()
    : logger_( spdlog::get( "InputHandler" ) ),
      sdlEvent_(),
      keyDownCallbacks_(),
      keyUpCallbacks_(),
      mouseButtonCallbacks_(),
      mouseMotionCallbacks_(),
      mouseWheelCallbacks_(),
      quitEventCallbacks_(),
      windowEventCallbacks_() {
  this->logger_->trace( fmt::runtime( "InputHandler()" ) );

  this->logger_->trace( fmt::runtime( "InputHandler()~" ) );
}

InputHandler::~InputHandler() {
  this->logger_->trace( fmt::runtime( "~InputHandler()" ) );

  this->logger_->trace( fmt::runtime( "~InputHandler()~" ) );
}

void InputHandler::CheckInputs() {
  // this->logger_->trace( fmt::runtime( "CheckInputs()" ) );

  while( SDL_PollEvent( &sdlEvent_ ) != 0 ) {
    switch( sdlEvent_.type ) {
      case SDL_QUIT:
        for( auto callback : this->quitEventCallbacks_ )
          callback();
        break;
      case SDL_WINDOWEVENT:
        for( auto callback : this->windowEventCallbacks_ )
          callback( sdlEvent_.window );
        break;
      case SDL_KEYDOWN:
        for( auto callback : this->keyDownCallbacks_ )
          callback( sdlEvent_.key );
        break;
      case SDL_KEYUP:
        for( auto callback : this->keyUpCallbacks_ )
          callback( sdlEvent_.key );
        break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        for( auto callback : this->mouseButtonCallbacks_ )
          callback( sdlEvent_.button );
        break;
      case SDL_MOUSEMOTION:
        for( auto callback : this->mouseMotionCallbacks_ )
          callback( sdlEvent_.motion );
        break;
      case SDL_MOUSEWHEEL:
        for( auto callback : this->mouseWheelCallbacks_ )
          callback( sdlEvent_.wheel );
        break;
      default:
        break;
    }
  }

  // this->logger_->trace( fmt::runtime( "CheckInputs()~" ) );
}

void InputHandler::RegisterKeyDownEvent( KeyCallback callback ) {
  this->logger_->trace( fmt::runtime( "RegisterKeyDownEvent(KeyCallback callback)" ) );

  if( callback )
    this->keyDownCallbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "RegisterKeyDownEvent()~" ) );
}

void InputHandler::RegisterKeyUpEvent( KeyCallback callback ) {
  this->logger_->trace( fmt::runtime( "RegisterKeyUpEvent(KeyCallback callback)" ) );

  if( callback )
    this->keyUpCallbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "RegisterKeyUpEvent()~" ) );
}

void InputHandler::RegisterMouseButtonEvent( MouseButtonCallback callback ) {
  this->logger_->trace( fmt::runtime( "RegisterMouseButtonEvent(MouseButtonCallback callback)" ) );

  if( callback )
    this->mouseButtonCallbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "RegisterMouseButtonEvent()~" ) );
}

void InputHandler::RegisterMouseMotionEvent( MouseMotionCallback callback ) {
  this->logger_->trace( fmt::runtime( "RegisterMouseMotionEvent(MouseMotionCallback callback)" ) );

  if( callback )
    this->mouseMotionCallbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "RegisterMouseMotionEvent()~" ) );
}

void InputHandler::RegisterMouseWheelEvent( MouseWheelCallback callback ) {
  this->logger_->trace( fmt::runtime( "RegisterMouseWheelEvent(MouseWheelCallback callback)" ) );

  if( callback )
    this->mouseWheelCallbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "RegisterMouseWheelEvent()~" ) );
}

void InputHandler::RegisterQuitEvent( QuitEventCallback callback ) {
  this->logger_->trace( fmt::runtime( "RegisterQuitEvent(QuitEventCallback callback)" ) );

  if( callback )
    this->quitEventCallbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "RegisterQuitEvent()~" ) );
}

void InputHandler::RegisterWindowEvent( WindowEventCallback callback ) {
  this->logger_->trace( fmt::runtime( "RegisterWindowEvent(WindowEventCallback callback)" ) );

  if( callback )
    this->windowEventCallbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "RegisterWindowEvent()~" ) );
}
}  // namespace SFG
