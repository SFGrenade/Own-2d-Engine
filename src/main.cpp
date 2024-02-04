#include <chrono>
#include <span>
#include <string_view>
#include <thread>
#include <vector>

#include "_globals/moreChrono.h"
#include "_globals/spdlogInclude.h"
#include "content/scripts/audioregion.h"
#include "content/scripts/bouncer.h"
#include "content/scripts/debuginfo.h"
#include "content/scripts/logscript.h"
#include "content/scripts/player.h"
#include "content/scripts/wall.h"
#include "engine/loggerfactory.h"
#include "engine/scriptmanager.h"
#include "engine/sdlaudio.h"
#include "engine/sdlengine.h"
#include "engine/sdlwindow.h"


int main( int argc, char** argv ) {
  int better_main( std::span< std::string_view const > ) noexcept;
  std::vector< std::string_view > args( argv, std::next( argv, static_cast< std::ptrdiff_t >( argc ) ) );
  return better_main( args );
}

int better_main( std::span< std::string_view const > args ) noexcept {
  SFG::Engine::LoggerFactory::init();

  spdlog::trace( fmt::runtime( "better_main( args = {:c}\"{:s}\"{:c} )" ), '{', fmt::join( args, "\", \"" ), '}' );

  SFG::Engine::SdlEngine* myEngine = new SFG::Engine::SdlEngine( SDL_INIT_EVERYTHING, IMG_INIT_PNG, MIX_INIT_OGG );

  std::vector< std::string > renderers = myEngine->get_renderer_names();
  spdlog::debug( fmt::runtime( "Available renderers: {}" ), fmt::join( renderers, ", " ) );

  SFG::Engine::SdlWindow* myWindow1 = myEngine->add_window( "My Window 1", 800, 600 );
  std::thread myThread1( [myEngine, myWindow1]() {
    SDL_WindowFlags myWindow1Flags;
    myWindow1Flags = myWindow1->get_flags();
    spdlog::trace( fmt::runtime( "window 1 flags: 0b{:0>32b}" ), static_cast< uint32_t >( myWindow1Flags ) );

    for( int i = 0; i < ( 800 / 50 ); i++ ) {
      SFG::Content::Wall* wallUpper = myWindow1->get_script_manager()->add_script< SFG::Content::Wall >();
      wallUpper->set_position( i * 50.0L, 0.0L );
      wallUpper->set_size( 50.0L, 50.0L );
      SFG::Content::Wall* wallLower = myWindow1->get_script_manager()->add_script< SFG::Content::Wall >();
      wallLower->set_position( i * 50.0L, 550.0L );
      wallLower->set_size( 50.0L, 50.0L );
    }
    for( int i = 0; i < ( 500 / 50 ); i++ ) {
      SFG::Content::Wall* wallLeft = myWindow1->get_script_manager()->add_script< SFG::Content::Wall >();
      wallLeft->set_position( 0.0L, ( i + 1 ) * 50.0L );
      wallLeft->set_size( 50.0L, 50.0L );
      SFG::Content::Wall* wallRight = myWindow1->get_script_manager()->add_script< SFG::Content::Wall >();
      wallRight->set_position( 750.0L, ( i + 1 ) * 50.0L );
      wallRight->set_size( 50.0L, 50.0L );
    }
    for( int i = 0; i < ( ( 800 / 50 ) / 2 / 2 ); i++ ) {
      SFG::Content::Wall* wall1 = myWindow1->get_script_manager()->add_script< SFG::Content::Wall >();
      wall1->set_position( ( ( i + 1 ) * 100.0L * 2 ), 50.0L );
      wall1->set_size( 50.0L * 2, 50.0L * 2 );
      SFG::Content::Wall* wall2 = myWindow1->get_script_manager()->add_script< SFG::Content::Wall >();
      wall2->set_position( 50.0L + ( i * 100.0L * 2 ), 200.0L );
      wall2->set_size( 50.0L * 2, 50.0L * 2 );
      SFG::Content::Wall* wall3 = myWindow1->get_script_manager()->add_script< SFG::Content::Wall >();
      wall3->set_position( ( ( i + 1 ) * 100.0L * 2 ), 350.0L );
      wall3->set_size( 50.0L * 2, 50.0L * 2 );
    }

    SFG::Content::AudioRegion* ar1 = myWindow1->get_script_manager()->add_script< SFG::Content::AudioRegion >();
    ar1->set_position( 65.0L, 65.0L );
    ar1->set_size( 20.0L, 20.0L );
    ar1->set_audio_file_path( R"(./Resources/Audio/8Bit 01 w.wav)" );

    SFG::Content::AudioRegion* ar2 = myWindow1->get_script_manager()->add_script< SFG::Content::AudioRegion >();
    ar2->set_position( 315.0L, 65.0L );
    ar2->set_size( 20.0L, 20.0L );
    ar2->set_audio_file_path( R"(./Resources/Audio/8Bit 02 w.wav)" );

    SFG::Content::AudioRegion* ar3 = myWindow1->get_script_manager()->add_script< SFG::Content::AudioRegion >();
    ar3->set_position( 515.0L, 65.0L );
    ar3->set_size( 20.0L, 20.0L );
    ar3->set_audio_file_path( R"(./Resources/Audio/8Bit 03 w.wav)" );

    SFG::Content::AudioRegion* ar4 = myWindow1->get_script_manager()->add_script< SFG::Content::AudioRegion >();
    ar4->set_position( 715.0L, 65.0L );
    ar4->set_size( 20.0L, 20.0L );
    ar4->set_audio_file_path( R"(./Resources/Audio/8Bit 04 w.wav)" );

    SFG::Content::AudioRegion* ar5 = myWindow1->get_script_manager()->add_script< SFG::Content::AudioRegion >();
    ar5->set_position( 65.0L, 315.0L );
    ar5->set_size( 20.0L, 20.0L );
    ar5->set_audio_file_path( R"(./Resources/Audio/8Bit 05 w.wav)" );

    SFG::Content::AudioRegion* ar6 = myWindow1->get_script_manager()->add_script< SFG::Content::AudioRegion >();
    ar6->set_position( 715.0L, 315.0L );
    ar6->set_size( 20.0L, 20.0L );
    ar6->set_audio_file_path( R"(./Resources/Audio/8Bit 06 w.wav)" );

    SFG::Content::AudioRegion* ar7 = myWindow1->get_script_manager()->add_script< SFG::Content::AudioRegion >();
    ar7->set_position( 65.0L, 515.0L );
    ar7->set_size( 20.0L, 20.0L );
    ar7->set_audio_file_path( R"(./Resources/Audio/8Bit 07 w.wav)" );

    SFG::Content::AudioRegion* ar8 = myWindow1->get_script_manager()->add_script< SFG::Content::AudioRegion >();
    ar8->set_position( 715.0L, 515.0L );
    ar8->set_size( 20.0L, 20.0L );
    ar8->set_audio_file_path( R"(./Resources/Audio/8Bit 08 w.wav)" );

    for( int i = 0; i < 2; i++ ) {
      for( int k = 0; k < 2; k++ ) {
        SFG::Content::Bouncer* bouncer = myWindow1->get_script_manager()->add_script< SFG::Content::Bouncer >();
        bouncer->set_position( ( k * 30.0L ) + 65.0L, ( i * 30.0L ) + 65.0L );
        bouncer->set_size( 20.0L, 20.0L );
        bouncer->set_speed( 70.0L, 70.0L );
      }
    }

    SFG::Content::Player* player = myWindow1->get_script_manager()->add_script< SFG::Content::Player >();
    player->set_position( 701.0L, 501.0L );
    player->set_size( 48.0L, 48.0L );

    myWindow1->get_script_manager()->add_script< SFG::Content::DebugInfo >();
    myWindow1->initialize_logic_controller();
    // myWindow1->initialize_window_renderer( "direct3d", SDL_RendererFlags::SDL_RENDERER_ACCELERATED );
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
  //   myWindow2->initialize_logic_controller();
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
  //   myWindow3->initialize_logic_controller();
  //   myWindow3->initialize_window_renderer( "direct3d12", SDL_RendererFlags::SDL_RENDERER_ACCELERATED );
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
  //   myWindow4->initialize_logic_controller();
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
  SFG::Engine::LoggerFactory::deinit();
  return EXIT_SUCCESS;
}
