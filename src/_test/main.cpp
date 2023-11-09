#include <chrono>
#include <csignal>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>
#include <zmqPb/pair.hpp>

// Including our headers
#include "_globals/spdlogInclude.h"
#include "networkMessages.pb.h"

#pragma region Misc

std::string getThreadId() {
  std::stringstream ret;
  ret << std::this_thread::get_id();
  return ret.str();
}

#pragma endregion Misc

#pragma region ThreadServer

class ThreadServer {
  public:
  ThreadServer( std::string const& host, bool server );
  ~ThreadServer();

  void startThreadServer();
  void waitForThreadServer();
  void stopThreadServer();

  void sendFunctionCall( std::function< void() > functionToSend );

  private:
  void onInterThreadFunctionCall( SFG::Proto::InterThreadFunctionCall const& msg );

  private:
  spdlogger logger_;
  static zmq::context_t threadNetworkContext_;

  ZmqPb::Pair network_;
  std::thread* thread_;
  bool loop_;
};

zmq::context_t ThreadServer::threadNetworkContext_ = zmq::context_t( 0 );

ThreadServer::ThreadServer( std::string const& host, bool server )
  : logger_( spdlog::get( "TSrv" ) ),
    network_( host, server, &ThreadServer::threadNetworkContext_ ),
    thread_( nullptr ),
    loop_( false ) {
  logger_->trace( fmt::runtime( "[thread {:s}] ThreadServer(std::string const& host = \"{:s}\", bool server = {})" ), getThreadId(), host, server );
  network_.subscribe( new SFG::Proto::InterThreadFunctionCall(), [this]( google::protobuf::Message const& message ) {
    this->onInterThreadFunctionCall( static_cast< SFG::Proto::InterThreadFunctionCall const& >( message ) );
  } );
  logger_->trace( fmt::runtime( "[thread {:s}] ThreadServer()~" ), getThreadId() );
}
ThreadServer::~ThreadServer() {
  logger_->trace( fmt::runtime( "[thread {:s}] ~ThreadServer()" ), getThreadId() );
  if( thread_ ) {
    logger_->trace( fmt::runtime( "[thread {:s}] ~ThreadServer - deleting thread" ), getThreadId() );
    if( thread_->joinable() ) {
      thread_->join();
    }
    delete thread_;
    thread_ = nullptr;
  }
  logger_->trace( fmt::runtime( "[thread {:s}] ~ThreadServer()~" ), getThreadId() );
}
void ThreadServer::startThreadServer() {
  logger_->trace( fmt::runtime( "[thread {:s}] startThreadServer()" ), getThreadId() );

  loop_ = true;

  thread_ = new std::thread( [this]() {
    this->logger_->trace( fmt::runtime( "[thread {:s}] thread_()" ), getThreadId() );
    while( this->loop_ ) {
      try {
        this->network_.run();
      } catch( std::exception const& e ) {
        this->logger_->error( fmt::runtime( "[thread {:s}] thread_ - reqRep error: {:s}" ), getThreadId(), e.what() );
      }
      std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    }
    this->logger_->trace( fmt::runtime( "[thread {:s}] thread_()~" ), getThreadId() );
  } );

  logger_->trace( fmt::runtime( "[thread {:s}] startThreadServer()~" ), getThreadId() );
}
void ThreadServer::waitForThreadServer() {
  logger_->trace( fmt::runtime( "[thread {:s}] waitForThreadServer()" ), getThreadId() );

  if( thread_ ) {
    if( thread_->joinable() ) {
      thread_->join();
    }
  }

  logger_->trace( fmt::runtime( "[thread {:s}] waitForThreadServer()~" ), getThreadId() );
}
void ThreadServer::stopThreadServer() {
  logger_->trace( fmt::runtime( "[thread {:s}] stopThreadServer()" ), getThreadId() );

  loop_ = false;

  logger_->trace( fmt::runtime( "[thread {:s}] stopThreadServer()~" ), getThreadId() );
}
void ThreadServer::sendFunctionCall( std::function< void() > functionToSend ) {
  logger_->trace( fmt::runtime( "[thread {:s}] sendFunctionCall(std::function< void() > functionToSend = {:p})" ), getThreadId(), static_cast< void* >( functionToSend.target< void(*)() >() ) );

  SFG::Proto::InterThreadFunctionCall* msg = new SFG::Proto::InterThreadFunctionCall();
  msg->set_functionpointer( reinterpret_cast< int64_t >( functionToSend.target< void(*)() >() ) );
  network_.sendMessage( msg );

  logger_->trace( fmt::runtime( "[thread {:s}] sendFunctionCall()~" ), getThreadId() );
}
void ThreadServer::onInterThreadFunctionCall( SFG::Proto::InterThreadFunctionCall const& msg ) {
  void( *functionToCall )() = reinterpret_cast< void(*)() >( msg.functionpointer() );
  logger_->trace( fmt::runtime( "[thread {:s}] onInterThreadFunctionCall(SFG::Proto::InterThreadFunctionCall const& msg = {:p})" ), getThreadId(), static_cast< void* >( functionToCall ) );

  if ( functionToCall != nullptr ) {
    functionToCall();
  } else {
    stopThreadServer();
  }

  logger_->trace( fmt::runtime( "[thread {:s}] onInterThreadFunctionCall()~" ), getThreadId() );
}

