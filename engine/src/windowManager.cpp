#include "SFG/Own2dEngine/Engine/windowManager.h"

#include "SFG/Own2dEngine/Engine/rendererManager.h"

namespace SFG {
namespace Own2dEngine {
namespace Engine {

SFG::Own2dEngine::Logger::spdlogger WindowManager::logger_ = nullptr;
std::map< uint32_t, SDL_Window* > WindowManager::windows_;

void WindowManager::init() {
  WindowManager::logger_ = SFG::Own2dEngine::Logger::LoggerFactory::get_logger( "WindowManager" );
}

SDL_Window* WindowManager::AddWindow( std::string const& title, int x, int y, int w, int h, SDL_WindowFlags flags ) {
  WindowManager::logger_
      ->trace( "AddWindow( title: {:?}, x: {:d}, y: {:d}, w: {:d}, h: {:d}, flags: 0x{:0>8x} )", title, x, y, w, h, static_cast< int >( flags ) );

  SDL_Window* window = SDL_CreateWindow( title.c_str(), x, y, w, h, flags );
  if( window == nullptr ) {
    WindowManager::logger_->error( "error calling SDL_CreateWindow: {:s}", SDL_GetError() );
    return nullptr;
  }
  uint32_t windowId = SDL_GetWindowID( window );
  if( windowId == 0 ) {
    WindowManager::logger_->error( "error calling SDL_GetWindowID: {:s}", SDL_GetError() );
    SDL_DestroyWindow( window );
    return nullptr;
  }
  WindowManager::windows_.emplace( SDL_GetWindowID( window ), window );

  return window;
}

void WindowManager::RunEvent( SDL_WindowEvent const& event ) {
  uint32_t windowId = event.windowID;
  if( !WindowManager::windows_.contains( windowId ) ) {
    WindowManager::logger_->warn( "RunEvent - window id {:d} not found in active windows, ignoring", windowId );
    return;
  }
  SDL_Window* window = WindowManager::windows_.at( windowId );

  if( event.event == SDL_WINDOWEVENT_CLOSE ) {
    RendererManager::DestroyRendererForWindow( window );
    SDL_HideWindow( window );
    SDL_DestroyWindow( window );
    WindowManager::windows_.erase( windowId );
  } else if( event.event == SDL_WINDOWEVENT_HIDDEN ) {
    SDL_HideWindow( window );
  } else if( event.event == SDL_WINDOWEVENT_SHOWN ) {
    SDL_ShowWindow( window );
  }
}

void WindowManager::Shutdown() {
  WindowManager::logger_->trace( "Shutdown()" );

  for( std::pair< uint32_t, SDL_Window* > const& pair : WindowManager::windows_ ) {
    RendererManager::DestroyRendererForWindow( pair.second );
    SDL_HideWindow( pair.second );
    SDL_DestroyWindow( pair.second );
  }
  WindowManager::windows_.clear();
}

}  // namespace Engine
}  // namespace Own2dEngine
}  // namespace SFG
