#include <csignal>
#include <cstdint>
#include <string_view>
#include <vector>

// Including our headers
#include "_globals/spdlogInclude.h"
#include "_server/server.h"

#pragma region Initialization

std::function< void( int32_t ) > signalCallback = nullptr;
void signalHandler( int sigNum ) {
  signal( sigNum, signalHandler );
  if( signalCallback ) {
    signalCallback( sigNum );
  }
}

void InitializeSignalHandler() noexcept {
  spdlog::trace( "InitializeSignalHandler()" );

  std::set_terminate( []() {
    spdlog::trace( "terminateCallback()" );
    spdlog::trace( "terminateCallback()~" );
  } );
  for( int i = 0; i <= NSIG; i++ ) {
    auto retCode = signal( i, signalHandler );
    spdlog::debug( "Installing handler for {}: {}", i, SIG_ERR != retCode );
  }

  spdlog::trace( "InitializeSignalHandler()~" );
}

void InitializeLoggers() {
  auto consoleSink = std::make_shared< spdlog::sinks::stdout_color_sink_mt >();
  consoleSink->set_level( spdlog::level::warn );

  auto truncatedFileSink = std::make_shared< spdlog::sinks::basic_file_sink_mt >( "log_server.log", true );
  truncatedFileSink->set_level( spdlog::level::trace );
  spdlog::sinks_init_list truncatedSinkList = { truncatedFileSink, consoleSink };

  spdlogger mainLogger = std::make_shared< spdlog::logger >( "main", truncatedSinkList.begin(), truncatedSinkList.end() );
  mainLogger->set_level( spdlog::level::trace );
  mainLogger->flush_on( spdlog::level::trace );
  spdlog::register_logger( mainLogger );
  spdlog::set_default_logger( mainLogger );

  auto normalFileSink = std::make_shared< spdlog::sinks::basic_file_sink_mt >( "log_server.log", false );
  normalFileSink->set_level( spdlog::level::trace );
  spdlog::sinks_init_list normalSinkList = { normalFileSink, consoleSink };

  std::vector< std::string > allLoggerNames = { "Server" };
  for( auto name : allLoggerNames ) {
    spdlogger logger = std::make_shared< spdlog::logger >( name, normalSinkList.begin(), normalSinkList.end() );
    logger->set_level( spdlog::level::trace );
    logger->flush_on( spdlog::level::trace );
    spdlog::register_logger( logger );
  }
  // spdlog::get("LogScript")->set_level(spdlog::level::warn);
}

#pragma endregion

int main( int const argc, char const *const *argv ) {
  int better_main( std::span< std::string_view const > ) noexcept;
  std::vector< std::string_view > args( argv, std::next( argv, static_cast< std::ptrdiff_t >( argc ) ) );
  return better_main( args );
}

int better_main( std::span< std::string_view const > args ) noexcept {
  InitializeLoggers();
  spdlog::trace( "better_main(args = {:c} \"{:s}\" {:c})", '{', fmt::join( args, "\", \"" ), '}' );

  InitializeSignalHandler();

  spdlog::trace( "Constructing Server" );
  SFG::Server *myServer = new SFG::Server( 13337, 13338 );

  signalCallback = [myServer]( int32_t signal ) {
    spdlog::trace( "signalCallback( signal: {} )", signal );
    myServer->stopServer();
    // delete myServer;
    spdlog::trace( "signalCallback()~" );
  };

  spdlog::trace( "Calling Server::startServer" );
  myServer->startServer();
  spdlog::trace( "Called Server::startServer" );

  spdlog::trace( "Waiting for Server to stop" );
  myServer->waitForServer();

  spdlog::trace( "Cleaning up Server" );
  delete myServer;

  spdlog::trace( "better_main()~" );
  return EXIT_SUCCESS;
}
