#pragma once

#include <zmqPb/pubSub.hpp>
#include <zmqPb/reqRep.hpp>

// Including our headers
#include "globals.h"
#include "networkMessages.pb.h"

namespace SFG {
class Server {
  public:
  Server( uint16_t portReqRep, uint16_t portPubSub );
  ~Server();

  void startServer();
  void waitForServer();
  void stopServer();

  private:
  void onSendMessageRequest( SFG::Proto::SendMessageRequest const& msg );
  void onGetAllMessagesRequest( SFG::Proto::GetAllMessagesRequest const& msg );

  private:
  spdlogger logger_;

  ZmqPb::ReqRep rrNetwork_;
  ZmqPb::PubSub psNetwork_;
  std::thread* thread_;
  bool loop_;

  std::vector< SFG::Proto::MsgContent > messages_;
};
}  // namespace SFG
