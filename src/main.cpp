// Include our overall Objects

#include "confighandler/confighandler.h"
#include "fonthandler/fonthandler.h"
#include "globals.h"
#include "inputhandler/inputhandler.h"
#include "logichandler/logichandler.h"
#include "networkhandler/networkhandler.h"
#include "performance/performance.h"
#include "scripthandler/CustomScripts/logscript.h"
#include "scripthandler/scripthandler.h"
#include "window/window.h"

#pragma region Debug Output

constexpr std::string const GetRendererFlags( uint32_t const rendererFlags ) {
  std::string ret = "";
  if( rendererFlags & SDL_RENDERER_SOFTWARE ) {
    ret += "SDL_RENDERER_SOFTWARE | ";
  }
  if( rendererFlags & SDL_RENDERER_ACCELERATED ) {
    ret += "SDL_RENDERER_ACCELERATED | ";
  }
  if( rendererFlags & SDL_RENDERER_PRESENTVSYNC ) {
    ret += "SDL_RENDERER_PRESENTVSYNC | ";
  }
  if( rendererFlags & SDL_RENDERER_TARGETTEXTURE ) {
    ret += "SDL_RENDERER_TARGETTEXTURE | ";
  }
  return ret.substr( 0, ret.size() - 3 );
}

constexpr std::string const GetPixelFormatEnum( uint32_t const format ) {
  if( format == SDL_PIXELFORMAT_UNKNOWN ) {
    return "SDL_PIXELFORMAT_UNKNOWN";
  }
  if( format == SDL_PIXELFORMAT_INDEX1LSB ) {
    return "SDL_PIXELFORMAT_INDEX1LSB";
  }
  if( format == SDL_PIXELFORMAT_INDEX1MSB ) {
    return "SDL_PIXELFORMAT_INDEX1MSB";
  }
  if( format == SDL_PIXELFORMAT_INDEX4LSB ) {
    return "SDL_PIXELFORMAT_INDEX4LSB";
  }
  if( format == SDL_PIXELFORMAT_INDEX4MSB ) {
    return "SDL_PIXELFORMAT_INDEX4MSB";
  }
  if( format == SDL_PIXELFORMAT_INDEX8 ) {
    return "SDL_PIXELFORMAT_INDEX8";
  }
  if( format == SDL_PIXELFORMAT_RGB332 ) {
    return "SDL_PIXELFORMAT_RGB332";
  }
  if( format == SDL_PIXELFORMAT_XRGB4444 ) {
    return "SDL_PIXELFORMAT_XRGB4444";
  }
  if( format == SDL_PIXELFORMAT_RGB444 ) {
    return "SDL_PIXELFORMAT_RGB444";
  }
  if( format == SDL_PIXELFORMAT_XBGR4444 ) {
    return "SDL_PIXELFORMAT_XBGR4444";
  }
  if( format == SDL_PIXELFORMAT_BGR444 ) {
    return "SDL_PIXELFORMAT_BGR444";
  }
  if( format == SDL_PIXELFORMAT_XRGB1555 ) {
    return "SDL_PIXELFORMAT_XRGB1555";
  }
  if( format == SDL_PIXELFORMAT_RGB555 ) {
    return "SDL_PIXELFORMAT_RGB555";
  }
  if( format == SDL_PIXELFORMAT_XBGR1555 ) {
    return "SDL_PIXELFORMAT_XBGR1555";
  }
  if( format == SDL_PIXELFORMAT_BGR555 ) {
    return "SDL_PIXELFORMAT_BGR555";
  }
  if( format == SDL_PIXELFORMAT_ARGB4444 ) {
    return "SDL_PIXELFORMAT_ARGB4444";
  }
  if( format == SDL_PIXELFORMAT_RGBA4444 ) {
    return "SDL_PIXELFORMAT_RGBA4444";
  }
  if( format == SDL_PIXELFORMAT_ABGR4444 ) {
    return "SDL_PIXELFORMAT_ABGR4444";
  }
  if( format == SDL_PIXELFORMAT_BGRA4444 ) {
    return "SDL_PIXELFORMAT_BGRA4444";
  }
  if( format == SDL_PIXELFORMAT_ARGB1555 ) {
    return "SDL_PIXELFORMAT_ARGB1555";
  }
  if( format == SDL_PIXELFORMAT_RGBA5551 ) {
    return "SDL_PIXELFORMAT_RGBA5551";
  }
  if( format == SDL_PIXELFORMAT_ABGR1555 ) {
    return "SDL_PIXELFORMAT_ABGR1555";
  }
  if( format == SDL_PIXELFORMAT_BGRA5551 ) {
    return "SDL_PIXELFORMAT_BGRA5551";
  }
  if( format == SDL_PIXELFORMAT_RGB565 ) {
    return "SDL_PIXELFORMAT_RGB565";
  }
  if( format == SDL_PIXELFORMAT_BGR565 ) {
    return "SDL_PIXELFORMAT_BGR565";
  }
  if( format == SDL_PIXELFORMAT_RGB24 ) {
    return "SDL_PIXELFORMAT_RGB24";
  }
  if( format == SDL_PIXELFORMAT_BGR24 ) {
    return "SDL_PIXELFORMAT_BGR24";
  }
  if( format == SDL_PIXELFORMAT_XRGB8888 ) {
    return "SDL_PIXELFORMAT_XRGB8888";
  }
  if( format == SDL_PIXELFORMAT_RGB888 ) {
    return "SDL_PIXELFORMAT_RGB888";
  }
  if( format == SDL_PIXELFORMAT_RGBX8888 ) {
    return "SDL_PIXELFORMAT_RGBX8888";
  }
  if( format == SDL_PIXELFORMAT_XBGR8888 ) {
    return "SDL_PIXELFORMAT_XBGR8888";
  }
  if( format == SDL_PIXELFORMAT_BGR888 ) {
    return "SDL_PIXELFORMAT_BGR888";
  }
  if( format == SDL_PIXELFORMAT_BGRX8888 ) {
    return "SDL_PIXELFORMAT_BGRX8888";
  }
  if( format == SDL_PIXELFORMAT_ARGB8888 ) {
    return "SDL_PIXELFORMAT_ARGB8888";
  }
  if( format == SDL_PIXELFORMAT_RGBA8888 ) {
    return "SDL_PIXELFORMAT_RGBA8888";
  }
  if( format == SDL_PIXELFORMAT_ABGR8888 ) {
    return "SDL_PIXELFORMAT_ABGR8888";
  }
  if( format == SDL_PIXELFORMAT_BGRA8888 ) {
    return "SDL_PIXELFORMAT_BGRA8888";
  }
  if( format == SDL_PIXELFORMAT_ARGB2101010 ) {
    return "SDL_PIXELFORMAT_ARGB2101010";
  }
  if( format == SDL_PIXELFORMAT_RGBA32 ) {
    return "SDL_PIXELFORMAT_RGBA32";
  }
  if( format == SDL_PIXELFORMAT_ARGB32 ) {
    return "SDL_PIXELFORMAT_ARGB32";
  }
  if( format == SDL_PIXELFORMAT_BGRA32 ) {
    return "SDL_PIXELFORMAT_BGRA32";
  }
  if( format == SDL_PIXELFORMAT_ABGR32 ) {
    return "SDL_PIXELFORMAT_ABGR32";
  }
  if( format == SDL_PIXELFORMAT_YV12 ) {
    return "SDL_PIXELFORMAT_YV12";
  }
  if( format == SDL_PIXELFORMAT_IYUV ) {
    return "SDL_PIXELFORMAT_IYUV";
  }
  if( format == SDL_PIXELFORMAT_YUY2 ) {
    return "SDL_PIXELFORMAT_YUY2";
  }
  if( format == SDL_PIXELFORMAT_UYVY ) {
    return "SDL_PIXELFORMAT_UYVY";
  }
  if( format == SDL_PIXELFORMAT_YVYU ) {
    return "SDL_PIXELFORMAT_YVYU";
  }
  if( format == SDL_PIXELFORMAT_NV12 ) {
    return "SDL_PIXELFORMAT_NV12";
  }
  if( format == SDL_PIXELFORMAT_NV21 ) {
    return "SDL_PIXELFORMAT_NV21";
  }
  if( format == SDL_PIXELFORMAT_EXTERNAL_OES ) {
    return "SDL_PIXELFORMAT_EXTERNAL_OES";
  }
  return "";
}

