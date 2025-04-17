#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SFG/Own2dEngine/Game/audioManager.h>
#include <SFG/Own2dEngine/Game/inputManager.h>
#include <SFG/Own2dEngine/Game/options.h>
#include <SFG/Own2dEngine/Game/performance.h>
#include <SFG/Own2dEngine/Game/ui/button.h>
#include <SFG/Own2dEngine/Game/ui/label.h>
#include <SFG/Own2dEngine/Game/ui/widget.h>
#include <SFG/Own2dEngine/Logger/_include.h>
#include <SFG/Own2dEngine/Utils/_include.h>
#include <hedley/hedley.h>
#include <iostream>
#include <string>
#include <vector>

namespace SFGO2DL = SFG::Own2dEngine::Logger;
namespace SFGO2DG = SFG::Own2dEngine::Game;
namespace SFGO2DU = SFG::Own2dEngine::Utils;

int main( int argc, char** argv ) noexcept {
  int better_main( std::vector< std::string > const& ) noexcept;
  std::vector< std::string > args( argv, std::next( argv, static_cast< std::ptrdiff_t >( argc ) ) );
  return better_main( args );
}

int better_main( std::vector< std::string > const& args ) noexcept {
  std::locale::global( std::locale( "" ) );
  SFGO2DL::LoggerFactory::init( "game.log", false );
  spdlog::trace( "better_main( args = [{:n}] )", args );

  SFGO2DG::Performance::init();
  SFGO2DG::Performance::startProgram();

  // SFGO2DL::LoggerFactory::get_logger( "Performance" )->set_level( spdlog::level::level_enum::debug );

#pragma region Initialize Audio
  SFGO2DG::AudioManager::init();
  SFGO2DG::AudioManager::GetAudioInfos();
#pragma endregion Initialize Audio

#pragma region Initialize Options
  SFGO2DG::Options* options = new SFGO2DG::Options();
  options->SetAudioDevices( SFGO2DG::AudioManager::TestABunchOfShit() );
// having this later would be funny
// options->SetAudioIndexes( -1, -1 );
#pragma endregion Initialize Options

#pragma region Initialize SDL
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
#pragma endregion Initialize SDL

  std::vector< SFGO2DG::UI::Widget* > widgetsToCycle;
  int32_t widgetIndex = 0;
  std::function< void( int32_t ) > buttonCallback = [&widgetIndex, &widgetsToCycle]( int32_t newIndex ) {
    newIndex = SFGO2DU::mod< int32_t >( newIndex, widgetsToCycle.size() );
    widgetsToCycle[widgetIndex]->SetVisible( false );
    widgetsToCycle[newIndex]->SetVisible( true );
    widgetIndex = newIndex;
  };

#pragma region UI elements
  SFGO2DG::UI::Widget* rootWidget = new SFGO2DG::UI::Widget( nullptr, SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f } );

  {
    SFGO2DG::UI::Widget* mainMenuPage = new SFGO2DG::UI::Widget( rootWidget, SDL_FRect{ 0.0f, 0.0f, 1.0f, 1.0f } );
    SFGO2DG::UI::Widget* optionsPage = new SFGO2DG::UI::Widget( rootWidget, SDL_FRect{ 0.0f, 0.0f, 1.0f, 1.0f } );

    widgetsToCycle.push_back( mainMenuPage );
    widgetsToCycle.push_back( optionsPage );

#pragma region Main menu UI
    {
      SFGO2DG::UI::Label* tmp = new SFGO2DG::UI::Label( "Big-ass Title", mainMenuPage, SDL_FRect{ 0.25f, 0.125f, 0.5f, 0.25f } );
      tmp->SetHorizontalAlignment( SFGO2DG::UI::Label::HorizontalAlignment::Centered );
      tmp->SetVerticalAlignment( SFGO2DG::UI::Label::VerticalAlignment::Centered );
      tmp->SetFontFile( "Resources/Fonts/NotoSerif-Regular.ttf" );
      tmp->SetFontSize( 72 );
      tmp->SetFontColourActive( SDL_Color{ 0x40, 0xC0, 0xff, 0xff } );
      tmp->SetFontColourInactive( SDL_Color{ 0x40, 0xC0, 0xff, 0x80 } );
    }
    float mainMenuButtonHeight = 0.0625f;
    float mainMenuYOffset = 0.5f;
    {
      std::function< void() > func = [&buttonCallback]() {
        spdlog::debug( "PLAY GAME PRESSED!" );
        // buttonCallback( 0 );
      };
      SFGO2DG::UI::Button* tmp = new SFGO2DG::UI::Button( func, "Play Game", mainMenuPage, SDL_FRect{ 0.375f, mainMenuYOffset, 0.25f, mainMenuButtonHeight } );
      tmp->SetHorizontalAlignment( SFGO2DG::UI::Label::HorizontalAlignment::Centered );
      tmp->SetVerticalAlignment( SFGO2DG::UI::Label::VerticalAlignment::Centered );
    }
    mainMenuYOffset += mainMenuButtonHeight;
    mainMenuYOffset += mainMenuButtonHeight;
    {
      std::function< void() > func = [&buttonCallback]() {
        spdlog::debug( "TO OPTIONS MENU" );
        buttonCallback( 1 );
      };
      SFGO2DG::UI::Button* tmp = new SFGO2DG::UI::Button( func, "Options", mainMenuPage, SDL_FRect{ 0.375f, mainMenuYOffset, 0.25f, mainMenuButtonHeight } );
      tmp->SetHorizontalAlignment( SFGO2DG::UI::Label::HorizontalAlignment::Centered );
      tmp->SetVerticalAlignment( SFGO2DG::UI::Label::VerticalAlignment::Centered );
    }
#pragma endregion Main menu UI

#pragma region Options UI
    float optionsMenuButtonHeight = 0.0625f;
    float optionsMenuYOffset = 0.5f;
    SFGO2DG::UI::Label* audioInputLabel;
    SFGO2DG::UI::Label* audioOutputLabel;
    {
      std::function< void() > func = [options]() { options->AudioSelectPrevInput(); };
      SFGO2DG::UI::Button* tmp = new SFGO2DG::UI::Button( func, "<", optionsPage, SDL_FRect{ 0.25f, optionsMenuYOffset, 0.0625f, optionsMenuButtonHeight } );
      tmp->SetHorizontalAlignment( SFGO2DG::UI::Label::HorizontalAlignment::Centered );
      tmp->SetVerticalAlignment( SFGO2DG::UI::Label::VerticalAlignment::Centered );
    }
    {
      audioInputLabel = new SFGO2DG::UI::Label( "Input Device Name", optionsPage, SDL_FRect{ 0.3125f, optionsMenuYOffset, 0.375f, optionsMenuButtonHeight } );
      audioInputLabel->SetHorizontalAlignment( SFGO2DG::UI::Label::HorizontalAlignment::Centered );
      audioInputLabel->SetVerticalAlignment( SFGO2DG::UI::Label::VerticalAlignment::Centered );
    }
    {
      std::function< void() > func = [options]() { options->AudioSelectNextInput(); };
      SFGO2DG::UI::Button* tmp = new SFGO2DG::UI::Button( func, ">", optionsPage, SDL_FRect{ 0.6875f, optionsMenuYOffset, 0.0625f, optionsMenuButtonHeight } );
      tmp->SetHorizontalAlignment( SFGO2DG::UI::Label::HorizontalAlignment::Centered );
      tmp->SetVerticalAlignment( SFGO2DG::UI::Label::VerticalAlignment::Centered );
    }
    optionsMenuYOffset += optionsMenuButtonHeight;
    {
      std::function< void() > func = [options]() { options->AudioSelectPrevOutput(); };
      SFGO2DG::UI::Button* tmp = new SFGO2DG::UI::Button( func, "<", optionsPage, SDL_FRect{ 0.25f, optionsMenuYOffset, 0.0625f, optionsMenuButtonHeight } );
      tmp->SetHorizontalAlignment( SFGO2DG::UI::Label::HorizontalAlignment::Centered );
      tmp->SetVerticalAlignment( SFGO2DG::UI::Label::VerticalAlignment::Centered );
    }
    {
      audioOutputLabel = new SFGO2DG::UI::Label( "Output Device Name", optionsPage, SDL_FRect{ 0.3125f, optionsMenuYOffset, 0.375f, optionsMenuButtonHeight } );
      audioOutputLabel->SetHorizontalAlignment( SFGO2DG::UI::Label::HorizontalAlignment::Centered );
      audioOutputLabel->SetVerticalAlignment( SFGO2DG::UI::Label::VerticalAlignment::Centered );
    }
    {
      std::function< void() > func = [options]() { options->AudioSelectNextOutput(); };
      SFGO2DG::UI::Button* tmp = new SFGO2DG::UI::Button( func, ">", optionsPage, SDL_FRect{ 0.6875f, optionsMenuYOffset, 0.0625f, optionsMenuButtonHeight } );
      tmp->SetHorizontalAlignment( SFGO2DG::UI::Label::HorizontalAlignment::Centered );
      tmp->SetVerticalAlignment( SFGO2DG::UI::Label::VerticalAlignment::Centered );
    }
    optionsMenuYOffset += optionsMenuButtonHeight;
    {
      std::function< void() > func = [&buttonCallback]() {
        spdlog::debug( "TO MAIN MENU" );
        buttonCallback( 0 );
      };
      SFGO2DG::UI::Button* tmp = new SFGO2DG::UI::Button( func, "Back", optionsPage, SDL_FRect{ 0.375f, optionsMenuYOffset, 0.25f, optionsMenuButtonHeight } );
      tmp->SetHorizontalAlignment( SFGO2DG::UI::Label::HorizontalAlignment::Centered );
      tmp->SetVerticalAlignment( SFGO2DG::UI::Label::VerticalAlignment::Centered );
    }
    options->SetAudioDisplayLabels( audioInputLabel, audioOutputLabel );
#pragma endregion Options UI

    for( int i = 0; i < widgetsToCycle.size(); i++ ) {
      buttonCallback( i );
    }
    // select first index
    buttonCallback( 0 );
  }
