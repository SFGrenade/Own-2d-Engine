#pragma once

#include <functional>
#include <thread>
#include <vector>
#include <zmqPb/pubsub.hpp>

// Including our headers
#include "_globals/spdlogInclude.h"
#include "inProcMessages.pb.h"

namespace SFG {
class Network {
  public:
  Network( zmq::context_t* contextToUse );
  ~Network();

  void start();
  void run();
  bool isRunning();

  void add_Stop_Thread_callback( std::function< void( SFG::Proto::InProc::Stop_Thread_Request const& ) > callback );
  void add_Get_Performance_Counters_callback( std::function< void( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) > callback );

  private:
  spdlogger logger_;
  ZmqPb::PubSub network_Network_Send_;
  ZmqPb::PubSub network_Graphics_Receive_;
  ZmqPb::PubSub network_Input_Receive_;
  ZmqPb::PubSub network_Logic_Receive_;
  ZmqPb::PubSub network_Network_Receive_;
  std::unique_ptr< std::thread > workerThread_;
  bool isRunning_;

  uint64_t performanceLoops_;

  std::vector< std::function< void( SFG::Proto::InProc::Stop_Thread_Request const& ) > > stop_thread_callbacks_;
  std::vector< std::function< void( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) > > get_performance_counters_callbacks_;
};
}  // namespace SFG