void PrintRendererInfo( SDL_RendererInfo const &rendererInfo ) {
  spdlog::debug( "- Renderer \"{:s}\":", rendererInfo.name );
  std::string tmpString1 = GetRendererFlags( rendererInfo.flags );
  spdlog::debug( "  - Flags: {:#034b} / {:s}", rendererInfo.flags, tmpString1 );
  spdlog::debug( "  - NumTextureFormats: {:d}", rendererInfo.num_texture_formats );
  spdlog::debug( "  - TextureFormats:" );
  for( uint32_t j = 0; j < rendererInfo.num_texture_formats; j++ ) {
    std::string tmpString2 = GetPixelFormatEnum( rendererInfo.texture_formats[j] );
    spdlog::debug( "    - TextureFormat {:d}: {:#034b} / {:s}", j, rendererInfo.texture_formats[j], tmpString2 );
  }
  spdlog::debug( "  - MaxTextureWidth: {:d}", rendererInfo.max_texture_width );
  spdlog::debug( "  - MaxTextureHeight: {:d}", rendererInfo.max_texture_height );
}

int CheckAndSelectRenderer( std::string_view const cliSelection ) {
  spdlog::debug( "CheckAndSelectRenderer(std::string_view const cliSelection = \"{:s}\")", cliSelection );
  int numRDevices = SDL_GetNumRenderDrivers();
  int ret = 0;
  if( numRDevices < 0 ) {
    spdlog::error( "CheckAndSelectRenderer - Error when SDL_GetNumRenderDrivers: {:s}", SDL_GetError() );
    return -1;
  }
  spdlog::debug( "CheckAndSelectRenderer - SDL_GetNumRenderDrivers() => {:d}", numRDevices );
  for( int i = 0; i < numRDevices; i++ ) {
    SDL_Renderer *renderer = SDL_CreateRenderer( SFG::Window::GetSdlWindow().get(), i, 0 );
    if( renderer == nullptr ) {
      spdlog::error( "CheckAndSelectRenderer - Error when SDL_CreateRenderer: {:s}", SDL_GetError() );
    }
    SDL_RendererInfo info;
    if( SDL_GetRendererInfo( renderer, &info ) != 0 ) {
      spdlog::error( "CheckAndSelectRenderer - Error when SDL_GetRendererInfo: {:s}", SDL_GetError() );
    }
    SDL_DestroyRenderer( renderer );
    PrintRendererInfo( info );
    if( cliSelection == info.name ) {
      ret = i;
    }
  }
  return ret;
}

