#include "networkhandler/networkhandler.h"

namespace SFG {
spdlogger NetworkHandler::logger = nullptr;
std::string NetworkHandler::hostReqRep = "";
uint16_t NetworkHandler::portReqRep = 0;
std::string NetworkHandler::hostPubSub = "";
uint16_t NetworkHandler::portPubSub = 0;
ZmqPb::ReqRep* NetworkHandler::rrNetwork = nullptr;
ZmqPb::PubSub* NetworkHandler::psNetwork = nullptr;

void NetworkHandler::SetReqRepInfo( std::string const& host, uint16_t port ) {
  NetworkHandler::logger->trace( "SetReqRepInfo(std::string const& host = \"{:s}\", uint16_t port = {:d})", host, port );

  NetworkHandler::hostReqRep = host;
  NetworkHandler::portReqRep = port;

  NetworkHandler::logger->trace( "SetReqRepInfo()~" );
}

void NetworkHandler::SetPubSubInfo( std::string const& host, uint16_t port ) {
  NetworkHandler::logger->trace( "SetPubSubInfo(std::string const& host = \"{:s}\", uint16_t port = {:d})", host, port );

  NetworkHandler::hostPubSub = host;
  NetworkHandler::portPubSub = port;

  NetworkHandler::logger->trace( "SetPubSubInfo()~" );
}

void NetworkHandler::Initialize() {
  NetworkHandler::logger = spdlog::get( "NetworkHandler" );
  NetworkHandler::logger->trace( "Initialize()" );
  NetworkHandler::logger->trace( "Initialize()~" );
}

void NetworkHandler::InitializeNetwork() {
  NetworkHandler::logger->trace( "InitializeNetwork()" );

  NetworkHandler::rrNetwork = new ZmqPb::ReqRep( NetworkHandler::hostReqRep, NetworkHandler::portReqRep, false );
  NetworkHandler::psNetwork = new ZmqPb::PubSub( NetworkHandler::hostPubSub, NetworkHandler::portPubSub, false );
  NetworkHandler::rrNetwork->subscribe( new SFG::Proto::SendMessageResponse(), []( google::protobuf::Message const& message ) {
    NetworkHandler::onSendMessageResponse( static_cast< SFG::Proto::SendMessageResponse const& >( message ) );
  } );
  NetworkHandler::rrNetwork->subscribe( new SFG::Proto::AllMessages(), []( google::protobuf::Message const& message ) {
    NetworkHandler::onAllMessages( static_cast< SFG::Proto::AllMessages const& >( message ) );
  } );
  NetworkHandler::psNetwork->subscribe( new SFG::Proto::AllMessages(), []( google::protobuf::Message const& message ) {
    NetworkHandler::onAllMessages( static_cast< SFG::Proto::AllMessages const& >( message ) );
  } );

  NetworkHandler::logger->trace( "InitializeNetwork()~" );
}

void NetworkHandler::Destroy() {
  NetworkHandler::logger->trace( "Destroy()" );
  NetworkHandler::logger->trace( "Destroy()~" );
}

void NetworkHandler::RunNetwork() {
  // NetworkHandler::logger->trace( "RunNetwork()" );

  NetworkHandler::rrNetwork->run();
  NetworkHandler::psNetwork->run();

  // NetworkHandler::logger->trace( "RunNetwork()~" );
}

void NetworkHandler::SendMessageRequest( std::string const& username, std::string const& messageContent ) {
  NetworkHandler::logger->trace( "SendMessageRequest(std::string const& username = \"{:s}\", std::string const& messageContent = \"{:s}\")",
                                 username,
                                 messageContent );

  SFG::Proto::SendMessageRequest* msg = new SFG::Proto::SendMessageRequest();
  SFG::Proto::MsgContent* actualMsg = new SFG::Proto::MsgContent();
  actualMsg->set_username( username );
  actualMsg->set_msgtext( messageContent );
  msg->set_allocated_msg( actualMsg );
  NetworkHandler::rrNetwork->sendMessage( msg );

  NetworkHandler::logger->trace( "SendMessageRequest()~" );
}

void NetworkHandler::RequestAllMessage() {
  NetworkHandler::logger->trace( "RequestAllMessage()" );

  SFG::Proto::GetAllMessagesRequest* msg = new SFG::Proto::GetAllMessagesRequest();
  NetworkHandler::rrNetwork->sendMessage( msg );

  NetworkHandler::logger->trace( "RequestAllMessage()~" );
}

void NetworkHandler::onSendMessageResponse( SFG::Proto::SendMessageResponse const& msg ) {
  NetworkHandler::logger->trace( "onSendMessageResponse(SFG::Proto::SendMessageResponse const& msg)" );

  if( !msg.success() ) {
    NetworkHandler::logger->error( "onSendMessageResponse - error sending message!" );
  }

  NetworkHandler::logger->trace( "onSendMessageResponse()~" );
}

void NetworkHandler::onAllMessages( SFG::Proto::AllMessages const& msg ) {
  NetworkHandler::logger->trace( "onAllMessages(SFG::Proto::AllMessages const& msg)" );

  if( msg.msglist_size() > 0 ) {
    NetworkHandler::logger->trace( "onAllMessages - Messages:" );
    for( auto message : msg.msglist() ) {
      NetworkHandler::logger->trace( "onAllMessages - - \"{:s}\" - \"{:s}\"", message.username(), message.msgtext() );
    }
  }

  NetworkHandler::logger->trace( "onAllMessages()~" );
}
}  // namespace SFG
