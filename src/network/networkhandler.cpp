#include "network/networkhandler.h"

namespace SFG {
NetworkHandler::NetworkHandler()
    : logger_( spdlog::get( "NetworkHandler" ) ),
      hostReqRep_( "" ),
      portReqRep_( 0 ),
      hostPubSub_( "" ),
      portPubSub_( 0 ),
      rrNetwork_( nullptr ),
      psNetwork_( nullptr ) {
  this->logger_->trace( fmt::runtime( "NetworkHandler()" ) );

  this->logger_->trace( fmt::runtime( "NetworkHandler()~" ) );
}

NetworkHandler::~NetworkHandler() {
  this->logger_->trace( fmt::runtime( "~NetworkHandler()" ) );

  if( this->rrNetwork_ )
    delete rrNetwork_;
  if( this->psNetwork_ )
    delete psNetwork_;

  this->logger_->trace( fmt::runtime( "~NetworkHandler()~" ) );
}

void NetworkHandler::SetReqRepInfo( std::string const& host, uint16_t port ) {
  this->logger_->trace( fmt::runtime( "SetReqRepInfo( host = \"{:s}\", port = {:d} )" ), host, port );

  this->hostReqRep_ = host;
  this->portReqRep_ = port;

  this->logger_->trace( fmt::runtime( "SetReqRepInfo()~" ) );
}

void NetworkHandler::SetPubSubInfo( std::string const& host, uint16_t port ) {
  this->logger_->trace( fmt::runtime( "SetPubSubInfo( host = \"{:s}\", port = {:d} )" ), host, port );

  this->hostPubSub_ = host;
  this->portPubSub_ = port;

  this->logger_->trace( fmt::runtime( "SetPubSubInfo()~" ) );
}

void NetworkHandler::InitializeNetwork() {
  this->logger_->trace( fmt::runtime( "InitializeNetwork()" ) );

  this->rrNetwork_ = new ZmqPb::ReqRep( fmt::format( fmt::runtime( "{:s}:{:d}" ), this->hostReqRep_, this->portReqRep_ ), false );
  this->psNetwork_ = new ZmqPb::PubSub( fmt::format( fmt::runtime( "{:s}:{:d}" ), this->hostPubSub_, this->portPubSub_ ), false );
  this->rrNetwork_->subscribe( new SFG::Proto::ReqRepErrorMessage(), []( google::protobuf::Message const& ) {} );

  this->rrNetwork_->subscribe( new SFG::Proto::SendMessageResponse(), [this]( google::protobuf::Message const& message ) {
    this->onSendMessageResponse( static_cast< SFG::Proto::SendMessageResponse const& >( message ) );
  } );
  this->rrNetwork_->subscribe( new SFG::Proto::AllMessages(), [this]( google::protobuf::Message const& message ) {
    this->onAllMessages( static_cast< SFG::Proto::AllMessages const& >( message ) );
  } );
  this->psNetwork_->subscribe( new SFG::Proto::AllMessages(), [this]( google::protobuf::Message const& message ) {
    this->onAllMessages( static_cast< SFG::Proto::AllMessages const& >( message ) );
  } );

  this->logger_->trace( fmt::runtime( "InitializeNetwork()~" ) );
}

void NetworkHandler::RunNetwork() {
  // this->logger_->trace( fmt::runtime( "RunNetwork()" ) );

  this->rrNetwork_->run();
  this->psNetwork_->run();

  // this->logger_->trace( fmt::runtime( "RunNetwork()~" ) );
}

void NetworkHandler::SendMessageRequest( std::string const& username, std::string const& messageContent ) {
  this->logger_->trace( fmt::runtime( "SendMessageRequest( username = \"{:s}\", messageContent = \"{:s}\" )" ), username, messageContent );

  SFG::Proto::SendMessageRequest* msg = new SFG::Proto::SendMessageRequest();
  SFG::Proto::MsgContent* actualMsg = new SFG::Proto::MsgContent();
  actualMsg->set_username( username );
  actualMsg->set_msgtext( messageContent );
  msg->set_allocated_msg( actualMsg );
  this->rrNetwork_->sendMessage( msg );

  this->logger_->trace( fmt::runtime( "SendMessageRequest()~" ) );
}

void NetworkHandler::RequestAllMessage() {
  this->logger_->trace( fmt::runtime( "RequestAllMessage()" ) );

  SFG::Proto::GetAllMessagesRequest* msg = new SFG::Proto::GetAllMessagesRequest();
  this->rrNetwork_->sendMessage( msg );

  this->logger_->trace( fmt::runtime( "RequestAllMessage()~" ) );
}

void NetworkHandler::onSendMessageResponse( SFG::Proto::SendMessageResponse const& msg ) {
  this->logger_->trace( fmt::runtime( "onSendMessageResponse( msg )" ) );

  if( !msg.success() ) {
    this->logger_->error( fmt::runtime( "onSendMessageResponse - error sending message: {}" ), msg.errormsg() );
  }

  this->logger_->trace( fmt::runtime( "onSendMessageResponse()~" ) );
}

void NetworkHandler::onAllMessages( SFG::Proto::AllMessages const& msg ) {
  this->logger_->trace( fmt::runtime( "onAllMessages( msg )" ) );

  if( msg.msglist_size() > 0 ) {
    this->logger_->trace( fmt::runtime( "onAllMessages - Messages:" ) );
    for( auto message : msg.msglist() ) {
      this->logger_->trace( fmt::runtime( "onAllMessages - - \"{:s}\" - \"{:s}\"" ), message.username(), message.msgtext() );
    }
  }

  this->logger_->trace( fmt::runtime( "onAllMessages()~" ) );
}
}  // namespace SFG
