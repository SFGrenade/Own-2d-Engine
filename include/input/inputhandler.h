#pragma once

#include <functional>
#include <vector>

// Including our headers
#include "_globals/sdlInclude.h"
#include "_globals/spdlogInclude.h"

namespace SFG {
typedef std::function< void( SDL_KeyboardEvent const& key ) > KeyCallback;
typedef std::function< void( SDL_MouseButtonEvent const& button ) > MouseButtonCallback;
typedef std::function< void( SDL_MouseMotionEvent const& motion ) > MouseMotionCallback;
typedef std::function< void( SDL_MouseWheelEvent const& wheel ) > MouseWheelCallback;
typedef std::function< void() > QuitEventCallback;
typedef std::function< void( SDL_WindowEvent const& window ) > WindowEventCallback;

class InputHandler {
  public:
  InputHandler();
  ~InputHandler();

  void CheckInputs();
  void RegisterKeyDownEvent( KeyCallback callback );
  void RegisterKeyUpEvent( KeyCallback callback );
  void RegisterMouseButtonEvent( MouseButtonCallback callback );
  void RegisterMouseMotionEvent( MouseMotionCallback callback );
  void RegisterMouseWheelEvent( MouseWheelCallback callback );
  void RegisterQuitEvent( QuitEventCallback callback );
  void RegisterWindowEvent( WindowEventCallback callback );

  private:
  spdlogger logger_;
  SDL_Event sdlEvent_;
  std::vector< KeyCallback > keyDownCallbacks_;
  std::vector< KeyCallback > keyUpCallbacks_;
  std::vector< MouseButtonCallback > mouseButtonCallbacks_;
  std::vector< MouseMotionCallback > mouseMotionCallbacks_;
  std::vector< MouseWheelCallback > mouseWheelCallbacks_;
  std::vector< QuitEventCallback > quitEventCallbacks_;
  std::vector< WindowEventCallback > windowEventCallbacks_;
};
}  // namespace SFG
