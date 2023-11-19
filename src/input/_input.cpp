#include "input/_input.h"

#include <string>
#include <thread>

#include "_globals/misc.h"
#include "_globals/zmq.h"
#include "confighandler/confighandler.h"
#include "input/inputhandler.h"

namespace SFG {
Input::Input( zmq::context_t* contextToUse )
    : logger_( spdlog::get( "Input" ) ),
      network_Input_Send_( "inproc://input", true, contextToUse ),
      network_Graphics_Receive_( "inproc://graphics", false, contextToUse ),
      network_Input_Receive_( "inproc://input", false, contextToUse ),
      network_Logic_Receive_( "inproc://logic", false, contextToUse ),
      network_Network_Receive_( "inproc://network", false, contextToUse ),
      isRunning_( false ),
      inputHandler_( new InputHandler() ),
      performanceLoops_( 0 ),
      stop_thread_callbacks_(),
      get_performance_counters_callbacks_() {
  this->logger_->trace( fmt::runtime( "Input( contextToUse = {:p} )" ), static_cast< void* >( contextToUse ) );

  this->inputHandler_->RegisterQuitEvent( [this]() {
    this->logger_->trace( fmt::runtime( "QuitEvent()" ) );

    SFG::Proto::InProc::Stop_Thread_Request* msg = new SFG::Proto::InProc::Stop_Thread_Request();
    this->network_Input_Send_.sendMessage( msg );

    this->logger_->trace( fmt::runtime( "QuitEvent()~" ) );
  } );
  this->inputHandler_->RegisterWindowEvent( [this]( SDL_WindowEvent const& window ) {
    this->logger_->trace( fmt::runtime( "WindowEvent( window = {:d} )" ), static_cast< int >( window.event ) );

    if( window.event == SDL_WINDOWEVENT_CLOSE ) {
      SFG::Proto::InProc::Stop_Thread_Request* msg = new SFG::Proto::InProc::Stop_Thread_Request();
      this->network_Input_Send_.sendMessage( msg );
    }

    this->logger_->trace( fmt::runtime( "WindowEvent()~" ) );
  } );
  this->inputHandler_->RegisterKeyDownEvent( [this]( SDL_KeyboardEvent const& key ) {
    this->logger_->trace( fmt::runtime( "KeyDownEvent( key = {:d} )" ), static_cast< int >( key.keysym.sym ) );

    if( key.keysym.sym == SFG::ConfigHandler::get_Input_StopGameKey() ) {
      SFG::Proto::InProc::Stop_Thread_Request* msg = new SFG::Proto::InProc::Stop_Thread_Request();
      this->network_Input_Send_.sendMessage( msg );
    }

    this->logger_->trace( fmt::runtime( "KeyDownEvent()~" ) );
  } );

  add_Get_Performance_Counters_callback( [this]( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) {
    // this->logger_->trace( fmt::runtime( "Get_Performance_Counters_callback( msg )" ) );

    SFG::Proto::InProc::Get_Performance_Counters_Reply* repMsg = new SFG::Proto::InProc::Get_Performance_Counters_Reply();
    repMsg->set_counter_input( this->performanceLoops_ );
    this->network_Input_Send_.sendMessage( repMsg );
    this->performanceLoops_ = 0;

    // this->logger_->trace( fmt::runtime( "Get_Performance_Counters_callback()~" ) );
  } );
  add_Stop_Thread_callback( [this]( SFG::Proto::InProc::Stop_Thread_Request const& ) {
    this->logger_->trace( fmt::runtime( "Stop_Thread_callback( msg )" ) );

    this->isRunning_ = false;

    this->logger_->trace( fmt::runtime( "Stop_Thread_callback()~" ) );
  } );

  network_Input_Receive_.subscribe( new SFG::Proto::InProc::Stop_Thread_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->stop_thread_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Stop_Thread_Request const& >( message ) );
  } );
  network_Logic_Receive_.subscribe( new SFG::Proto::InProc::Get_Performance_Counters_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->get_performance_counters_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Get_Performance_Counters_Request const& >( message ) );
  } );

  EmptySubscribe< SFG::Proto::InProc::Run_UpdateFrame_Request >( network_Graphics_Receive_ );
  EmptySubscribe< SFG::Proto::InProc::Update_Performance_Information_Request >( network_Logic_Receive_ );
  EmptySubscribe< SFG::Proto::InProc::Get_Performance_Counters_Reply >( network_Graphics_Receive_ );
  EmptySubscribe< SFG::Proto::InProc::Get_Performance_Counters_Reply >( network_Input_Receive_ );
  EmptySubscribe< SFG::Proto::InProc::Get_Performance_Counters_Reply >( network_Logic_Receive_ );
  EmptySubscribe< SFG::Proto::InProc::Get_Performance_Counters_Reply >( network_Network_Receive_ );

  this->logger_->trace( fmt::runtime( "Input()~" ) );
}

Input::~Input() {
  this->logger_->trace( fmt::runtime( "~Input()" ) );

  this->logger_->trace( fmt::runtime( "~Input()~" ) );
}

#define tryCatchZmqpbRun( x )                                                         \
  try {                                                                               \
    ( x ).run();                                                                      \
  } catch( std::exception const& e ) {                                                \
    this->logger_->error( fmt::runtime( "thread - network error: {:s}" ), e.what() ); \
  }

void Input::start() {
  this->logger_->trace( fmt::runtime( "start()" ) );

  this->isRunning_ = true;

  this->logger_->trace( fmt::runtime( "start()~" ) );
}

void Input::run() {
  // this->logger_->trace( fmt::runtime( "run()" ) );

  // inproc networking
  tryCatchZmqpbRun( this->network_Input_Send_ );
  tryCatchZmqpbRun( this->network_Graphics_Receive_ );
  tryCatchZmqpbRun( this->network_Input_Receive_ );
  tryCatchZmqpbRun( this->network_Logic_Receive_ );
  tryCatchZmqpbRun( this->network_Network_Receive_ );

  this->inputHandler_->CheckInputs();

  ++performanceLoops_;

  // this->logger_->trace( fmt::runtime( "run()~" ) );
}

bool Input::isRunning() {
  // this->logger_->trace( fmt::runtime( "isRunning()" ) );

  // this->logger_->trace( fmt::runtime( "isRunning()~ => {}" ), this->isRunning_ );
  return this->isRunning_;
}

void Input::add_Stop_Thread_callback( std::function< void( SFG::Proto::InProc::Stop_Thread_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "add_Stop_Thread_callback( callback )" ) );

  if( callback )
    stop_thread_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "add_Stop_Thread_callback()~" ) );
}

void Input::add_Get_Performance_Counters_callback( std::function< void( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "add_Get_Performance_Counters_callback( callback )" ) );

  if( callback )
    get_performance_counters_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "add_Get_Performance_Counters_callback()~" ) );
}
}  // namespace SFG
