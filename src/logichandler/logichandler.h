#pragma once

// Including our headers
#include "../globals.h"
#include "timer.h"

namespace SFG {
class LogicHandler {
  private:
  static spdlogger logger;
  static std::vector< std::shared_ptr< Timer > > timers;
  static std::shared_ptr< bool > quitFlag;
  static std::thread logicThread;

  static void deleteTimer( Timer* ptr );
  static void UpdateLogic();

  public:
  static void Initialize();
  static void Destroy();

  static void SetQuitFlag( std::shared_ptr< bool > quitFlag );
  static void StartLogic();
  static void StopLogic();
  static void AddTimer( TimerCallback callback, std::chrono::nanoseconds interval, bool returnInterval );
};
}  // namespace SFG
