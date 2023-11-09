#include "_globals/misc.h"
#include "_inproc/inprocconnection.h"

namespace SFG {
zmq::context_t InProcConnection::networkContext_ = zmq::context_t( 0 );

InProcConnection::InProcConnection( std::string const& host, bool server )
  : logger_( spdlog::get( "TSrv" ) ),
    network_( host, server, &InProcConnection::networkContext_ ),
    thread_( nullptr ),
    loop_( false ) {
  logger_->trace( fmt::runtime( "[thread {:s}] InProcConnection(std::string const& host = \"{:s}\", bool server = {})" ), getThreadId(), host, server );
  network_.subscribe( new SFG::Proto::InterThreadFunctionCall(), [this]( google::protobuf::Message const& message ) {
    this->onInterThreadFunctionCall( static_cast< SFG::Proto::InterThreadFunctionCall const& >( message ) );
  } );
  logger_->trace( fmt::runtime( "[thread {:s}] InProcConnection()~" ), getThreadId() );
}

InProcConnection::~InProcConnection() {
  logger_->trace( fmt::runtime( "[thread {:s}] ~InProcConnection()" ), getThreadId() );
  if( thread_ ) {
    logger_->trace( fmt::runtime( "[thread {:s}] ~InProcConnection - deleting thread" ), getThreadId() );
    if( thread_->joinable() ) {
      thread_->join();
    }
    delete thread_;
    thread_ = nullptr;
  }
  logger_->trace( fmt::runtime( "[thread {:s}] ~InProcConnection()~" ), getThreadId() );
}

void InProcConnection::run() {
  // logger_->trace( fmt::runtime( "[thread {:s}] run()" ), getThreadId() );

  try {
    this->network_.run();
  } catch( std::exception const& e ) {
    this->logger_->error( fmt::runtime( "[thread {:s}] run - network error: {:s}" ), getThreadId(), e.what() );
  }

  // logger_->trace( fmt::runtime( "[thread {:s}] run()~" ), getThreadId() );
}

void InProcConnection::sendFunctionCall( std::function< void() > functionToSend ) {
  logger_->trace( fmt::runtime( "[thread {:s}] sendFunctionCall(std::function< void() > functionToSend = {:p})" ), getThreadId(), static_cast< void* >( functionToSend.target< void(*)() >() ) );

  SFG::Proto::InterThreadFunctionCall* msg = new SFG::Proto::InterThreadFunctionCall();
  msg->set_functionpointer( reinterpret_cast< int64_t >( functionToSend.target< void(*)() >() ) );
  network_.sendMessage( msg );

  logger_->trace( fmt::runtime( "[thread {:s}] sendFunctionCall()~" ), getThreadId() );
}

void InProcConnection::onInterThreadFunctionCall( SFG::Proto::InterThreadFunctionCall const& msg ) {
  void( *functionToCall )() = reinterpret_cast< void(*)() >( msg.functionpointer() );
  logger_->trace( fmt::runtime( "[thread {:s}] onInterThreadFunctionCall(SFG::Proto::InterThreadFunctionCall const& msg = {:p})" ), getThreadId(), static_cast< void* >( functionToCall ) );

  if ( functionToCall != nullptr ) {
    functionToCall();
  } else {
    stopServer();
  }

  logger_->trace( fmt::runtime( "[thread {:s}] onInterThreadFunctionCall()~" ), getThreadId() );
}
}  // namespace SFG
