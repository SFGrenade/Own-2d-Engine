#pragma once

#include <functional>
#include <string>
#include <thread>
#include <zmqPb/pair.hpp>

// Including our headers
#include "_globals/spdlogInclude.h"
#include "inProcMessages.pb.h"

namespace SFG {
class InProcConnection {
  public:
  InProcConnection( std::string const& host, bool server );
  ~InProcConnection();

  void run();

  void sendInProcTestThis( std::string const& t1, std::string const& t2 );

  private:
  void onInProcTestThis( SFG::Proto::InProc::TestThis const& msg );

  private:
  spdlogger logger_;
  static zmq::context_t networkContext_;

  ZmqPb::Pair network_;
};
}  // namespace SFG
