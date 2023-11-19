#include "graphics/_graphics.h"

#include <string>
#include <thread>

#include "_globals/misc.h"
#include "_globals/zmq.h"
#include "confighandler/confighandler.h"
#include "graphics/window.h"

namespace SFG {
Graphics::Graphics( zmq::context_t* contextToUse )
    : logger_( spdlog::get( "Graphics" ) ),
      network_Graphics_Send_( "inproc://graphics", true, contextToUse ),
      network_Graphics_Receive_( "inproc://graphics", false, contextToUse ),
      network_Input_Receive_( "inproc://input", false, contextToUse ),
      network_Logic_Receive_( "inproc://logic", false, contextToUse ),
      network_Network_Receive_( "inproc://network", false, contextToUse ),
      isRunning_( false ),
      window_( std::shared_ptr< SFG::Window >( new SFG::Window() ) ),
      performanceLoops_( 0 ),
      stop_thread_callbacks_(),
      update_performance_information_callbacks_(),
      get_performance_counters_callbacks_() {
  this->logger_->trace( fmt::runtime( "Graphics( contextToUse = {:p} )" ), static_cast< void* >( contextToUse ) );

  /* Initialize SDL to get the backend renderer */
  this->window_->InitializeSDL();
  this->window_->InitializeWindow();
  /* Get the backend renderer */
  int numRDevices = SDL_GetNumRenderDrivers();
  int rendererIndex = 0;
  if( numRDevices < 0 ) {
    this->logger_->error( fmt::runtime( "Graphics - Error when SDL_GetNumRenderDrivers: {:s}" ), SDL_GetError() );
  } else {
    this->logger_->debug( fmt::runtime( "Graphics - SDL_GetNumRenderDrivers() => {:d}" ), numRDevices );
    for( int i = 0; i < numRDevices; i++ ) {
      SDL_Renderer* renderer = SDL_CreateRenderer( this->window_->GetSdlWindow().get(), i, 0 );
      if( renderer == nullptr ) {
        this->logger_->error( fmt::runtime( "Graphics - Error when SDL_CreateRenderer: {:s}" ), SDL_GetError() );
      }
      SDL_RendererInfo info;
      if( SDL_GetRendererInfo( renderer, &info ) != 0 ) {
        this->logger_->error( fmt::runtime( "Graphics - Error when SDL_GetRendererInfo: {:s}" ), SDL_GetError() );
      }
      SDL_DestroyRenderer( renderer );
      if( SFG::ConfigHandler::get_Rendering_Renderer() == info.name ) {
        rendererIndex = i;
      }
    }
  }
  /* Destroy and create a new window */
  this->window_.reset();
  this->window_ = std::shared_ptr< SFG::Window >( new SFG::Window() );
  this->window_->SetSize( SFG::ConfigHandler::get_Window_Width(), SFG::ConfigHandler::get_Window_Height() );
  this->window_->InitializeSDL();
  this->window_->InitializeWindow();
  this->window_->GetGraphicsHandler()->SetRendererIndex( rendererIndex );

  this->window_->GetGraphicsHandler()->RegisterDrawEvent( [this]( std::shared_ptr< SDL_Renderer > ) {
    SFG::Proto::InProc::Run_UpdateFrame_Request* msg = new SFG::Proto::InProc::Run_UpdateFrame_Request();
    this->network_Graphics_Send_.sendMessage( msg );
  } );

  add_Stop_Thread_callback( [this]( SFG::Proto::InProc::Stop_Thread_Request const& ) {
    this->logger_->trace( fmt::runtime( "Stop_Thread_callback()" ) );

    this->isRunning_ = false;

    this->logger_->trace( fmt::runtime( "Stop_Thread_callback()~" ) );
  } );
  add_Update_Performance_Information_callback( [this]( SFG::Proto::InProc::Update_Performance_Information_Request const& msg ) {
    // this->logger_->trace( fmt::runtime( "Update_Performance_Information_callback()" ) );

    this->window_->GetGraphicsHandler()->SetDebugString( msg.performance() );

    // this->logger_->trace( fmt::runtime( "Update_Performance_Information_callback()~" ) );
  } );
  add_Get_Performance_Counters_callback( [this]( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) {
    // this->logger_->trace( fmt::runtime( "Get_Performance_Counters_callback()" ) );

    SFG::Proto::InProc::Get_Performance_Counters_Reply* repMsg = new SFG::Proto::InProc::Get_Performance_Counters_Reply();
    repMsg->set_counter_graphics( this->performanceLoops_ );
    network_Graphics_Send_.sendMessage( repMsg );
    this->performanceLoops_ = 0;

    // this->logger_->trace( fmt::runtime( "Get_Performance_Counters_callback()~" ) );
  } );

  network_Input_Receive_.subscribe( new SFG::Proto::InProc::Stop_Thread_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->stop_thread_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Stop_Thread_Request const& >( message ) );
  } );
  network_Logic_Receive_.subscribe( new SFG::Proto::InProc::Update_Performance_Information_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->update_performance_information_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Update_Performance_Information_Request const& >( message ) );
  } );
  network_Logic_Receive_.subscribe( new SFG::Proto::InProc::Get_Performance_Counters_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->get_performance_counters_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Get_Performance_Counters_Request const& >( message ) );
  } );

  EmptySubscribe< SFG::Proto::InProc::Run_UpdateFrame_Request >( network_Graphics_Receive_ );
  EmptySubscribe< SFG::Proto::InProc::Get_Performance_Counters_Reply >( network_Graphics_Receive_ );
  EmptySubscribe< SFG::Proto::InProc::Get_Performance_Counters_Reply >( network_Input_Receive_ );
  EmptySubscribe< SFG::Proto::InProc::Get_Performance_Counters_Reply >( network_Logic_Receive_ );
  EmptySubscribe< SFG::Proto::InProc::Get_Performance_Counters_Reply >( network_Network_Receive_ );

  this->logger_->trace( fmt::runtime( "Graphics()~" ) );
}