#pragma endregion ThreadServer

#pragma region TestClass

class TestClass {
  public:
  TestClass();
  ~TestClass();

  void doStuff( int num );

  private:
  spdlogger logger_;
};
TestClass::TestClass()
  : logger_( spdlog::get( "TSrv" ) ) {
  logger_->trace( fmt::runtime( "[thread {:s}] TestClass()" ), getThreadId() );
  logger_->trace( fmt::runtime( "[thread {:s}] TestClass()~" ), getThreadId() );
}
TestClass::~TestClass() {
  logger_->trace( fmt::runtime( "[thread {:s}] ~TestClass()" ), getThreadId() );
  logger_->trace( fmt::runtime( "[thread {:s}] ~TestClass()~" ), getThreadId() );
}
void TestClass::doStuff( int num ) {
  logger_->trace( fmt::runtime( "[thread {:s}] doStuff(int num = {:d})" ), getThreadId(), num );
  logger_->trace( fmt::runtime( "[thread {:s}] doStuff()~" ), getThreadId() );
}

#pragma endregion ThreadServer

#pragma region Initialization

std::function< void( int32_t ) > signalCallback = nullptr;
void signalHandler( int sigNum ) {
  signal( sigNum, signalHandler );
  if( signalCallback ) {
    signalCallback( sigNum );
  }
}

void InitializeSignalHandler() noexcept {
  spdlog::trace( fmt::runtime( "[thread {:s}] InitializeSignalHandler()" ), getThreadId() );

  std::set_terminate( []() {
    spdlog::trace( fmt::runtime( "[thread {:s}] terminateCallback()" ), getThreadId() );
    spdlog::trace( fmt::runtime( "[thread {:s}] terminateCallback()~" ), getThreadId() );
  } );
  for( int i = 0; i <= NSIG; i++ ) {
    auto retCode = signal( i, signalHandler );
    spdlog::debug( fmt::runtime( "[thread {:s}] Installing handler for {}: {}" ), getThreadId(), i, SIG_ERR != retCode );
  }

  spdlog::trace( fmt::runtime( "[thread {:s}] InitializeSignalHandler()~" ), getThreadId() );
}

