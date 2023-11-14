#include "graphics/window.h"

#include "input/confighandler.h"

#define SDL_IMG_INIT_EVERYTHING ( IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP )

#define SDL_MIX_INIT_EVERYTHING ( MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS )
#define SDL_MIX_INIT_MP3_OGG ( MIX_INIT_MP3 | MIX_INIT_OGG )

namespace SFG {
spdlogger Window::logger = nullptr;
int Window::xPos = SDL_WINDOWPOS_CENTERED;
int Window::yPos = SDL_WINDOWPOS_CENTERED;
int Window::width = 800;
int Window::height = 600;
std::string Window::windowTitle = "Own 2D Engine - Exit with ESC";
std::shared_ptr< SDL_Window > Window::window = std::shared_ptr< SDL_Window >();
std::shared_ptr< GraphicsHandler > Window::graphicsHandler = std::shared_ptr< GraphicsHandler >();

void Window::deleteGraphicsHandler( GraphicsHandler* ptr ) {
  Window::logger->trace( fmt::runtime( "deleteGraphicsHandler({:p})" ), static_cast< void* >( ptr ) );
  delete ptr;
  Window::logger->trace( fmt::runtime( "deleteGraphicsHandler()~" ) );
}

void Window::deleteWindow( SDL_Window* ptr ) {
  Window::logger->trace( fmt::runtime( "deleteWindow({:p})" ), static_cast< void* >( ptr ) );
  SDL_DestroyWindow( ptr );
  Window::logger->trace( fmt::runtime( "deleteWindow()~" ) );
}

void Window::SetSize( int newWidth, int newHeight ) {
  Window::logger->trace( fmt::runtime( "SetSize({:d}, {:d})" ), newWidth, newHeight );
  Window::width = newWidth;
  Window::height = newHeight;
  Window::logger->trace( fmt::runtime( "SetSize()~" ) );
}

void Window::Initialize() {
  Window::logger = spdlog::get( "Window" );
  Window::logger->trace( fmt::runtime( "Initialize()" ) );
  Window::logger->trace( fmt::runtime( "Initialize()~" ) );
}

void Window::Destroy() {
  Window::logger->trace( fmt::runtime( "Destroy()" ) );
  Window::graphicsHandler.reset();
  Window::window.reset();
  Mix_CloseAudio();
  Mix_Quit();
  IMG_Quit();
  SDL_Quit();
  Window::logger->trace( fmt::runtime( "Destroy()~" ) );
}

bool Window::InitializeSDL() {
  Window::logger->trace( fmt::runtime( "InitializeSDL()" ) );
  if( SDL_Init( SDL_INIT_EVERYTHING ) != 0 ) {
    Window::logger->error( fmt::runtime( "SDL initialize error! {:s}" ), SDL_GetError() );
    Window::logger->trace( fmt::runtime( "InitializeSDL()~" ) );
    return false;
  }
  if( IMG_Init( IMG_INIT_PNG ) != IMG_INIT_PNG ) {
    Window::logger->error( fmt::runtime( "SDL_image initialize error! {:s}" ), IMG_GetError() );
    Window::logger->trace( fmt::runtime( "InitializeSDL()~" ) );
    return false;
  }
  if( ( Mix_Init( SDL_MIX_INIT_MP3_OGG ) != SDL_MIX_INIT_MP3_OGG )
      || Mix_OpenAudio( ConfigHandler::get_Audio_SampleRate(),
                        ConfigHandler::get_Audio_Format(),
                        ConfigHandler::get_Audio_Channels(),
                        ConfigHandler::get_Audio_ChunkSize() )
             < 0 ) {
    Window::logger->error( fmt::runtime( "SDL_mixer initialize error! {:s}" ), Mix_GetError() );
    Window::logger->trace( fmt::runtime( "InitializeSDL()~" ) );
    return false;
  }
  Window::logger->trace( fmt::runtime( "InitializeSDL()~" ) );
  return true;
}

bool Window::InitializeWindow() {
  Window::logger->trace( fmt::runtime( "InitializeWindow()" ) );
  Window::window = std::shared_ptr< SDL_Window >( SDL_CreateWindow( Window::windowTitle.c_str(),
                                                                    Window::xPos,
                                                                    Window::yPos,
                                                                    Window::width,
                                                                    Window::height,
                                                                    SDL_WINDOW_HIDDEN ),
                                                  deleteWindow );
  if( !Window::window ) {
    Window::logger->error( fmt::runtime( "Window creation error! {:s}" ), SDL_GetError() );
    Window::logger->trace( fmt::runtime( "InitializeWindow()~" ) );
    return false;
  }
  Window::graphicsHandler = std::shared_ptr< GraphicsHandler >( new GraphicsHandler( Window::window ), deleteGraphicsHandler );
  Window::logger->trace( fmt::runtime( "InitializeWindow()~" ) );
  return true;
}

bool Window::ShowWindow() {
  Window::logger->trace( fmt::runtime( "ShowWindow()" ) );
  SDL_ShowWindow( Window::window.get() );
  Window::logger->trace( fmt::runtime( "ShowWindow()~" ) );
  return true;
}

std::shared_ptr< SDL_Window > Window::GetSdlWindow() {
  Window::logger->trace( fmt::runtime( "GetSdlWindow()" ) );
  Window::logger->trace( fmt::runtime( "GetSdlWindow()~" ) );
  return Window::window;
}

std::shared_ptr< GraphicsHandler > Window::GetGraphicsHandler() {
  Window::logger->trace( fmt::runtime( "GetGraphicsHandler()" ) );
  Window::logger->trace( fmt::runtime( "GetGraphicsHandler()~" ) );
  return Window::graphicsHandler;
}
}  // namespace SFG
