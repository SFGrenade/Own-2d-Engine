#include <chrono>
#include <span>
#include <string_view>
#include <thread>
#include <vector>

#include "_globals/moreChrono.h"
#include "_globals/spdlogInclude.h"
#include "content/scripts/debuginfo.h"
#include "content/scripts/logscript.h"
#include "content/scripts/player.h"
#include "content/scripts/wall.h"
#include "engine/scriptmanager.h"
#include "engine/sdlengine.h"
#include "engine/sdlwindow.h"


void InitializeLoggers() {
  auto consoleSink = std::make_shared< spdlog::sinks::stdout_color_sink_mt >();
  consoleSink->set_level( spdlog::level::warn );

  auto truncatedFileSink = std::make_shared< spdlog::sinks::basic_file_sink_mt >( "log.log", true );
  truncatedFileSink->set_level( spdlog::level::trace );
  spdlog::sinks_init_list truncatedSinkList = { truncatedFileSink, consoleSink };

  spdlogger mainLogger = std::make_shared< spdlog::logger >( "main", truncatedSinkList.begin(), truncatedSinkList.end() );
  mainLogger->set_level( spdlog::level::trace );
  mainLogger->flush_on( spdlog::level::trace );
  spdlog::register_logger( mainLogger );
  spdlog::set_default_logger( mainLogger );

  std::vector< std::string > allLoggerNames = {
      "Engine_LogicController",
      "Engine_PerformanceController",
      "Engine_ScriptManager",
      "Engine_SdlEngine",
      "Engine_SdlWindow",
      "Engine_SdlWindowRenderer",
      "Content_DebugInfo",
      "Content_LogScript",
      "Content_Player",
      "Content_Wall",
  };
  for( auto name : allLoggerNames ) {
    spdlogger logger = std::make_shared< spdlog::logger >( name, truncatedSinkList.begin(), truncatedSinkList.end() );
    logger->set_level( spdlog::level::trace );
    logger->flush_on( spdlog::level::trace );
    spdlog::register_logger( logger );
  }
  // spdlog::get("LogScript")->set_level(spdlog::level::warn);
  spdlog::set_pattern( "[%Y-%m-%d %H:%M:%S.%e] [thread %t] [%n] [%l] %v" );
}

int main( int const argc, char const* const* argv ) {
  int better_main( std::span< std::string_view const > ) noexcept;
  std::vector< std::string_view > args( argv, std::next( argv, static_cast< std::ptrdiff_t >( argc ) ) );
  return better_main( args );
}

