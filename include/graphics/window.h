#pragma once

#include <string>

// Including our headers
#include "_globals/sdlInclude.h"
#include "_globals/spdlogInclude.h"
#include "graphics/graphicshandler.h"

namespace SFG {
class Window {
  private:
  void deleteGraphicsHandler( GraphicsHandler* ptr );
  void deleteWindow( SDL_Window* ptr );

  public:
  Window();
  ~Window();
  void SetSize( int width, int height );

  bool InitializeSDL();
  bool InitializeWindow();
  bool ShowWindow();
  std::shared_ptr< SDL_Window > GetSdlWindow();
  std::shared_ptr< GraphicsHandler > GetGraphicsHandler();

  private:
  spdlogger logger_;
  int xPos_;
  int yPos_;
  int width_;
  int height_;
  std::string windowTitle_;
  std::shared_ptr< SDL_Window > window_;
  std::shared_ptr< GraphicsHandler > graphicsHandler_;
};
}  // namespace SFG
