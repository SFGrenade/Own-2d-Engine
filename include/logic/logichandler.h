#pragma once

#include <thread>
#include <vector>

// Including our headers
#include "_globals/spdlogInclude.h"
#include "logic/timer.h"

namespace SFG {
class LogicHandler {
  private:
  void deleteTimer( Timer* ptr );

  public:
  LogicHandler();
  ~LogicHandler();

  void StartLogic();
  void UpdateLogic();
  void AddTimer( TimerCallback callback, std::chrono::nanoseconds interval, bool returnInterval );

  private:
  spdlogger logger_;
  std::chrono::time_point< std::chrono::high_resolution_clock > old_;
  std::vector< std::shared_ptr< Timer > > timers_;
};
}  // namespace SFG