void InitializeLoggers() {
  auto consoleSink = std::make_shared< spdlog::sinks::stdout_color_sink_mt >();
  consoleSink->set_level( spdlog::level::warn );

  auto truncatedFileSink = std::make_shared< spdlog::sinks::basic_file_sink_mt >( "log_test.log", true );
  truncatedFileSink->set_level( spdlog::level::trace );
  spdlog::sinks_init_list truncatedSinkList = { truncatedFileSink, consoleSink };

  spdlogger mainLogger = std::make_shared< spdlog::logger >( "main", truncatedSinkList.begin(), truncatedSinkList.end() );
  mainLogger->set_level( spdlog::level::trace );
  mainLogger->flush_on( spdlog::level::trace );
  spdlog::register_logger( mainLogger );
  spdlog::set_default_logger( mainLogger );

  // auto normalFileSink = std::make_shared< spdlog::sinks::basic_file_sink_mt >( "log_test.log", false );
  // normalFileSink->set_level( spdlog::level::trace );
  // spdlog::sinks_init_list normalSinkList = { normalFileSink, consoleSink };

  std::vector< std::string > allLoggerNames = { "TSrv" };
  for( auto name : allLoggerNames ) {
    spdlogger logger = std::make_shared< spdlog::logger >( name, truncatedSinkList.begin(), truncatedSinkList.end() );
    logger->set_level( spdlog::level::trace );
    logger->flush_on( spdlog::level::trace );
    spdlog::register_logger( logger );
  }
  // spdlog::get("LogScript")->set_level(spdlog::level::warn);
}

#pragma endregion Initialization

#pragma region Other Misc

template < typename TypeObject, typename TypeFunction, typename... TypeArgs >
void TestStuff( ThreadServer* threadPeer, TypeObject* objSelf, TypeFunction( TypeObject::*objFunction )( TypeArgs... ), TypeArgs... args ) {
  threadPeer->sendFunctionCall( [objSelf, objFunction, args...]() {
    objSelf->*objFunction( args... );
  } );
}

#pragma endregion Other Misc

int main( int const argc, char const *const *argv ) {
  int better_main( std::span< std::string_view const > ) noexcept;
  std::vector< std::string_view > args( argv, std::next( argv, static_cast< std::ptrdiff_t >( argc ) ) );
  return better_main( args );
}

void MyThreadFunction() {
  ThreadServer *myServer = new ThreadServer( "inproc://example", true );
  myServer->startThreadServer();
  myServer->waitForThreadServer();
  delete myServer;
}

int better_main( std::span< std::string_view const > args ) noexcept {
  InitializeLoggers();
  spdlog::trace( fmt::runtime( "[thread {:s}] better_main(args = {:c} \"{:s}\" {:c})" ), getThreadId(), '{', fmt::join( args, "\", \"" ), '}' );

  InitializeSignalHandler();

  std::thread thingieThread( MyThreadFunction );

  std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

  spdlog::trace( fmt::runtime( "[thread {:s}] Constructing Server" ), getThreadId() );
  ThreadServer *myServer = new ThreadServer( "inproc://example", false );

  TestClass *myTestClass = new TestClass();

  signalCallback = [myServer]( int32_t signal ) {
    spdlog::trace( fmt::runtime( "[thread {:s}] signalCallback( signal: {} )" ), getThreadId(), signal );
    myServer->stopThreadServer();
    // delete myServer;
    spdlog::trace( fmt::runtime( "[thread {:s}] signalCallback()~" ), getThreadId() );
  };

  spdlog::trace( fmt::runtime( "[thread {:s}] Calling Server::startServer" ), getThreadId() );
  myServer->startThreadServer();
  spdlog::trace( fmt::runtime( "[thread {:s}] Called Server::startServer" ), getThreadId() );

  std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

  myServer->sendFunctionCall( []() {
    spdlog::trace( fmt::runtime( "[thread {:s}] test sendFunctionCall()" ), getThreadId() );
    spdlog::trace( fmt::runtime( "[thread {:s}] test sendFunctionCall()~" ), getThreadId() );
  } );

  std::thread otherThingieThread( [myServer, myTestClass]() {
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    // TestStuff( myServer, myTestClass, &TestClass::doStuff, 1 );
    myServer->sendFunctionCall( nullptr );
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    myServer->stopThreadServer();
  } );

  spdlog::trace( fmt::runtime( "[thread {:s}] Waiting for Server to stop" ), getThreadId() );
  myServer->waitForThreadServer();

  spdlog::trace( fmt::runtime( "[thread {:s}] Cleaning up Server" ), getThreadId() );
  delete myServer;

  spdlog::trace( fmt::runtime( "[thread {:s}] better_main()~" ), getThreadId() );
  return EXIT_SUCCESS;
}
