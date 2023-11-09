#pragma once

#include <functional>
#include <string>
#include <thread>
#include <zmqPb/pair.hpp>

// Including our headers
#include "_globals/spdlogInclude.h"
#include "networkMessages.pb.h"

namespace SFG {
class InProcConnection {
  public:
  InProcConnection( std::string const& host, bool server );
  ~InProcConnection();

  void run();

  void sendFunctionCall( std::function< void() > functionToSend );

  private:
  void onInterThreadFunctionCall( SFG::Proto::InterThreadFunctionCall const& msg );

  private:
  spdlogger logger_;
  static zmq::context_t networkContext_;

  ZmqPb::Pair network_;
};
}  // namespace SFG
