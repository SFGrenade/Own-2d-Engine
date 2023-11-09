#include "_globals/misc.h"
#include "_inproc/inprocconnection.h"

namespace SFG {
zmq::context_t InProcConnection::networkContext_ = zmq::context_t( 0 );

InProcConnection::InProcConnection( std::string const& host, bool server )
  : logger_( spdlog::get( "TSrv" ) ),
    network_( host, server, &InProcConnection::networkContext_ ) {
  logger_->trace( fmt::runtime( "[thread {:s}] InProcConnection(std::string const& host = \"{:s}\", bool server = {})" ), getThreadId(), host, server );
  network_.subscribe( new SFG::Proto::InProc::TestThis(), [this]( google::protobuf::Message const& message ) {
    this->onInProcTestThis( static_cast< SFG::Proto::InProc::TestThis const& >( message ) );
  } );
  logger_->trace( fmt::runtime( "[thread {:s}] InProcConnection()~" ), getThreadId() );
}

InProcConnection::~InProcConnection() {
  logger_->trace( fmt::runtime( "[thread {:s}] ~InProcConnection()" ), getThreadId() );

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

void InProcConnection::sendInProcTestThis( std::string const& t1, std::string const& t2 ) {
  logger_->trace( fmt::runtime( "[thread {:s}] sendInProcTestThis(std::string const& t1 = \"{:s}\", std::string const& t2 = \"{:s}\")" ), getThreadId(), t1, t2 );

  SFG::Proto::InProc::TestThis* msg = new SFG::Proto::InProc::TestThis();
  msg->set_username( t1 );
  msg->set_msgtext( t2 );
  network_.sendMessage( msg );

  logger_->trace( fmt::runtime( "[thread {:s}] sendFunctionCall()~" ), getThreadId() );
}

void InProcConnection::onInProcTestThis( SFG::Proto::InProc::TestThis const& msg ) {
  logger_->trace( fmt::runtime( "[thread {:s}] onInterThreadFunctionCall(SFG::Proto::InProc::TestThis const& msg = \"{:s}\"/\"{:s}\")" ), getThreadId(), msg.username(), msg.msgtext() );

  logger_->trace( fmt::runtime( "[thread {:s}] onInterThreadFunctionCall()~" ), getThreadId() );
}
}  // namespace SFG
