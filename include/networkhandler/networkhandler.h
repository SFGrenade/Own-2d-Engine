#pragma once

#include <cstdint>
#include <string>
#include <zmqPb/pubSub.hpp>
#include <zmqPb/reqRep.hpp>

// Including our headers
#include "_globals/spdlogInclude.h"
#include "networkMessages.pb.h"

namespace SFG {
class NetworkHandler {
  private:
  static spdlogger logger;
  static std::string hostReqRep;
  static uint16_t portReqRep;
  static std::string hostPubSub;
  static uint16_t portPubSub;
  static ZmqPb::ReqRep* rrNetwork;
  static ZmqPb::PubSub* psNetwork;

  public:
  static void SetReqRepInfo( std::string const& host, uint16_t port );
  static void SetPubSubInfo( std::string const& host, uint16_t port );

  static void Initialize();
  static void InitializeNetwork();
  static void Destroy();

  static void RunNetwork();

  static void SendMessageRequest( std::string const& username, std::string const& messageContent );
  static void RequestAllMessage();

  private:
  static void onSendMessageResponse( SFG::Proto::SendMessageResponse const& msg );
  static void onAllMessages( SFG::Proto::AllMessages const& msg );
};
}  // namespace SFG
