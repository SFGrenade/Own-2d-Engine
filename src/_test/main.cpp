#include <chrono>
#include <csignal>
#include <cstdint>
#include <iostream>
#include <span>
#include <sstream>
#include <string_view>
#include <vector>
#include <zmqPb/pair.hpp>

// Including our headers
#include "_globals/misc.h"
#include "_globals/spdlogInclude.h"
#include "testMessages.pb.h"

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
  void onInterThreadFunctionCall( SFG::Proto::Test::InterThreadFunctionCall const& msg );

  private:
  spdlogger logger_;
  static zmq::context_t threadNetworkContext_;

  ZmqPb::Pair network_;
  std::thread* thread_;
  bool loop_;
};

zmq::context_t ThreadServer::threadNetworkContext_ = zmq::context_t( 0 );

ThreadServer::ThreadServer( std::string const& host, bool server )
    : logger_( spdlog::get( "TSrv" ) ), network_( host, server, &ThreadServer::threadNetworkContext_ ), thread_( nullptr ), loop_( false ) {
  logger_->trace( fmt::runtime( "ThreadServer( host = \"{:s}\", bool server = {} )" ), host, server );

  network_.subscribe( new SFG::Proto::Test::InterThreadFunctionCall(), [this]( google::protobuf::Message const& message ) {
    this->onInterThreadFunctionCall( static_cast< SFG::Proto::Test::InterThreadFunctionCall const& >( message ) );
  } );

  logger_->trace( fmt::runtime( "ThreadServer()~" ) );
}
ThreadServer::~ThreadServer() {
  logger_->trace( fmt::runtime( "~ThreadServer()" ) );

  if( thread_ ) {
    logger_->trace( fmt::runtime( "~ThreadServer - deleting thread" ) );
    if( thread_->joinable() ) {
      thread_->join();
    }
    delete thread_;
    thread_ = nullptr;
  }

  logger_->trace( fmt::runtime( "~ThreadServer()~" ) );
}
void ThreadServer::startThreadServer() {
  logger_->trace( fmt::runtime( "startThreadServer()" ) );

  loop_ = true;

  thread_ = new std::thread( [this]() {
    this->logger_->trace( fmt::runtime( "thread_()" ) );
    while( this->loop_ ) {
      try {
        this->network_.run();
      } catch( std::exception const& e ) {
        this->logger_->error( fmt::runtime( "thread_ - reqRep error: {:s}" ), e.what() );
      }
      std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    }
    this->logger_->trace( fmt::runtime( "thread_()~" ) );
  } );

  logger_->trace( fmt::runtime( "startThreadServer()~" ) );
}
void ThreadServer::waitForThreadServer() {
  logger_->trace( fmt::runtime( "waitForThreadServer()" ) );

  if( thread_ ) {
    if( thread_->joinable() ) {
      thread_->join();
    }
  }

  logger_->trace( fmt::runtime( "waitForThreadServer()~" ) );
}
void ThreadServer::stopThreadServer() {
  logger_->trace( fmt::runtime( "stopThreadServer()" ) );

  loop_ = false;

  logger_->trace( fmt::runtime( "stopThreadServer()~" ) );
}
void ThreadServer::sendFunctionCall( std::function< void() > functionToSend ) {
  logger_->trace( fmt::runtime( "sendFunctionCall( functionToSend = {:p} )" ), static_cast< void* >( functionToSend.target< void ( * )() >() ) );

  SFG::Proto::Test::InterThreadFunctionCall* msg = new SFG::Proto::Test::InterThreadFunctionCall();
  msg->set_functionpointer( reinterpret_cast< int64_t >( functionToSend.target< void ( * )() >() ) );
  network_.sendMessage( msg );

  logger_->trace( fmt::runtime( "sendFunctionCall()~" ) );
}
void ThreadServer::onInterThreadFunctionCall( SFG::Proto::Test::InterThreadFunctionCall const& msg ) {
  void ( *functionToCall )() = reinterpret_cast< void ( * )() >( msg.functionpointer() );
  logger_->trace( fmt::runtime( "onInterThreadFunctionCall( msg = {:p} )" ), static_cast< void* >( functionToCall ) );

  if( functionToCall != nullptr ) {
    functionToCall();
  } else {
    stopThreadServer();
  }

  logger_->trace( fmt::runtime( "onInterThreadFunctionCall()~" ) );
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
TestClass::TestClass() : logger_( spdlog::get( "TSrv" ) ) {
  logger_->trace( fmt::runtime( "TestClass()" ) );

  logger_->trace( fmt::runtime( "TestClass()~" ) );
}
TestClass::~TestClass() {
  logger_->trace( fmt::runtime( "~TestClass()" ) );

  logger_->trace( fmt::runtime( "~TestClass()~" ) );
}
void TestClass::doStuff( int num ) {
  logger_->trace( fmt::runtime( "doStuff( num = {:d} )" ), num );

  logger_->trace( fmt::runtime( "doStuff()~" ) );
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
  spdlog::trace( fmt::runtime( "InitializeSignalHandler()" ) );

  std::set_terminate( []() {
    spdlog::trace( fmt::runtime( "terminateCallback()" ) );
    spdlog::trace( fmt::runtime( "terminateCallback()~" ) );
  } );
  for( int i = 0; i <= NSIG; i++ ) {
    auto retCode = signal( i, signalHandler );
    spdlog::debug( fmt::runtime( "Installing handler for {}: {}" ), i, SIG_ERR != retCode );
  }

  spdlog::trace( fmt::runtime( "InitializeSignalHandler()~" ) );
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

  std::vector< std::string > allLoggerNames = { "TSrv" };
  for( auto name : allLoggerNames ) {
    spdlogger logger = std::make_shared< spdlog::logger >( name, truncatedSinkList.begin(), truncatedSinkList.end() );
    logger->set_level( spdlog::level::trace );
    logger->flush_on( spdlog::level::trace );
    spdlog::register_logger( logger );
  }
  // spdlog::get("LogScript")->set_level(spdlog::level::warn);
  spdlog::set_pattern( "[%Y-%m-%d %H:%M:%S.%e] [thread %t] [%n] [%l] %v" );
}

#pragma endregion Initialization

#pragma region Other Misc

template < typename TypeObject, typename TypeFunction, typename... TypeArgs >
void TestStuff( ThreadServer* threadPeer, TypeObject* objSelf, TypeFunction ( TypeObject::*objFunction )( TypeArgs... ), TypeArgs... args ) {
  threadPeer->sendFunctionCall( [objSelf, objFunction, args...]() { objSelf->*objFunction( args... ); } );
}

#pragma endregion Other Misc

int main( int const argc, char const* const* argv ) {
  int better_main( std::span< std::string_view const > ) noexcept;
  std::vector< std::string_view > args( argv, std::next( argv, static_cast< std::ptrdiff_t >( argc ) ) );
  return better_main( args );
}

void MyThreadFunction() {
  ThreadServer* myServer = new ThreadServer( "inproc://example", true );
  myServer->startThreadServer();
  myServer->waitForThreadServer();
  delete myServer;
}

int better_main( std::span< std::string_view const > args ) noexcept {
  InitializeLoggers();
  spdlog::trace( fmt::runtime( "better_main( args = {:c}\"{:s}\"{:c} )" ), '{', fmt::join( args, "\", \"" ), '}' );

  InitializeSignalHandler();

  std::thread thingieThread( MyThreadFunction );

  std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

  spdlog::trace( fmt::runtime( "Constructing Server" ) );
  ThreadServer* myServer = new ThreadServer( "inproc://example", false );

  TestClass* myTestClass = new TestClass();

  signalCallback = [myServer]( int32_t signal ) {
    spdlog::trace( fmt::runtime( "signalCallback( signal: {} )" ), signal );

    myServer->stopThreadServer();
    // delete myServer;

    spdlog::trace( fmt::runtime( "signalCallback()~" ) );
  };

  spdlog::trace( fmt::runtime( "Calling Server::startServer" ) );
  myServer->startThreadServer();
  spdlog::trace( fmt::runtime( "Called Server::startServer" ) );

  std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

  myServer->sendFunctionCall( []() {
    spdlog::trace( fmt::runtime( "test sendFunctionCall()" ) );

    spdlog::trace( fmt::runtime( "test sendFunctionCall()~" ) );
  } );

  std::thread otherThingieThread( [myServer, myTestClass]() {
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    // TestStuff( myServer, myTestClass, &TestClass::doStuff, 1 );
    myServer->sendFunctionCall( nullptr );
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    myServer->stopThreadServer();
  } );

  spdlog::trace( fmt::runtime( "Waiting for Server to stop" ) );
  myServer->waitForThreadServer();

  spdlog::trace( fmt::runtime( "Cleaning up Server" ) );
  delete myServer;

  spdlog::trace( fmt::runtime( "better_main()~" ) );
  return EXIT_SUCCESS;
}