#pragma endregion

#pragma region Initialization

void InitializeLoggers() {
  auto consoleSink = std::make_shared< spdlog::sinks::stdout_color_sink_mt >();
  consoleSink->set_level( SFG::ConfigHandler::get_Logging_StdOutLevel() );

  auto truncatedFileSink = std::make_shared< spdlog::sinks::basic_file_sink_mt >( "log.log", true );
  truncatedFileSink->set_level( SFG::ConfigHandler::get_Logging_LogFileLevel() );
  spdlog::sinks_init_list truncatedSinkList = { truncatedFileSink, consoleSink };

  spdlogger mainLogger = std::make_shared< spdlog::logger >( "main", truncatedSinkList.begin(), truncatedSinkList.end() );
  mainLogger->set_level( spdlog::level::trace );
  mainLogger->flush_on( spdlog::level::trace );
  spdlog::register_logger( mainLogger );
  spdlog::set_default_logger( mainLogger );

  auto normalFileSink = std::make_shared< spdlog::sinks::basic_file_sink_mt >( "log.log", false );
  normalFileSink->set_level( SFG::ConfigHandler::get_Logging_LogFileLevel() );
  spdlog::sinks_init_list normalSinkList = { normalFileSink, consoleSink };

  std::vector< std::string > allLoggerNames
      = { "ConfigHandler", "FontHandler", "GraphicsHandler", "InputHandler", "LogicHandler", "NetworkHandler", "Timer", "ScriptHandler", "LogScript", "Window" };
  for( auto name : allLoggerNames ) {
    spdlogger logger = std::make_shared< spdlog::logger >( name, normalSinkList.begin(), normalSinkList.end() );
    logger->set_level( spdlog::level::trace );
    logger->flush_on( spdlog::level::trace );
    spdlog::register_logger( logger );
  }
  // spdlog::get("LogScript")->set_level(spdlog::level::warn);
}

void InitializeConfig() {
  SFG::ConfigHandler::InitializeNoLog();
}

