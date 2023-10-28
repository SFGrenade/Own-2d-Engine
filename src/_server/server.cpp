#include "_server/server.h"

namespace SFG {
Server::Server( uint16_t portReqRep, uint16_t portPubSub )
    : logger_( spdlog::get( "Server" ) ),
      rrNetwork_( fmt::format( "tcp://*:{}", portReqRep ), true ),
      psNetwork_( fmt::format( "tcp://*:{}", portPubSub ), true ),
      thread_( nullptr ),
      loop_( false ) {
  logger_->trace( "Server()" );
  rrNetwork_.subscribe( new SFG::Proto::SendMessageRequest(), [this]( google::protobuf::Message const& message ) {
    this->onSendMessageRequest( static_cast< SFG::Proto::SendMessageRequest const& >( message ) );
  } );
  rrNetwork_.subscribe( new SFG::Proto::GetAllMessagesRequest(), [this]( google::protobuf::Message const& message ) {
    this->onGetAllMessagesRequest( static_cast< SFG::Proto::GetAllMessagesRequest const& >( message ) );
  } );
  logger_->trace( "Server()~" );
}

Server::~Server() {
  logger_->trace( "~Server()" );
  if( thread_ ) {
    logger_->trace( "~Server - deleting thread" );
    if( thread_->joinable() ) {
      thread_->join();
    }
    delete thread_;
    thread_ = nullptr;
  }
  logger_->trace( "~Server()~" );
}

void Server::startServer() {
  logger_->trace( "startServer()" );

  loop_ = true;

  thread_ = new std::thread( [this]() {
    this->logger_->trace( "thread_()" );
    while( this->loop_ ) {
      try {
        this->rrNetwork_.run();
      } catch( std::exception const& e ) {
        this->logger_->error( "thread_ - reqRep error: {}", e.what() );
        this->rrNetwork_.sendMessage( new SFG::Proto::ReqRepErrorMessage() );  // needed since reqRep is always back and forth
      }
      try {
        this->psNetwork_.run();
      } catch( std::exception const& e ) {
        this->logger_->error( "thread_ - pubSub error: {}", e.what() );
      }
      std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    }
    this->logger_->trace( "thread_()~" );
  } );

  logger_->trace( "startServer()~" );
}

void Server::waitForServer() {
  logger_->trace( "waitForServer()" );

  if( thread_ ) {
    if( thread_->joinable() ) {
      thread_->join();
    }
  }

  logger_->trace( "waitForServer()~" );
}

void Server::stopServer() {
  logger_->trace( "stopServer()" );

  loop_ = false;

  logger_->trace( "stopServer()~" );
}

void Server::onSendMessageRequest( SFG::Proto::SendMessageRequest const& msg ) {
  logger_->trace( "onSendMessageRequest(SFG::Proto::SendMessageRequest const& msg = \"{:s}\"/\"{:s}\")", msg.msg().username(), msg.msg().msgtext() );

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

  logger_->trace( "onMessageRequest()~" );
}

void Server::onGetAllMessagesRequest( SFG::Proto::GetAllMessagesRequest const& msg ) {
  logger_->trace( "onGetAllMessagesRequest(SFG::Proto::GetAllMessagesRequest const& msg)" );

  SFG::Proto::AllMessages* repMsg = new SFG::Proto::AllMessages();
  SFG::Proto::MsgContent* singleMsg;
  for( auto message : messages_ ) {
    singleMsg = repMsg->add_msglist();
    singleMsg->set_username( message.username() );
    singleMsg->set_msgtext( message.msgtext() );
  }
  rrNetwork_.sendMessage( repMsg );

  logger_->trace( "onMessageRequest()~" );
}
}  // namespace SFG
