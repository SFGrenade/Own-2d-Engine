#pragma once

// Including our headers
#include "globals.h"
#include "graphicshandler/graphicshandler.h"

namespace SFG {
class Window {
  private:
  static spdlogger logger;
  static int xPos;
  static int yPos;
  static int width;
  static int height;
  static std::string windowTitle;
  static std::shared_ptr< SDL_Window > window;
  static std::shared_ptr< GraphicsHandler > graphicsHandler;

  static void deleteGraphicsHandler( GraphicsHandler* ptr );
  static void deleteWindow( SDL_Window* ptr );

  public:
  static void SetSize( int width, int height );
  static void Initialize();
  static void Destroy();

  static bool InitializeSDL();
  static bool InitializeWindow();
  static bool ShowWindow();
  static std::shared_ptr< SDL_Window > GetSdlWindow();
  static std::shared_ptr< GraphicsHandler > GetGraphicsHandler();
};
}  // namespace SFG
