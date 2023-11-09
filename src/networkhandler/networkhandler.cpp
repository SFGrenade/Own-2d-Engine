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
  NetworkHandler::logger->trace( fmt::runtime( "SetReqRepInfo(std::string const& host = \"{:s}\", uint16_t port = {:d})" ), host, port );

  NetworkHandler::hostReqRep = host;
  NetworkHandler::portReqRep = port;

  NetworkHandler::logger->trace( fmt::runtime( "SetReqRepInfo()~" ) );
}

void NetworkHandler::SetPubSubInfo( std::string const& host, uint16_t port ) {
  NetworkHandler::logger->trace( fmt::runtime( "SetPubSubInfo(std::string const& host = \"{:s}\", uint16_t port = {:d})" ), host, port );

  NetworkHandler::hostPubSub = host;
  NetworkHandler::portPubSub = port;

  NetworkHandler::logger->trace( fmt::runtime( "SetPubSubInfo()~" ) );
}

void NetworkHandler::Initialize() {
  NetworkHandler::logger = spdlog::get( "NetworkHandler" );
  NetworkHandler::logger->trace( fmt::runtime( "Initialize()" ) );
  NetworkHandler::logger->trace( fmt::runtime( "Initialize()~" ) );
}

void NetworkHandler::InitializeNetwork() {
  NetworkHandler::logger->trace( fmt::runtime( "InitializeNetwork()" ) );

  NetworkHandler::rrNetwork = new ZmqPb::ReqRep( fmt::format( fmt::runtime( "{:s}:{:d}" ), NetworkHandler::hostReqRep, NetworkHandler::portReqRep ), false );
  NetworkHandler::psNetwork = new ZmqPb::PubSub( fmt::format( fmt::runtime( "{:s}:{:d}" ), NetworkHandler::hostPubSub, NetworkHandler::portPubSub ), false );
  NetworkHandler::rrNetwork->subscribe( new SFG::Proto::ReqRepErrorMessage(), []( google::protobuf::Message const& ) {} );

  NetworkHandler::rrNetwork->subscribe( new SFG::Proto::SendMessageResponse(), []( google::protobuf::Message const& message ) {
    NetworkHandler::onSendMessageResponse( static_cast< SFG::Proto::SendMessageResponse const& >( message ) );
  } );
  NetworkHandler::rrNetwork->subscribe( new SFG::Proto::AllMessages(), []( google::protobuf::Message const& message ) {
    NetworkHandler::onAllMessages( static_cast< SFG::Proto::AllMessages const& >( message ) );
  } );
  NetworkHandler::psNetwork->subscribe( new SFG::Proto::AllMessages(), []( google::protobuf::Message const& message ) {
    NetworkHandler::onAllMessages( static_cast< SFG::Proto::AllMessages const& >( message ) );
  } );

  NetworkHandler::logger->trace( fmt::runtime( "InitializeNetwork()~" ) );
}

void NetworkHandler::Destroy() {
  NetworkHandler::logger->trace( fmt::runtime( "Destroy()" ) );
  NetworkHandler::logger->trace( fmt::runtime( "Destroy()~" ) );
}

void NetworkHandler::RunNetwork() {
  // NetworkHandler::logger->trace( fmt::runtime( "RunNetwork()" ) );

  NetworkHandler::rrNetwork->run();
  NetworkHandler::psNetwork->run();

  // NetworkHandler::logger->trace( fmt::runtime( "RunNetwork()~" ) );
}

void NetworkHandler::SendMessageRequest( std::string const& username, std::string const& messageContent ) {
  NetworkHandler::logger->trace( fmt::runtime( "SendMessageRequest(std::string const& username = \"{:s}\", std::string const& messageContent = \"{:s}\")" ),
                                 username,
                                 messageContent );

  SFG::Proto::SendMessageRequest* msg = new SFG::Proto::SendMessageRequest();
  SFG::Proto::MsgContent* actualMsg = new SFG::Proto::MsgContent();
  actualMsg->set_username( username );
  actualMsg->set_msgtext( messageContent );
  msg->set_allocated_msg( actualMsg );
  NetworkHandler::rrNetwork->sendMessage( msg );

  NetworkHandler::logger->trace( fmt::runtime( "SendMessageRequest()~" ) );
}

void NetworkHandler::RequestAllMessage() {
  NetworkHandler::logger->trace( fmt::runtime( "RequestAllMessage()" ) );

  SFG::Proto::GetAllMessagesRequest* msg = new SFG::Proto::GetAllMessagesRequest();
  NetworkHandler::rrNetwork->sendMessage( msg );

  NetworkHandler::logger->trace( fmt::runtime( "RequestAllMessage()~" ) );
}

void NetworkHandler::onSendMessageResponse( SFG::Proto::SendMessageResponse const& msg ) {
  NetworkHandler::logger->trace( fmt::runtime( "onSendMessageResponse(SFG::Proto::SendMessageResponse const& msg)" ) );

  if( !msg.success() ) {
    NetworkHandler::logger->error( fmt::runtime( "onSendMessageResponse - error sending message: {}" ), msg.errormsg() );
  }

  NetworkHandler::logger->trace( fmt::runtime( "onSendMessageResponse()~" ) );
}

void NetworkHandler::onAllMessages( SFG::Proto::AllMessages const& msg ) {
  NetworkHandler::logger->trace( fmt::runtime( "onAllMessages(SFG::Proto::AllMessages const& msg)" ) );

  if( msg.msglist_size() > 0 ) {
    NetworkHandler::logger->trace( fmt::runtime( "onAllMessages - Messages:" ) );
    for( auto message : msg.msglist() ) {
      NetworkHandler::logger->trace( fmt::runtime( "onAllMessages - - \"{:s}\" - \"{:s}\"" ), message.username(), message.msgtext() );
    }
  }

  NetworkHandler::logger->trace( fmt::runtime( "onAllMessages()~" ) );
}
}  // namespace SFG
