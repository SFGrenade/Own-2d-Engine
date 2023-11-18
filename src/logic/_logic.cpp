#include "logic/_logic.h"

#include <string>
#include <thread>

#include "_globals/misc.h"
#include "_globals/zmq.h"
#include "confighandler/confighandler.h"
#include "logic/logichandler.h"

namespace SFG {
Logic::Logic( zmq::context_t* contextToUse )
    : logger_( spdlog::get( "Logic" ) ),
      network_Logic_Send_( "inproc://logic", true, contextToUse ),
      network_Graphics_Receive_( "inproc://graphics", false, contextToUse ),
      network_Input_Receive_( "inproc://input", false, contextToUse ),
      network_Logic_Receive_( "inproc://logic", false, contextToUse ),
      network_Network_Receive_( "inproc://network", false, contextToUse ),
      workerThread_( nullptr ),
      isRunning_( false ),
      performanceLoops_( 0 ),
      stop_thread_callbacks_(),
      get_performance_counters_callbacks_() {
  this->logger_->trace( fmt::runtime( "Logic()" ) );
  this->logger_->trace( fmt::runtime( "Logic - using context at {:p}" ), static_cast< void* >( contextToUse ) );

  add_Get_Performance_Counters_callback( [this]( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) {
    this->logger_->trace( fmt::runtime( "Get_Performance_Counters_callback()" ) );

    SFG::Proto::InProc::Get_Performance_Counters_Reply* repMsg = new SFG::Proto::InProc::Get_Performance_Counters_Reply();
    repMsg->set_counter_logic( this->performanceLoops_ );
    network_Logic_Send_.sendMessage( repMsg );
    this->performanceLoops_ = 0;

    this->logger_->trace( fmt::runtime( "Get_Performance_Counters_callback()~" ) );
  } );
  add_Stop_Thread_callback( [this]( SFG::Proto::InProc::Stop_Thread_Request const& ) {
    this->logger_->trace( fmt::runtime( "Stop_Thread_callback()" ) );

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
  EmptySubscribe< SFG::Proto::InProc::Get_Performance_Counters_Reply >( network_Graphics_Receive_ );
  EmptySubscribe< SFG::Proto::InProc::Get_Performance_Counters_Reply >( network_Input_Receive_ );
  EmptySubscribe< SFG::Proto::InProc::Get_Performance_Counters_Reply >( network_Logic_Receive_ );
  EmptySubscribe< SFG::Proto::InProc::Get_Performance_Counters_Reply >( network_Network_Receive_ );

  this->logger_->trace( fmt::runtime( "Logic()~" ) );
}

Logic::~Logic() {
  this->logger_->trace( fmt::runtime( "~Logic()" ) );

  this->logger_->trace( fmt::runtime( "~Logic()~" ) );
}

#define tryCatchZmqpbRun( x )                                                         \
  try {                                                                               \
    ( x ).run();                                                                      \
  } catch( std::exception const& e ) {                                                \
    this->logger_->error( fmt::runtime( "thread - network error: {:s}" ), e.what() ); \
  }

void Logic::start() {
  this->logger_->trace( fmt::runtime( "start()" ) );

  this->isRunning_ = true;

  this->logger_->trace( fmt::runtime( "start()~" ) );
}

void Logic::run() {
  // this->logger_->trace( fmt::runtime( "run()" ) );

  // inproc networking
  tryCatchZmqpbRun( this->network_Logic_Send_ );
  tryCatchZmqpbRun( this->network_Graphics_Receive_ );
  tryCatchZmqpbRun( this->network_Input_Receive_ );
  tryCatchZmqpbRun( this->network_Logic_Receive_ );
  tryCatchZmqpbRun( this->network_Network_Receive_ );
  ++performanceLoops_;

  // this->logger_->trace( fmt::runtime( "run()~" ) );
}

bool Logic::isRunning() {
  // this->logger_->trace( fmt::runtime( "isRunning()" ) );

  // this->logger_->trace( fmt::runtime( "isRunning()~ => {}" ), this->isRunning_ );
  return this->isRunning_;
}

void Logic::add_Stop_Thread_callback( std::function< void( SFG::Proto::InProc::Stop_Thread_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "add_Stop_Thread_callback( callback )" ) );

  if( callback )
    stop_thread_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "add_Stop_Thread_callback()~" ) );
}

void Logic::add_Get_Performance_Counters_callback( std::function< void( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "add_Get_Performance_Counters_callback( callback )" ) );

  if( callback )
    get_performance_counters_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "add_Get_Performance_Counters_callback()~" ) );
}
}  // namespace SFG
