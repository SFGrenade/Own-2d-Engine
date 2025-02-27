#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SFG/Own2dEngine/Engine/audioManager.h>
#include <SFG/Own2dEngine/Engine/performance.h>
#include <SFG/Own2dEngine/Engine/rendererManager.h>
#include <SFG/Own2dEngine/Engine/windowManager.h>
#include <SFG/Own2dEngine/Logger/_include.h>
#include <SFG/Own2dEngine/Utils/_include.h>
#include <hedley/hedley.h>
#include <iostream>
#include <string>
#include <vector>

namespace SFGO2DL = SFG::Own2dEngine::Logger;
namespace SFGO2DE = SFG::Own2dEngine::Engine;

int main( int argc, char** argv ) noexcept {
  int better_main( std::vector< std::string > const& ) noexcept;
  std::vector< std::string > args( argv, std::next( argv, static_cast< std::ptrdiff_t >( argc ) ) );
  return better_main( args );
}

int better_main( std::vector< std::string > const& args ) noexcept {
  std::locale::global( std::locale( "" ) );
  SFGO2DL::LoggerFactory::init( "engine.log", false );
  spdlog::trace( "better_main( args = {:c}\"{:s}\"{:c} )", '{', fmt::join( args, "\", \"" ), '}' );

  SFGO2DE::Performance::init();
  SFGO2DE::Performance::startProgram();

  // SFGO2DL::LoggerFactory::get_logger( "Performance" )->set_level( spdlog::level::level_enum::debug );

  SFGO2DE::AudioManager::init();
  SFGO2DE::AudioManager::GetAudioInfos();
  SFGO2DE::AudioManager::Start();

  int sdlErrorCode;
  spdlog::trace( "better_main - initializing sdl" );
  if( ( sdlErrorCode = SDL_Init( SDL_INIT_EVERYTHING ) ) != 0 ) {
    spdlog::error( "better_main - error initializing sdl: {:d} ({:s})", sdlErrorCode, SDL_GetError() );
  }
  if( ( sdlErrorCode = IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP | IMG_INIT_JXL | IMG_INIT_AVIF ) ) == 0 ) {
    spdlog::error( "better_main - error initializing sdl_image: {:d} ({:s})", sdlErrorCode, SDL_GetError() );
  }
  if( ( sdlErrorCode = Mix_Init( MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS | MIX_INIT_WAVPACK ) ) == 0 ) {
    spdlog::error( "better_main - error initializing sdl_mixer: {:d} ({:s})", sdlErrorCode, SDL_GetError() );
  }
  if( ( sdlErrorCode = TTF_Init() ) != 0 ) {
    spdlog::error( "better_main - error initializing sdl_ttf: {:d} ({:s})", sdlErrorCode, SDL_GetError() );
  }
  spdlog::trace( "better_main - initialized sdl" );

  SFGO2DE::RendererManager::init();
  SFGO2DE::WindowManager::init();

  SFGO2DE::RendererManager::GetRendererInfos();

  int x = SDL_WINDOWPOS_CENTERED;
  int y = SDL_WINDOWPOS_CENTERED;
  int w = 320;
  int h = 180;
  SDL_WindowFlags flags = static_cast< SDL_WindowFlags >( 0 );

  // has to be in main thread
  SDL_Window* mainWindow = SFGO2DE::WindowManager::AddWindow( "REGULAR", x, y, w, h, flags );

  uint64_t frameCounter = 0;

  // has to be in thread that does drawing
  SDL_Renderer* mainWindowRenderer = SFGO2DE::RendererManager::CreateRenderer(
      mainWindow, [&frameCounter]( SDL_Renderer* renderer ) { frameCounter++; }, SDL_RENDERER_ACCELERATED, "default" );
  if( mainWindowRenderer == nullptr ) {
    spdlog::error( "better_main - error calling SDL_CreateRenderer: {:s}", SDL_GetError() );
  }
  SDL_ShowWindow( mainWindow );

  SFGO2DE::RendererManager::GetRendererInfos( mainWindowRenderer );

  bool running = true;
  SDL_Event event;
  while( running ) {
    SFGO2DE::Performance::startTiming( "SdlEvents" );
    while( SDL_PollEvent( &event ) != 0 ) {
      if( event.type == SDL_QUIT ) {
        running = false;
        break;
      } else if( event.type == SDL_WINDOWEVENT ) {
        SFGO2DE::WindowManager::RunEvent( event.window );
      }
    }
    SFGO2DE::Performance::endTiming( "SdlEvents" );
    if( !running ) {
      // you know, we don't need to go through rendering if all windows are closed anyway
      break;
    }
  }

  SFGO2DE::RendererManager::Shutdown();
  SFGO2DE::WindowManager::Shutdown();

  spdlog::trace( "better_main - frameCounter: {:d}", frameCounter );

  TTF_Quit();
  Mix_Quit();
  IMG_Quit();
  SDL_Quit();

  SFGO2DE::AudioManager::Shutdown();

  SFGO2DE::Performance::endProgram();

  spdlog::trace( "better_main()~" );
  SFGO2DL::LoggerFactory::deinit();
  return EXIT_SUCCESS;
}
