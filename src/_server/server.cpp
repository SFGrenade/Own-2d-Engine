#include "_server/server.h"

namespace SFG {
Server::Server( uint16_t portReqRep, uint16_t portPubSub )
    : logger_( spdlog::get( "Server" ) ),
      rrNetwork_( fmt::format( fmt::runtime( "tcp://*:{}" ), portReqRep ), true ),
      psNetwork_( fmt::format( fmt::runtime( "tcp://*:{}" ), portPubSub ), true ),
      thread_( nullptr ),
      loop_( false ) {
  logger_->trace( fmt::runtime( "Server()" ) );
  rrNetwork_.subscribe( new SFG::Proto::SendMessageRequest(), [this]( google::protobuf::Message const& message ) {
    this->onSendMessageRequest( static_cast< SFG::Proto::SendMessageRequest const& >( message ) );
  } );
  rrNetwork_.subscribe( new SFG::Proto::GetAllMessagesRequest(), [this]( google::protobuf::Message const& message ) {
    this->onGetAllMessagesRequest( static_cast< SFG::Proto::GetAllMessagesRequest const& >( message ) );
  } );
  logger_->trace( fmt::runtime( "Server()~" ) );
}

Server::~Server() {
  logger_->trace( fmt::runtime( "~Server()" ) );
  if( thread_ ) {
    logger_->trace( fmt::runtime( "~Server - deleting thread" ) );
    if( thread_->joinable() ) {
      thread_->join();
    }
    delete thread_;
    thread_ = nullptr;
  }
  logger_->trace( fmt::runtime( "~Server()~" ) );
}

void Server::startServer() {
  logger_->trace( fmt::runtime( "startServer()" ) );

  loop_ = true;

  thread_ = new std::thread( [this]() {
    this->logger_->trace( fmt::runtime( "thread_()" ) );
    while( this->loop_ ) {
      try {
        this->rrNetwork_.run();
      } catch( std::exception const& e ) {
        this->logger_->error( fmt::runtime( "thread_ - reqRep error: {}" ), e.what() );
        this->rrNetwork_.sendMessage( new SFG::Proto::ReqRepErrorMessage() );  // needed since reqRep is always back and forth
      }
      try {
        this->psNetwork_.run();
      } catch( std::exception const& e ) {
        this->logger_->error( fmt::runtime( "thread_ - pubSub error: {}" ), e.what() );
      }
      std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    }
    this->logger_->trace( fmt::runtime( "thread_()~" ) );
  } );

  logger_->trace( fmt::runtime( "startServer()~" ) );
}

void Server::waitForServer() {
  logger_->trace( fmt::runtime( "waitForServer()" ) );

  if( thread_ ) {
    if( thread_->joinable() ) {
      thread_->join();
    }
  }

  logger_->trace( fmt::runtime( "waitForServer()~" ) );
}

void Server::stopServer() {
  logger_->trace( fmt::runtime( "stopServer()" ) );

  loop_ = false;

  logger_->trace( fmt::runtime( "stopServer()~" ) );
}

void Server::onSendMessageRequest( SFG::Proto::SendMessageRequest const& msg ) {
  logger_->trace( fmt::runtime( "onSendMessageRequest(SFG::Proto::SendMessageRequest const& msg = \"{:s}\"/\"{:s}\")" ), msg.msg().username(), msg.msg().msgtext() );

  SFG::Proto::SendMessageResponse* repMsg = new SFG::Proto::SendMessageResponse();
  if( msg.msg().username().empty() ) {
    repMsg->set_success( false );
    repMsg->set_errormsg( "username is empty!" );
  } else if( msg.msg().msgtext().empty() ) {
    repMsg->set_success( false );
    repMsg->set_errormsg( "msgText is empty!" );
  } else if( messages_.size() >= messages_.max_size() ) {
    repMsg->set_success( false );
    repMsg->set_errormsg( "message storage is maxed out!" );
  } else {
    messages_.push_back( msg.msg() );
    repMsg->set_success( true );
    repMsg->set_errormsg( "" );
  }
  rrNetwork_.sendMessage( repMsg );

  SFG::Proto::AllMessages* pubMsg = new SFG::Proto::AllMessages();
  SFG::Proto::MsgContent* singleMsg;
  for( auto message : messages_ ) {
    singleMsg = pubMsg->add_msglist();
    singleMsg->set_username( message.username() );
    singleMsg->set_msgtext( message.msgtext() );
  }
  psNetwork_.sendMessage( pubMsg );

  logger_->trace( fmt::runtime( "onMessageRequest()~" ) );
}

void Server::onGetAllMessagesRequest( SFG::Proto::GetAllMessagesRequest const& ) {
  logger_->trace( fmt::runtime( "onGetAllMessagesRequest(SFG::Proto::GetAllMessagesRequest const&)" ) );

  SFG::Proto::AllMessages* repMsg = new SFG::Proto::AllMessages();
  SFG::Proto::MsgContent* singleMsg;
  for( auto message : messages_ ) {
    singleMsg = repMsg->add_msglist();
    singleMsg->set_username( message.username() );
    singleMsg->set_msgtext( message.msgtext() );
  }
  rrNetwork_.sendMessage( repMsg );

  logger_->trace( fmt::runtime( "onMessageRequest()~" ) );
}
}  // namespace SFG
