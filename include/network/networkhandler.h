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
  public:
  NetworkHandler();
  ~NetworkHandler();

  void SetReqRepInfo( std::string const& host, uint16_t port );
  void SetPubSubInfo( std::string const& host, uint16_t port );
  void InitializeNetwork();

  void RunNetwork();

  void SendMessageRequest( std::string const& username, std::string const& messageContent );
  void RequestAllMessage();

  private:
  void onSendMessageResponse( SFG::Proto::SendMessageResponse const& msg );
  void onAllMessages( SFG::Proto::AllMessages const& msg );

  private:
  spdlogger logger_;
  std::string hostReqRep_;
  uint16_t portReqRep_;
  std::string hostPubSub_;
  uint16_t portPubSub_;
  ZmqPb::ReqRep* rrNetwork_;
  ZmqPb::PubSub* psNetwork_;
};
}  // namespace SFG
