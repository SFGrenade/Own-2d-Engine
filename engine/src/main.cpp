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
  spdlog::trace( "better_main( args = [{:n}] )", args );

  SFGO2DE::Performance::init();
  SFGO2DE::Performance::startProgram();

  // SFGO2DL::LoggerFactory::get_logger( "Performance" )->set_level( spdlog::level::level_enum::debug );

  SFGO2DE::AudioManager::init();
  SFGO2DE::AudioManager::GetAudioInfos();
  SFGO2DE::AudioManager::Start();

  /*
  SFGO2DE::AudioManager::CreateAudio( "Background Music",
                                      SFGO2DE::AudioManager::AudioType::BGM,
                                      1,
                                      std::vector< SFGO2DE::AudioStreamType >{
                                          static_cast< SFGO2DE::AudioStreamType >( -16384 ),       static_cast< SFGO2DE::AudioStreamType >( -16086.11364 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -15788.22727 ), static_cast< SFGO2DE::AudioStreamType >( -15490.34091 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -15192.45455 ), static_cast< SFGO2DE::AudioStreamType >( -14894.56818 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -14596.68182 ), static_cast< SFGO2DE::AudioStreamType >( -14298.79545 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -14000.90909 ), static_cast< SFGO2DE::AudioStreamType >( -13703.02273 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -13405.13636 ), static_cast< SFGO2DE::AudioStreamType >( -13107.25 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -12809.36364 ), static_cast< SFGO2DE::AudioStreamType >( -12511.47727 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -12213.59091 ), static_cast< SFGO2DE::AudioStreamType >( -11915.70455 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -11617.81818 ), static_cast< SFGO2DE::AudioStreamType >( -11319.93182 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -11022.04545 ), static_cast< SFGO2DE::AudioStreamType >( -10724.15909 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -10426.27273 ), static_cast< SFGO2DE::AudioStreamType >( -10128.38636 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -9830.5 ),      static_cast< SFGO2DE::AudioStreamType >( -9532.613636 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -9234.727273 ), static_cast< SFGO2DE::AudioStreamType >( -8936.840909 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -8638.954545 ), static_cast< SFGO2DE::AudioStreamType >( -8341.068182 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -8043.181818 ), static_cast< SFGO2DE::AudioStreamType >( -7745.295455 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -7447.409091 ), static_cast< SFGO2DE::AudioStreamType >( -7149.522727 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -6851.636364 ), static_cast< SFGO2DE::AudioStreamType >( -6553.75 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -6255.863636 ), static_cast< SFGO2DE::AudioStreamType >( -5957.977273 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -5660.090909 ), static_cast< SFGO2DE::AudioStreamType >( -5362.204545 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -5064.318182 ), static_cast< SFGO2DE::AudioStreamType >( -4766.431818 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -4468.545455 ), static_cast< SFGO2DE::AudioStreamType >( -4170.659091 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -3872.772727 ), static_cast< SFGO2DE::AudioStreamType >( -3574.886364 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -3277 ),        static_cast< SFGO2DE::AudioStreamType >( -2979.113636 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -2681.227273 ), static_cast< SFGO2DE::AudioStreamType >( -2383.340909 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -2085.454545 ), static_cast< SFGO2DE::AudioStreamType >( -1787.568182 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -1489.681818 ), static_cast< SFGO2DE::AudioStreamType >( -1191.795455 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -893.9090909 ), static_cast< SFGO2DE::AudioStreamType >( -596.0227273 ),
                                          static_cast< SFGO2DE::AudioStreamType >( -298.1363636 ), static_cast< SFGO2DE::AudioStreamType >( -0.25 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 297.6363636 ),  static_cast< SFGO2DE::AudioStreamType >( 595.5227273 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 893.4090909 ),  static_cast< SFGO2DE::AudioStreamType >( 1191.295455 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 1489.181818 ),  static_cast< SFGO2DE::AudioStreamType >( 1787.068182 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 2084.954545 ),  static_cast< SFGO2DE::AudioStreamType >( 2382.840909 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 2680.727273 ),  static_cast< SFGO2DE::AudioStreamType >( 2978.613636 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 3276.5 ),       static_cast< SFGO2DE::AudioStreamType >( 3574.386364 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 3872.272727 ),  static_cast< SFGO2DE::AudioStreamType >( 4170.159091 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 4468.045455 ),  static_cast< SFGO2DE::AudioStreamType >( 4765.931818 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 5063.818182 ),  static_cast< SFGO2DE::AudioStreamType >( 5361.704545 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 5659.590909 ),  static_cast< SFGO2DE::AudioStreamType >( 5957.477273 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 6255.363636 ),  static_cast< SFGO2DE::AudioStreamType >( 6553.25 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 6851.136364 ),  static_cast< SFGO2DE::AudioStreamType >( 7149.022727 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 7446.909091 ),  static_cast< SFGO2DE::AudioStreamType >( 7744.795455 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 8042.681818 ),  static_cast< SFGO2DE::AudioStreamType >( 8340.568182 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 8638.454545 ),  static_cast< SFGO2DE::AudioStreamType >( 8936.340909 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 9234.227273 ),  static_cast< SFGO2DE::AudioStreamType >( 9532.113636 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 9830 ),         static_cast< SFGO2DE::AudioStreamType >( 10127.88636 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 10425.77273 ),  static_cast< SFGO2DE::AudioStreamType >( 10723.65909 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 11021.54545 ),  static_cast< SFGO2DE::AudioStreamType >( 11319.43182 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 11617.31818 ),  static_cast< SFGO2DE::AudioStreamType >( 11915.20455 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 12213.09091 ),  static_cast< SFGO2DE::AudioStreamType >( 12510.97727 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 12808.86364 ),  static_cast< SFGO2DE::AudioStreamType >( 13106.75 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 13404.63636 ),  static_cast< SFGO2DE::AudioStreamType >( 13702.52273 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 14000.40909 ),  static_cast< SFGO2DE::AudioStreamType >( 14298.29545 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 14596.18182 ),  static_cast< SFGO2DE::AudioStreamType >( 14894.06818 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 15191.95455 ),  static_cast< SFGO2DE::AudioStreamType >( 15489.84091 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 15787.72727 ),  static_cast< SFGO2DE::AudioStreamType >( 16085.61364 ),
                                          static_cast< SFGO2DE::AudioStreamType >( 16383.5 ),
                                      },
                                      44100 );
  */

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
  int w = 1600;
  int h = 900;
  SDL_WindowFlags flags = static_cast< SDL_WindowFlags >( 0 );

  // has to be in main thread
  SDL_Window* mainWindow = SFGO2DE::WindowManager::AddWindow( "REGULAR", x, y, w, h, flags );

  // has to be in thread that does drawing
  SDL_Renderer* mainWindowRenderer = SFGO2DE::RendererManager::CreateRenderer(
      mainWindow, []( SDL_Renderer* renderer ) {}, SDL_RENDERER_ACCELERATED, "default" );
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
