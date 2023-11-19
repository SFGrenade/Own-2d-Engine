#include "logic/_logic.h"

#include <string>
#include <thread>

#include "_globals/misc.h"
#include "_globals/zmq.h"
#include "confighandler/confighandler.h"
#include "logic/CustomScripts/logscript.h"
#include "logic/logichandler.h"
#include "logic/scripthandler.h"

namespace SFG {
Logic::Logic( zmq::context_t* contextToUse )
    : logger_( spdlog::get( "Logic" ) ),
      network_Logic_Send_( "inproc://logic", true, contextToUse ),
      network_Graphics_Receive_( "inproc://graphics", false, contextToUse ),
      network_Input_Receive_( "inproc://input", false, contextToUse ),
      network_Logic_Receive_( "inproc://logic", false, contextToUse ),
      network_Network_Receive_( "inproc://network", false, contextToUse ),
      isRunning_( false ),
      logicHandler_( new LogicHandler() ),
      scriptHandler_( new ScriptHandler() ),
      cachePerformanceCounters_(),
      performanceLoops_( 0 ),
      stop_thread_callbacks_(),
      get_performance_counters_callbacks_() {
  this->logger_->trace( fmt::runtime( "Logic( contextToUse = {:p} )" ), static_cast< void* >( contextToUse ) );

  // this->scriptHandler_->AddScript< SFG::LogScript >();

  this->logicHandler_->AddTimer(
      [this]( std::optional< std::chrono::secondsLongDouble > ) {
        // 50 hz test timer
        this->scriptHandler_->UpdateScriptsLogicFrame();
      },
      std::chrono::duration_cast< std::chrono::nanoseconds >( 1.0s / SFG::ConfigHandler::get_Logic_LogicInterval() ),
      false );
  this->logicHandler_->AddTimer(
      [this]( std::optional< std::chrono::secondsLongDouble > ) {
        SFG::Proto::InProc::Get_Performance_Counters_Request* msg = new SFG::Proto::InProc::Get_Performance_Counters_Request();
        network_Logic_Send_.sendMessage( msg );
      },
      std::chrono::duration_cast< std::chrono::nanoseconds >( 1.0s / SFG::ConfigHandler::get_Logic_PerformanceInterval() ),
      false );
  spdlog::get( "Performance" )->trace( fmt::runtime( "Graphics\tInput\tLogic\tNetwork" ) );
  this->logicHandler_->AddTimer(
      [this]( std::optional< std::chrono::secondsLongDouble > ) {
        uint64_t counter_graphics
            = static_cast< uint64_t >( ( this->cachePerformanceCounters_.has_counter_graphics() ? this->cachePerformanceCounters_.counter_graphics() : 0 )
                                       * SFG::ConfigHandler::get_Logic_PerformanceInterval() );
        uint64_t counter_input
            = static_cast< uint64_t >( ( this->cachePerformanceCounters_.has_counter_input() ? this->cachePerformanceCounters_.counter_input() : 0 )
                                       * SFG::ConfigHandler::get_Logic_PerformanceInterval() );
        uint64_t counter_logic
            = static_cast< uint64_t >( ( this->cachePerformanceCounters_.has_counter_logic() ? this->cachePerformanceCounters_.counter_logic() : 0 )
                                       * SFG::ConfigHandler::get_Logic_PerformanceInterval() );
        uint64_t counter_network
            = static_cast< uint64_t >( ( this->cachePerformanceCounters_.has_counter_network() ? this->cachePerformanceCounters_.counter_network() : 0 )
                                       * SFG::ConfigHandler::get_Logic_PerformanceInterval() );
        spdlog::get( "Performance" )->trace( fmt::runtime( "{:d}\t{:d}\t{:d}\t{:d}" ), counter_graphics, counter_input, counter_logic, counter_network );

        std::string performanceString = fmt::format( fmt::runtime( R"(Performance (per second):
{:>20d} Frames drawn
{:>20d} Input checks
{:>20d} Logic loops
{:>20d} Network loops)" ),
                                                     counter_graphics,
                                                     counter_input,
                                                     counter_logic,
                                                     counter_network );
        SFG::Proto::InProc::Update_Performance_Information_Request* msg = new SFG::Proto::InProc::Update_Performance_Information_Request();
        msg->set_performance( performanceString );
        network_Logic_Send_.sendMessage( msg );
      },
      std::chrono::duration_cast< std::chrono::nanoseconds >( 1.0s / SFG::ConfigHandler::get_Logic_PerformanceInterval() ),
      false );

  add_Run_UpdateFrame_callback( [this]( SFG::Proto::InProc::Run_UpdateFrame_Request const& ) {
    this->logger_->trace( fmt::runtime( "Run_UpdateFrame_callback( msg )" ) );

    this->scriptHandler_->UpdateScriptsFrame();

    this->logger_->trace( fmt::runtime( "Run_UpdateFrame_callback()~" ) );
  } );
  add_Stop_Thread_callback( [this]( SFG::Proto::InProc::Stop_Thread_Request const& ) {
    this->logger_->trace( fmt::runtime( "Stop_Thread_callback( msg )" ) );

    this->isRunning_ = false;

    this->logger_->trace( fmt::runtime( "Stop_Thread_callback()~" ) );
  } );
  add_Get_Performance_Counters_callback( [this]( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) {
    // this->logger_->trace( fmt::runtime( "Get_Performance_Counters_callback( msg )" ) );

    SFG::Proto::InProc::Get_Performance_Counters_Reply* repMsg = new SFG::Proto::InProc::Get_Performance_Counters_Reply();
    repMsg->set_counter_logic( this->performanceLoops_ );
    network_Logic_Send_.sendMessage( repMsg );
    this->performanceLoops_ = 0;

    // this->logger_->trace( fmt::runtime( "Get_Performance_Counters_callback()~" ) );
  } );
  add_Set_Performance_Counters_callback( [this]( SFG::Proto::InProc::Get_Performance_Counters_Reply const& msg ) {
    // this->logger_->trace( fmt::runtime( "Set_Performance_Counters_callback( msg )" ) );

    if( msg.has_counter_graphics() ) {
      this->cachePerformanceCounters_.set_counter_graphics( msg.counter_graphics() );
    }
    if( msg.has_counter_input() ) {
      this->cachePerformanceCounters_.set_counter_input( msg.counter_input() );
    }
    if( msg.has_counter_logic() ) {
      this->cachePerformanceCounters_.set_counter_logic( msg.counter_logic() );
    }
    if( msg.has_counter_network() ) {
      this->cachePerformanceCounters_.set_counter_network( msg.counter_network() );
    }

    // this->logger_->trace( fmt::runtime( "Set_Performance_Counters_callback()~" ) );
  } );

  network_Graphics_Receive_.subscribe( new SFG::Proto::InProc::Run_UpdateFrame_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->run_updateFrame_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Run_UpdateFrame_Request const& >( message ) );
  } );
  network_Input_Receive_.subscribe( new SFG::Proto::InProc::Stop_Thread_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->stop_thread_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Stop_Thread_Request const& >( message ) );
  } );
  network_Logic_Receive_.subscribe( new SFG::Proto::InProc::Get_Performance_Counters_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->get_performance_counters_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Get_Performance_Counters_Request const& >( message ) );
  } );

  network_Graphics_Receive_.subscribe( new SFG::Proto::InProc::Get_Performance_Counters_Reply(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->set_performance_counters_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Get_Performance_Counters_Reply const& >( message ) );
  } );
  network_Input_Receive_.subscribe( new SFG::Proto::InProc::Get_Performance_Counters_Reply(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->set_performance_counters_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Get_Performance_Counters_Reply const& >( message ) );
  } );
  network_Logic_Receive_.subscribe( new SFG::Proto::InProc::Get_Performance_Counters_Reply(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->set_performance_counters_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Get_Performance_Counters_Reply const& >( message ) );
  } );
  network_Network_Receive_.subscribe( new SFG::Proto::InProc::Get_Performance_Counters_Reply(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->set_performance_counters_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Get_Performance_Counters_Reply const& >( message ) );
  } );

  EmptySubscribe< SFG::Proto::InProc::Run_UpdateFrame_Request >( network_Graphics_Receive_ );
  EmptySubscribe< SFG::Proto::InProc::Update_Performance_Information_Request >( network_Logic_Receive_ );

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
  this->logicHandler_->StartLogic();

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

  this->logicHandler_->UpdateLogic();

  ++performanceLoops_;

  // this->logger_->trace( fmt::runtime( "run()~" ) );
}

bool Logic::isRunning() {
  // this->logger_->trace( fmt::runtime( "isRunning()" ) );

  // this->logger_->trace( fmt::runtime( "isRunning()~ => {}" ), this->isRunning_ );
  return this->isRunning_;
}

void Logic::add_Run_UpdateFrame_callback( std::function< void( SFG::Proto::InProc::Run_UpdateFrame_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "add_Stop_Thread_callback( callback )" ) );

  if( callback )
    run_updateFrame_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "add_Stop_Thread_callback()~" ) );
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

void Logic::add_Set_Performance_Counters_callback( std::function< void( SFG::Proto::InProc::Get_Performance_Counters_Reply const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "add_Set_Performance_Counters_callback( callback )" ) );

  if( callback )
    set_performance_counters_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "add_Set_Performance_Counters_callback()~" ) );
}
}  // namespace SFG
