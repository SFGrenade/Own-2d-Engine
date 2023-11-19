#pragma once

#include <functional>
#include <vector>
#include <zmqPb/pubsub.hpp>

// Including our headers
#include "_globals/spdlogInclude.h"
#include "inProcMessages.pb.h"

namespace SFG {
class Window;

class Graphics {
  public:
  Graphics( zmq::context_t* contextToUse );
  ~Graphics();

  void start();
  void run();
  bool isRunning();

  void add_Stop_Thread_callback( std::function< void( SFG::Proto::InProc::Stop_Thread_Request const& ) > callback );
  void add_Update_Performance_Information_callback( std::function< void( SFG::Proto::InProc::Update_Performance_Information_Request const& ) > callback );
  void add_Get_Performance_Counters_callback( std::function< void( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) > callback );

  private:
  spdlogger logger_;
  ZmqPb::PubSub network_Graphics_Send_;
  ZmqPb::PubSub network_Graphics_Receive_;
  ZmqPb::PubSub network_Input_Receive_;
  ZmqPb::PubSub network_Logic_Receive_;
  ZmqPb::PubSub network_Network_Receive_;
  bool isRunning_;

  std::shared_ptr< Window > window_;
  uint64_t performanceLoops_;

  std::vector< std::function< void( SFG::Proto::InProc::Stop_Thread_Request const& ) > > stop_thread_callbacks_;
  std::vector< std::function< void( SFG::Proto::InProc::Update_Performance_Information_Request const& ) > > update_performance_information_callbacks_;
  std::vector< std::function< void( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) > > get_performance_counters_callbacks_;
};
}  // namespace SFG