#pragma endregion UI elements

  SFGO2DG::InputManager::init();

  int x = SDL_WINDOWPOS_CENTERED;
  int y = SDL_WINDOWPOS_CENTERED;
  int w = 1600;
  int h = 900;
  SDL_WindowFlags flags = static_cast< SDL_WindowFlags >( SDL_WINDOW_RESIZABLE );

  SDL_Window* mainWindow = SDL_CreateWindow( "GAME", x, y, w, h, flags );
  if( mainWindow == nullptr ) {
    spdlog::error( "better_main - error calling SDL_CreateWindow: {:s}", SDL_GetError() );
  }
  SDL_Renderer* mainWindowRenderer = SDL_CreateRenderer( mainWindow, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED );
  if( mainWindowRenderer == nullptr ) {
    spdlog::error( "better_main - error calling SDL_CreateWindow: {:s}", SDL_GetError() );
  }
  SDL_SetRenderDrawBlendMode( mainWindowRenderer, SDL_BlendMode::SDL_BLENDMODE_BLEND );

#pragma region Rest of options
  // rest of options?
  options->SetAudioIndexes( -1, -1 );
#pragma endregion Rest of options

  SDL_ShowWindow( mainWindow );

  bool running = true;
  SDL_Event event;
  while( running ) {
#pragma region Inputs
    SFGO2DG::Performance::startTiming( "SdlEvents" );
    while( SDL_PollEvent( &event ) != 0 ) {
      if( event.type == SDL_QUIT ) {
        running = false;
        break;
      } else if( event.type == SDL_WINDOWEVENT ) {
        if( event.window.event == SDL_WINDOWEVENT_CLOSE ) {
          SDL_HideWindow( mainWindow );
          SDL_DestroyRenderer( mainWindowRenderer );
          SDL_DestroyWindow( mainWindow );
        } else if( event.window.event == SDL_WINDOWEVENT_HIDDEN ) {
          SDL_HideWindow( mainWindow );
        } else if( event.window.event == SDL_WINDOWEVENT_SHOWN ) {
          SDL_ShowWindow( mainWindow );
        }
      } else if( ( event.type == SDL_KEYDOWN ) || ( event.type == SDL_KEYUP ) ) {
        SFGO2DG::InputManager::ProcessKeyEvent( event.key );
      } else if( event.type == SDL_MOUSEMOTION ) {
        SFGO2DG::InputManager::ProcessMouseMoveEvent( event.motion );
      } else if( ( event.type == SDL_MOUSEBUTTONDOWN ) || ( event.type == SDL_MOUSEBUTTONUP ) ) {
        SFGO2DG::InputManager::ProcessMouseButtonEvent( event.button );
      } else if( event.type == SDL_MOUSEWHEEL ) {
        SFGO2DG::InputManager::ProcessMouseWheelEvent( event.wheel );
      }
    }
    SFGO2DG::Performance::endTiming( "SdlEvents" );
    if( !running ) {
      // you know, we don't need to go through rendering if all windows are closed anyway
      break;
    }
#pragma endregion Inputs

#pragma region Logic
    SFGO2DG::InputManager::OnLogicUpdate_Early();

    {
      int winW;
      int winH;
      SDL_GetWindowSize( mainWindow, &winW, &winH );
      rootWidget->SetW( winW );
      rootWidget->SetH( winH );
    }
    // other logic
    rootWidget->OnLogic();

    SFGO2DG::InputManager::OnLogicUpdate_Late();
#pragma endregion Logic

#pragma region Rendering
    SFGO2DG::Performance::startTiming( "Rendering" );

    SDL_SetRenderDrawColor( mainWindowRenderer, 0x00, 0x00, 0x00, 0xff );
    SDL_RenderClear( mainWindowRenderer );

    // actually draw stuff here
    rootWidget->OnRender( mainWindowRenderer );

    SDL_RenderPresent( mainWindowRenderer );

    SFGO2DG::Performance::endTiming( "Rendering" );
#pragma endregion Rendering
  }
  SDL_HideWindow( mainWindow );
  SDL_DestroyRenderer( mainWindowRenderer );
  SDL_DestroyWindow( mainWindow );

#pragma region Destroy UI elements

  delete rootWidget;

#pragma endregion Destroy UI elements

#pragma region Quit SDL

  TTF_Quit();
  Mix_Quit();
  IMG_Quit();
  SDL_Quit();

#pragma endregion Quit SDL

  SFGO2DG::AudioManager::Shutdown();
  delete options;

  SFGO2DG::Performance::endProgram();

  spdlog::trace( "better_main()~" );
  SFGO2DL::LoggerFactory::deinit();
  return EXIT_SUCCESS;
}