int better_main( std::span< std::string_view const > args ) noexcept {
  InitializeLoggers();

  spdlog::trace( fmt::runtime( "better_main( args = {:c}\"{:s}\"{:c} )" ), '{', fmt::join( args, "\", \"" ), '}' );

  SFG::Engine::SdlEngine* myEngine = new SFG::Engine::SdlEngine( SDL_INIT_EVERYTHING, IMG_INIT_PNG, MIX_INIT_OGG );

  std::vector< std::string > renderers = myEngine->get_renderer_names();
  spdlog::debug( fmt::runtime( "Available renderers: {}" ), fmt::join( renderers, ", " ) );

  SFG::Engine::SdlWindow* myWindow1 = myEngine->add_window( "My Window 1", 800, 600 );
  std::thread myThread1( [myEngine, myWindow1]() {
    SDL_WindowFlags myWindow1Flags;
    myWindow1Flags = myWindow1->get_flags();
    spdlog::trace( fmt::runtime( "window 1 flags: 0b{:0>32b}" ), static_cast< uint32_t >( myWindow1Flags ) );

    myWindow1->initialize_logic_controller();
    SFG::Content::Wall* firstWall = myWindow1->get_script_manager()->add_script< SFG::Content::Wall >();
    firstWall->set_position( 200.0L, 200.0L );
    firstWall->set_size( 50.0L, 50.0L );
    spdlog::debug( fmt::runtime( "firstWall at {:p}" ), static_cast< void* >( firstWall ) );
    SFG::Content::Wall* secondWall = myWindow1->get_script_manager()->add_script< SFG::Content::Wall >();
    secondWall->set_position( 250.0L, 150.0L );
    secondWall->set_size( 50.0L, 50.0L );
    spdlog::debug( fmt::runtime( "secondWall at {:p}" ), static_cast< void* >( secondWall ) );
    SFG::Content::Player* player = myWindow1->get_script_manager()->add_script< SFG::Content::Player >();
    player->set_size( 50.0L, 50.0L );
    spdlog::debug( fmt::runtime( "player at {:p}" ), static_cast< void* >( player ) );

    myWindow1->get_script_manager()->add_script< SFG::Content::DebugInfo >();
    myWindow1->initialize_window_renderer( "direct3d", SDL_RendererFlags::SDL_RENDERER_ACCELERATED );

    myWindow1->set_flags( SDL_WindowFlags::SDL_WINDOW_SHOWN );
    myWindow1Flags = myWindow1->get_flags();
    spdlog::trace( fmt::runtime( "window 1 flags: 0b{:0>32b}" ), static_cast< uint32_t >( myWindow1Flags ) );

    myWindow1->run_input_loop();
  } );

  // SFG::Engine::SdlWindow* myWindow2 = myEngine->add_window( "My Window 2", 800, 600 );
  // std::thread myThread2( [myEngine, myWindow2]() {
  //   SDL_WindowFlags myWindow2Flags;
  //   myWindow2Flags = myWindow2->get_flags();
  //   spdlog::trace( fmt::runtime( "window 2 flags: 0b{:0>32b}" ), static_cast< uint32_t >( myWindow2Flags ) );
  //
  //   myWindow2->initialize_logic_controller();
  //   SFG::Content::Wall* firstWall = myWindow2->get_script_manager()->add_script< SFG::Content::Wall >();
  //   firstWall->set_position( 200.0L, 200.0L );
  //   firstWall->set_size( 50.0L, 50.0L );
  //   spdlog::debug( fmt::runtime( "firstWall at {:p}" ), static_cast< void* >( firstWall ) );
  //   SFG::Content::Wall* secondWall = myWindow2->get_script_manager()->add_script< SFG::Content::Wall >();
  //   secondWall->set_position( 250.0L, 150.0L );
  //   secondWall->set_size( 50.0L, 50.0L );
  //   spdlog::debug( fmt::runtime( "secondWall at {:p}" ), static_cast< void* >( secondWall ) );
  //   SFG::Content::Player* player = myWindow2->get_script_manager()->add_script< SFG::Content::Player >();
  //   player->set_size( 50.0L, 50.0L );
  //   spdlog::debug( fmt::runtime( "player at {:p}" ), static_cast< void* >( player ) );
  //
  //   myWindow2->get_script_manager()->add_script< SFG::Content::DebugInfo >();
  //   myWindow2->initialize_window_renderer( "direct3d11", SDL_RendererFlags::SDL_RENDERER_ACCELERATED );
  //
  //   myWindow2->set_flags( SDL_WindowFlags::SDL_WINDOW_SHOWN );
  //   myWindow2Flags = myWindow2->get_flags();
  //   spdlog::trace( fmt::runtime( "window 2 flags: 0b{:0>32b}" ), static_cast< uint32_t >( myWindow2Flags ) );
  //
  //   myWindow2->run_input_loop();
  // } );
  //
  // SFG::Engine::SdlWindow* myWindow3 = myEngine->add_window( "My Window 3", 800, 600 );
  // std::thread myThread3( [myEngine, myWindow3]() {
  //   SDL_WindowFlags myWindow3Flags;
  //   myWindow3Flags = myWindow3->get_flags();
  //   spdlog::trace( fmt::runtime( "window 3 flags: 0b{:0>32b}" ), static_cast< uint32_t >( myWindow3Flags ) );
  //
  //   myWindow3->initialize_logic_controller();
  //   SFG::Content::Wall* firstWall = myWindow3->get_script_manager()->add_script< SFG::Content::Wall >();
  //   firstWall->set_position( 200.0L, 200.0L );
  //   firstWall->set_size( 50.0L, 50.0L );
  //   spdlog::debug( fmt::runtime( "firstWall at {:p}" ), static_cast< void* >( firstWall ) );
  //   SFG::Content::Wall* secondWall = myWindow3->get_script_manager()->add_script< SFG::Content::Wall >();
  //   secondWall->set_position( 250.0L, 150.0L );
  //   secondWall->set_size( 50.0L, 50.0L );
  //   spdlog::debug( fmt::runtime( "secondWall at {:p}" ), static_cast< void* >( secondWall ) );
  //   SFG::Content::Player* player = myWindow3->get_script_manager()->add_script< SFG::Content::Player >();
  //   player->set_size( 50.0L, 50.0L );
  //   spdlog::debug( fmt::runtime( "player at {:p}" ), static_cast< void* >( player ) );
  //
  //   myWindow3->get_script_manager()->add_script< SFG::Content::DebugInfo >();
  //   myWindow3->initialize_window_renderer( "software", SDL_RendererFlags::SDL_RENDERER_ACCELERATED );
  //
  //   myWindow3->set_flags( SDL_WindowFlags::SDL_WINDOW_SHOWN );
  //   myWindow3Flags = myWindow3->get_flags();
  //   spdlog::trace( fmt::runtime( "window 3 flags: 0b{:0>32b}" ), static_cast< uint32_t >( myWindow3Flags ) );
  //
  //   myWindow3->run_input_loop();
  // } );
  //
  // SFG::Engine::SdlWindow* myWindow4 = myEngine->add_window( "My Window 4", 800, 600 );
  // std::thread myThread4( [myEngine, myWindow4]() {
  //   SDL_WindowFlags myWindow4Flags;
  //   myWindow4Flags = myWindow4->get_flags();
  //   spdlog::trace( fmt::runtime( "window 4 flags: 0b{:0>32b}" ), static_cast< uint32_t >( myWindow4Flags ) );
  //
  //   myWindow4->initialize_logic_controller();
  //   SFG::Content::Wall* firstWall = myWindow4->get_script_manager()->add_script< SFG::Content::Wall >();
  //   firstWall->set_position( 200.0L, 200.0L );
  //   firstWall->set_size( 50.0L, 50.0L );
  //   spdlog::debug( fmt::runtime( "firstWall at {:p}" ), static_cast< void* >( firstWall ) );
  //   SFG::Content::Wall* secondWall = myWindow4->get_script_manager()->add_script< SFG::Content::Wall >();
  //   secondWall->set_position( 250.0L, 150.0L );
  //   secondWall->set_size( 50.0L, 50.0L );
  //   spdlog::debug( fmt::runtime( "secondWall at {:p}" ), static_cast< void* >( secondWall ) );
  //   SFG::Content::Player* player = myWindow4->get_script_manager()->add_script< SFG::Content::Player >();
  //   player->set_size( 50.0L, 50.0L );
  //   spdlog::debug( fmt::runtime( "player at {:p}" ), static_cast< void* >( player ) );
  //
  //   myWindow4->get_script_manager()->add_script< SFG::Content::DebugInfo >();
  //   myWindow4->initialize_window_renderer( "software", SDL_RendererFlags::SDL_RENDERER_ACCELERATED );
  //
  //   myWindow4->set_flags( SDL_WindowFlags::SDL_WINDOW_SHOWN );
  //   myWindow4Flags = myWindow4->get_flags();
  //   spdlog::trace( fmt::runtime( "window 4 flags: 0b{:0>32b}" ), static_cast< uint32_t >( myWindow4Flags ) );
  //
  //   myWindow4->run_input_loop();
  // } );

  myEngine->run_input_loop();

  myThread1.join();
  // myThread2.join();
  // myThread3.join();
  // myThread4.join();

  delete myEngine;
  myEngine = nullptr;

  spdlog::trace( fmt::runtime( "better_main()~" ) );
  return EXIT_SUCCESS;
}
