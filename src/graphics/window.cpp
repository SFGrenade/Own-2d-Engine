#include "graphics/window.h"

#include "confighandler/confighandler.h"

#define SDL_IMG_INIT_EVERYTHING ( IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP )

#define SDL_MIX_INIT_EVERYTHING ( MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS )
#define SDL_MIX_INIT_MP3_OGG ( MIX_INIT_MP3 | MIX_INIT_OGG )

namespace SFG {
void Window::deleteGraphicsHandler( GraphicsHandler* ptr ) {
  this->logger_->trace( fmt::runtime( "deleteGraphicsHandler({:p})" ), static_cast< void* >( ptr ) );
  delete ptr;
  this->logger_->trace( fmt::runtime( "deleteGraphicsHandler()~" ) );
}

void Window::deleteWindow( SDL_Window* ptr ) {
  this->logger_->trace( fmt::runtime( "deleteWindow({:p})" ), static_cast< void* >( ptr ) );
  SDL_DestroyWindow( ptr );
  this->logger_->trace( fmt::runtime( "deleteWindow()~" ) );
}

Window::Window()
    : logger_( spdlog::get( "Window" ) ),
      xPos_( SDL_WINDOWPOS_CENTERED ),
      yPos_( SDL_WINDOWPOS_CENTERED ),
      width_( 800 ),
      height_( 600 ),
      windowTitle_( "Own 2D Engine - Exit with ESC" ),
      window_( std::shared_ptr< SDL_Window >() ),
      graphicsHandler_( std::shared_ptr< GraphicsHandler >() ) {
  this->logger_->trace( fmt::runtime( "Window()" ) );
  this->logger_->trace( fmt::runtime( "Window()~" ) );
}

Window::~Window() {
  this->logger_->trace( fmt::runtime( "~Window()" ) );

  this->graphicsHandler_.reset();
  this->window_.reset();
  Mix_CloseAudio();
  Mix_Quit();
  IMG_Quit();
  SDL_Quit();

  this->logger_->trace( fmt::runtime( "~Window()~" ) );
}

void Window::SetSize( int newWidth, int newHeight ) {
  this->logger_->trace( fmt::runtime( "SetSize({:d}, {:d})" ), newWidth, newHeight );
  this->width_ = newWidth;
  this->height_ = newHeight;
  this->logger_->trace( fmt::runtime( "SetSize()~" ) );
}

bool Window::InitializeSDL() {
  this->logger_->trace( fmt::runtime( "InitializeSDL()" ) );
  if( SDL_Init( SDL_INIT_EVERYTHING ) != 0 ) {
    this->logger_->error( fmt::runtime( "SDL initialize error! {:s}" ), SDL_GetError() );
    this->logger_->trace( fmt::runtime( "InitializeSDL()~" ) );
    return false;
  }
  if( IMG_Init( IMG_INIT_PNG ) != IMG_INIT_PNG ) {
    this->logger_->error( fmt::runtime( "SDL_image initialize error! {:s}" ), IMG_GetError() );
    this->logger_->trace( fmt::runtime( "InitializeSDL()~" ) );
    return false;
  }
  if( ( Mix_Init( SDL_MIX_INIT_MP3_OGG ) != SDL_MIX_INIT_MP3_OGG )
      || Mix_OpenAudio( ConfigHandler::get_Audio_SampleRate(),
                        ConfigHandler::get_Audio_Format(),
                        ConfigHandler::get_Audio_Channels(),
                        ConfigHandler::get_Audio_ChunkSize() )
             < 0 ) {
    this->logger_->error( fmt::runtime( "SDL_mixer initialize error! {:s}" ), Mix_GetError() );
    this->logger_->trace( fmt::runtime( "InitializeSDL()~" ) );
    return false;
  }
  this->logger_->trace( fmt::runtime( "InitializeSDL()~" ) );
  return true;
}

bool Window::InitializeWindow() {
  this->logger_->trace( fmt::runtime( "InitializeWindow()" ) );
  this->window_
      = std::shared_ptr< SDL_Window >( SDL_CreateWindow( this->windowTitle_.c_str(), this->xPos_, this->yPos_, this->width_, this->height_, SDL_WINDOW_HIDDEN ),
                                       [this]( SDL_Window* ptr ) { deleteWindow( ptr ); } );
  if( !this->window_ ) {
    this->logger_->error( fmt::runtime( "Window creation error! {:s}" ), SDL_GetError() );
    this->logger_->trace( fmt::runtime( "InitializeWindow()~" ) );
    return false;
  }
  this->graphicsHandler_
      = std::shared_ptr< GraphicsHandler >( new GraphicsHandler( this->window_ ), [this]( GraphicsHandler* ptr ) { deleteGraphicsHandler( ptr ); } );
  this->logger_->trace( fmt::runtime( "InitializeWindow()~" ) );
  return true;
}

bool Window::ShowWindow() {
  this->logger_->trace( fmt::runtime( "ShowWindow()" ) );
  SDL_ShowWindow( this->window_.get() );
  this->logger_->trace( fmt::runtime( "ShowWindow()~" ) );
  return true;
}

std::shared_ptr< SDL_Window > Window::GetSdlWindow() {
  // this->logger_->trace( fmt::runtime( "GetSdlWindow()" ) );
  // this->logger_->trace( fmt::runtime( "GetSdlWindow()~" ) );
  return this->window_;
}

std::shared_ptr< GraphicsHandler > Window::GetGraphicsHandler() {
  // this->logger_->trace( fmt::runtime( "GetGraphicsHandler()" ) );
  // this->logger_->trace( fmt::runtime( "GetGraphicsHandler()~" ) );
  return this->graphicsHandler_;
}
}  // namespace SFG
