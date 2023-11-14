#include "network/_network.h"

#include <string>
#include <thread>

#include "_globals/misc.h"
#include "_globals/zmq.h"

namespace SFG {
Network::Network()
    : logger_( spdlog::get( "TSrv" ) ),
      network_Network_Send_( "inproc://network", true, &global_inproc_context_ ),
      network_Graphics_Receive_( "inproc://graphics", false, &global_inproc_context_ ),
      network_Input_Receive_( "inproc://input", false, &global_inproc_context_ ),
      network_Logic_Receive_( "inproc://logic", false, &global_inproc_context_ ),
      network_Network_Receive_( "inproc://network", false, &global_inproc_context_ ),
      workerThread_( nullptr ),
      workerThreadIsRunning_( false ),
      performanceLoops_( 0 ),
      stop_thread_callbacks_(),
      get_config_callbacks_(),
      get_performance_counters_callbacks_() {
  this->logger_->trace( fmt::runtime( "[thread {:s}] Network()" ), getThreadId() );

  add_Get_Performance_Counters_callback( [this]( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) {
    this->logger_->trace( fmt::runtime( "[thread {:s}] Get_Performance_Counters_callback()" ), getThreadId() );

    SFG::Proto::InProc::Get_Performance_Counters_Reply* repMsg = new SFG::Proto::InProc::Get_Performance_Counters_Reply();
    repMsg->set_counter_network( this->performanceLoops_ );
    network_Network_Send_.sendMessage( repMsg );
    this->performanceLoops_ = 0;

    this->logger_->trace( fmt::runtime( "[thread {:s}] Get_Performance_Counters_callback()~" ), getThreadId() );
  } );
  add_Stop_Thread_callback( [this]( SFG::Proto::InProc::Stop_Thread_Request const& ) {
    this->logger_->trace( fmt::runtime( "[thread {:s}] Stop_Thread_callback()" ), getThreadId() );

    this->workerThreadIsRunning_ = false;

    this->logger_->trace( fmt::runtime( "[thread {:s}] Stop_Thread_callback()~" ), getThreadId() );
  } );

  network_Input_Receive_.subscribe( new SFG::Proto::InProc::Stop_Thread_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->stop_thread_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Stop_Thread_Request const& >( message ) );
  } );
  network_Input_Receive_.subscribe( new SFG::Proto::InProc::Get_Config_Reply(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->get_config_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Get_Config_Reply const& >( message ) );
  } );
  network_Logic_Receive_.subscribe( new SFG::Proto::InProc::Get_Performance_Counters_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->get_performance_counters_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Get_Performance_Counters_Request const& >( message ) );
  } );

  this->logger_->trace( fmt::runtime( "[thread {:s}] Network()~" ), getThreadId() );
}

Network::~Network() {
  this->logger_->trace( fmt::runtime( "[thread {:s}] ~Network()" ), getThreadId() );

  this->logger_->trace( fmt::runtime( "[thread {:s}] ~Network()~" ), getThreadId() );
}

#define tryCatchZmqpbRun( x )                                                                                      \
  try {                                                                                                            \
    ( x ).run();                                                                                                   \
  } catch( std::exception const& e ) {                                                                             \
    this->logger_->error( fmt::runtime( "[thread {:s}] thread - network error: {:s}" ), getThreadId(), e.what() ); \
  }

void Network::start() {
  this->logger_->trace( fmt::runtime( "[thread {:s}] start()" ), getThreadId() );

  this->workerThreadIsRunning_ = true;
  this->workerThread_ = std::make_unique< std::thread >( [this]() {
    this->logger_->trace( fmt::runtime( "[thread {:s}] thread()" ), getThreadId() );

    while( this->workerThreadIsRunning_ ) {  // inproc networking
      // inproc networking
      tryCatchZmqpbRun( this->network_Network_Send_ );
      tryCatchZmqpbRun( this->network_Graphics_Receive_ );
      tryCatchZmqpbRun( this->network_Input_Receive_ );
      tryCatchZmqpbRun( this->network_Logic_Receive_ );
      tryCatchZmqpbRun( this->network_Network_Receive_ );
      ++performanceLoops_;
    }

    this->logger_->trace( fmt::runtime( "[thread {:s}] thread()~" ), getThreadId() );
  } );

  this->logger_->trace( fmt::runtime( "[thread {:s}] start()~" ), getThreadId() );
}

void Network::join() {
  this->logger_->trace( fmt::runtime( "[thread {:s}] join()" ), getThreadId() );

  this->workerThread_->join();

  this->logger_->trace( fmt::runtime( "[thread {:s}] join()~" ), getThreadId() );
}

void Network::add_Stop_Thread_callback( std::function< void( SFG::Proto::InProc::Stop_Thread_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Stop_Thread_callback( callback )" ), getThreadId() );

  if( callback )
    stop_thread_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Stop_Thread_callback()~" ), getThreadId() );
}

void Network::add_Get_Config_callback( std::function< void( SFG::Proto::InProc::Get_Config_Reply const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Get_Config_callback( callback )" ), getThreadId() );

  if( callback )
    get_config_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Get_Config_callback()~" ), getThreadId() );
}

void Network::add_Get_Performance_Counters_callback( std::function< void( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Get_Performance_Counters_callback( callback )" ), getThreadId() );

  if( callback )
    get_performance_counters_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Get_Performance_Counters_callback()~" ), getThreadId() );
}
}  // namespace SFG