void InitializeComponents() {
  SFG::ConfigHandler::Initialize();
  SFG::FontHandler::Initialize();
  SFG::InputHandler::Initialize();
  SFG::NetworkHandler::Initialize();
  SFG::LogicHandler::Initialize();
  SFG::Window::Initialize();
  SFG::ScriptHandler::Initialize();
}

void InitializeNetwork() {
  SFG::NetworkHandler::SetReqRepInfo( fmt::format( "tcp://{:s}", SFG::ConfigHandler::get_Network_Host() ), SFG::ConfigHandler::get_Network_PortReqRep() );
  SFG::NetworkHandler::SetPubSubInfo( fmt::format( "tcp://{:s}", SFG::ConfigHandler::get_Network_Host() ), SFG::ConfigHandler::get_Network_PortPubSub() );
  SFG::NetworkHandler::InitializeNetwork();
}

void InitializeWindow() {
  SFG::Window::InitializeSDL();
  SFG::Window::InitializeWindow();
}

void UninitializeComponents() {
  SFG::ScriptHandler::Destroy();
  SFG::Window::Destroy();
  SFG::LogicHandler::Destroy();
  SFG::NetworkHandler::Destroy();
  SFG::InputHandler::Destroy();
  SFG::FontHandler::Destroy();
  SFG::ConfigHandler::Destroy();
}

#pragma endregion

int main( int const argc, char const *const *argv ) {
  int better_main( std::span< std::string_view const > ) noexcept;
  std::vector< std::string_view > args( argv, std::next( argv, static_cast< std::ptrdiff_t >( argc ) ) );
  return better_main( args );
}