Graphics::~Graphics() {
  this->logger_->trace( fmt::runtime( "~Graphics()" ) );

  this->logger_->trace( fmt::runtime( "~Graphics()~" ) );
}

void Graphics::start() {
  this->logger_->trace( fmt::runtime( "start()" ) );

  this->isRunning_ = true;
  this->window_->ShowWindow();
  this->window_->GetGraphicsHandler()->StartDraw();

  this->logger_->trace( fmt::runtime( "start()~" ) );
}

#define tryCatchZmqpbRun( x )                                                         \
  try {                                                                               \
    ( x ).run();                                                                      \
  } catch( std::exception const& e ) {                                                \
    this->logger_->error( fmt::runtime( "thread - network error: {:s}" ), e.what() ); \
  }

void Graphics::run() {
  // this->logger_->trace( fmt::runtime( "run()" ) );

  // inproc networking
  tryCatchZmqpbRun( this->network_Graphics_Send_ );
  tryCatchZmqpbRun( this->network_Graphics_Receive_ );
  tryCatchZmqpbRun( this->network_Input_Receive_ );
  tryCatchZmqpbRun( this->network_Logic_Receive_ );
  tryCatchZmqpbRun( this->network_Network_Receive_ );

  this->window_->GetGraphicsHandler()->Draw();

  ++performanceLoops_;

  // this->logger_->trace( fmt::runtime( "run()~" ) );
}

bool Graphics::isRunning() {
  // this->logger_->trace( fmt::runtime( "isRunning()" ) );

  // this->logger_->trace( fmt::runtime( "isRunning()~ => {}" ), this->isRunning_ );
  return this->isRunning_;
}

void Graphics::add_Stop_Thread_callback( std::function< void( SFG::Proto::InProc::Stop_Thread_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "add_Stop_Thread_callback( callback )" ) );

  if( callback )
    stop_thread_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "add_Stop_Thread_callback()~" ) );
}

void Graphics::add_Update_Performance_Information_callback(
    std::function< void( SFG::Proto::InProc::Update_Performance_Information_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "add_Update_Performance_Information_callback( callback )" ) );

  if( callback )
    update_performance_information_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "add_Update_Performance_Information_callback()~" ) );
}

void Graphics::add_Get_Performance_Counters_callback( std::function< void( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "add_Get_Performance_Counters_callback( callback )" ) );

  if( callback )
    get_performance_counters_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "add_Get_Performance_Counters_callback()~" ) );
}
}  // namespace SFG
