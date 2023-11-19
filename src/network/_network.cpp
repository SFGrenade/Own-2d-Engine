#include "network/_network.h"

#include <string>
#include <thread>

#include "_globals/misc.h"
#include "_globals/zmq.h"
#include "confighandler/confighandler.h"
#include "network/networkhandler.h"

namespace SFG {
Network::Network( zmq::context_t* contextToUse )
    : logger_( spdlog::get( "Network" ) ),
      network_Network_Send_( "inproc://network", true, contextToUse ),
      network_Graphics_Receive_( "inproc://graphics", false, contextToUse ),
      network_Input_Receive_( "inproc://input", false, contextToUse ),
      network_Logic_Receive_( "inproc://logic", false, contextToUse ),
      network_Network_Receive_( "inproc://network", false, contextToUse ),
      isRunning_( false ),
      networkHandler_( new NetworkHandler() ),
      performanceLoops_( 0 ),
      stop_thread_callbacks_(),
      get_performance_counters_callbacks_() {
  this->logger_->trace( fmt::runtime( "Network( contextToUse = {:p} )" ), static_cast< void* >( contextToUse ) );

  this->networkHandler_->SetReqRepInfo( fmt::format( fmt::runtime( "tcp://{:s}" ), SFG::ConfigHandler::get_Network_Host() ),
                                        SFG::ConfigHandler::get_Network_PortReqRep() );
  this->networkHandler_->SetPubSubInfo( fmt::format( fmt::runtime( "tcp://{:s}" ), SFG::ConfigHandler::get_Network_Host() ),
                                        SFG::ConfigHandler::get_Network_PortPubSub() );
  this->networkHandler_->InitializeNetwork();

  add_Get_Performance_Counters_callback( [this]( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) {
    // this->logger_->trace( fmt::runtime( "Get_Performance_Counters_callback( msg )" ) );

    SFG::Proto::InProc::Get_Performance_Counters_Reply* repMsg = new SFG::Proto::InProc::Get_Performance_Counters_Reply();
    repMsg->set_counter_network( this->performanceLoops_ );
    network_Network_Send_.sendMessage( repMsg );
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

  this->logger_->trace( fmt::runtime( "Network()~" ) );
}

Network::~Network() {
  this->logger_->trace( fmt::runtime( "~Network()" ) );

  this->logger_->trace( fmt::runtime( "~Network()~" ) );
}

#define tryCatchZmqpbRun( x )                                                         \
  try {                                                                               \
    ( x ).run();                                                                      \
  } catch( std::exception const& e ) {                                                \
    this->logger_->error( fmt::runtime( "thread - network error: {:s}" ), e.what() ); \
  }

void Network::start() {
  this->logger_->trace( fmt::runtime( "start()" ) );

  this->isRunning_ = true;

  this->logger_->trace( fmt::runtime( "start()~" ) );
}

void Network::run() {
  // this->logger_->trace( fmt::runtime( "run()" ) );

  // inproc networking
  tryCatchZmqpbRun( this->network_Network_Send_ );
  tryCatchZmqpbRun( this->network_Graphics_Receive_ );
  tryCatchZmqpbRun( this->network_Input_Receive_ );
  tryCatchZmqpbRun( this->network_Logic_Receive_ );
  tryCatchZmqpbRun( this->network_Network_Receive_ );

  this->networkHandler_->RunNetwork();

  ++performanceLoops_;

  // this->logger_->trace( fmt::runtime( "run()~" ) );
}

bool Network::isRunning() {
  // this->logger_->trace( fmt::runtime( "isRunning()" ) );

  // this->logger_->trace( fmt::runtime( "isRunning()~ => {}" ), this->isRunning_ );
  return this->isRunning_;
}

void Network::add_Stop_Thread_callback( std::function< void( SFG::Proto::InProc::Stop_Thread_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "add_Stop_Thread_callback( callback )" ) );

  if( callback )
    stop_thread_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "add_Stop_Thread_callback()~" ) );
}

void Network::add_Get_Performance_Counters_callback( std::function< void( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "add_Get_Performance_Counters_callback( callback )" ) );

  if( callback )
    get_performance_counters_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "add_Get_Performance_Counters_callback()~" ) );
}
}  // namespace SFG