int better_main( std::span< std::string_view const > args ) noexcept {
  InitializeConfig();
  InitializeLoggers();

  spdlog::trace( "better_main(args = {:c} \"{:s}\" {:c})", '{', fmt::join( args, "\", \"" ), '}' );

  bool quit = false;
  auto quitFlagDeleteFunction = []( bool * ) {};  // since quitFlag is on the stack
  std::shared_ptr< bool > quitPtr = std::shared_ptr< bool >( &quit, quitFlagDeleteFunction );

  InitializeComponents();
  InitializeNetwork();
  SFG::Window::SetSize( SFG::ConfigHandler::get_Window_Width(), SFG::ConfigHandler::get_Window_Height() );
  InitializeWindow();

  int rendererIndex = CheckAndSelectRenderer( SFG::ConfigHandler::get_Rendering_Renderer() );

  // Delete the current window and stuff to clean up contexts that make multithreading not work
  SFG::Window::Destroy();
  // Reinitialize the SDL library and window
  InitializeWindow();
  std::shared_ptr< SFG::GraphicsHandler > graphicsHandler = SFG::Window::GetGraphicsHandler();
  graphicsHandler->SetRendererIndex( rendererIndex );

  std::string performanceString;
  bool makeNewPerformanceTexture = false;
  SDL_Texture *performanceTexture = nullptr;
  SDL_Rect performanceTextureRect;
  performanceTextureRect.x = 5;
  performanceTextureRect.y = 5;
  performanceTextureRect.w = 0;
  performanceTextureRect.h = 0;
  std::vector< std::string > fgCol = str_split( SFG::ConfigHandler::get_Rendering_PerformanceStringTextColor(), " " );
  std::vector< std::string > bgCol = str_split( SFG::ConfigHandler::get_Rendering_PerformanceStringBackgroundColor(), " " );
  SDL_Color foregroundColor;
  foregroundColor.r = std::stoi( fgCol[0] );
  foregroundColor.g = std::stoi( fgCol[1] );
  foregroundColor.b = std::stoi( fgCol[2] );
  foregroundColor.a = std::stoi( fgCol[3] );
  SDL_Color backgroundColor;
  backgroundColor.r = std::stoi( bgCol[0] );
  backgroundColor.g = std::stoi( bgCol[1] );
  backgroundColor.b = std::stoi( bgCol[2] );
  backgroundColor.a = std::stoi( bgCol[3] );
  graphicsHandler->RegisterDrawEvent( []( std::shared_ptr< SDL_Renderer > /*windowRenderer*/ ) { SFG::ScriptHandler::UpdateScriptsFrame(); } );
  graphicsHandler->RegisterDrawEvent( [&performanceString, &makeNewPerformanceTexture, &performanceTexture, &performanceTextureRect, foregroundColor, backgroundColor]( std::shared_ptr< SDL_Renderer > windowRenderer ) {
    if( makeNewPerformanceTexture ) {
      SDL_Surface *textSurface
          = TTF_RenderUTF8_Shaded_Wrapped( SFG::FontHandler::GetFont( SFG::FontType::Console ).get(), performanceString.c_str(), foregroundColor, backgroundColor, 0 );
      if( performanceTexture ) {
        SDL_DestroyTexture( performanceTexture );
      }
      performanceTexture = SDL_CreateTextureFromSurface( windowRenderer.get(), textSurface );
      performanceTextureRect.w = textSurface->w;
      performanceTextureRect.h = textSurface->h;
      SDL_FreeSurface( textSurface );
      makeNewPerformanceTexture = false;
    }
    if( performanceTexture ) {
      if( SDL_RenderCopy( windowRenderer.get(), performanceTexture, NULL, &performanceTextureRect ) != 0 ) {
        spdlog::error( "graphicsHandler->draw - Error when SDL_RenderCopy: {:s}", SDL_GetError() );
      }
    }
  } );
  graphicsHandler->SetQuitFlag( quitPtr );

  SFG::InputHandler::RegisterQuitEvent( [&quit]() { quit = true; } );
  SFG::InputHandler::RegisterWindowEvent( [&quit]( SDL_WindowEvent &window ) { quit |= window.event == SDL_WINDOWEVENT_CLOSE; } );
  SFG::InputHandler::RegisterKeyDownEvent( [&quit]( SDL_KeyboardEvent &key ) { quit |= key.keysym.sym == SFG::ConfigHandler::get_Input_StopGameKey(); } );

  SFG::LogicHandler::AddTimer(
      []( std::optional< std::chrono::secondsLongDouble > /*interval*/ ) {
        // 50 hz test timer
        SFG::ScriptHandler::UpdateScriptsLogicFrame();
      },
      std::chrono::duration_cast< std::chrono::nanoseconds >( 1.0s / SFG::ConfigHandler::get_Logic_LogicInterval() ),
      false );
  SFG::LogicHandler::AddTimer(
      []( std::optional< std::chrono::secondsLongDouble > /*interval*/ ) {
        // 50 hz network timer
        SFG::NetworkHandler::RunNetwork();
      },
      std::chrono::duration_cast< std::chrono::nanoseconds >( 1.0s / SFG::ConfigHandler::get_Logic_NetworkInterval() ),
      false );
  SFG::LogicHandler::AddTimer(
      [&performanceString, &makeNewPerformanceTexture]( std::optional< std::chrono::secondsLongDouble > /*interval*/ ) {
        performanceString = fmt::format( R"(Performance (per second):
{:>20d} Frames drawn
{:>20d} Input checks
{:>20d} Logic loops)",
                                         static_cast< uint64_t >( SFG::Performance::GetGraphicsLoop() * SFG::ConfigHandler::get_Logic_PerformanceInterval() ),
                                         static_cast< uint64_t >( SFG::Performance::GetInputLoop() * SFG::ConfigHandler::get_Logic_PerformanceInterval() ),
                                         static_cast< uint64_t >( SFG::Performance::GetLogicLoop() * SFG::ConfigHandler::get_Logic_PerformanceInterval() ) );
        spdlog::debug( performanceString );
        SFG::NetworkHandler::SendMessageRequest( "TestUser", performanceString );
        makeNewPerformanceTexture = true;
      },
      std::chrono::duration_cast< std::chrono::nanoseconds >( 1.0s / SFG::ConfigHandler::get_Logic_PerformanceInterval() ),
      false );
  SFG::LogicHandler::SetQuitFlag( quitPtr );

  // SFG::ScriptHandler::AddScript< SFG::LogScript >();

  SFG::Window::ShowWindow();

  graphicsHandler->StartDraw();
  SFG::LogicHandler::StartLogic();

  // inputs have to be checked in the main thread
  while( !quit ) {
    SFG::InputHandler::CheckInputs();
    SFG::Performance::AddInputLoop();
  }

  SFG::LogicHandler::StopLogic();
  graphicsHandler->StopDraw();
  if( performanceTexture ) {
    SDL_DestroyTexture( performanceTexture );
  }

  UninitializeComponents();

  spdlog::trace( "better_main()~" );
  return EXIT_SUCCESS;
}
