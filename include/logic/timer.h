#pragma once

#include <functional>
#include <optional>

// Including our headers
#include "_globals/moreChrono.h"
#include "_globals/spdlogInclude.h"

namespace SFG {
typedef std::function< void( std::optional< std::chrono::secondsLongDouble > interval ) > TimerCallback;

class Timer {
  public:
  Timer( TimerCallback callback, std::chrono::nanoseconds interval, bool returnInterval );
  ~Timer();

  void UpdateTimer( std::chrono::nanoseconds duration );

  private:
  spdlogger logger;
  TimerCallback timerCallback;
  std::chrono::nanoseconds timerInterval;
  std::chrono::nanoseconds timerCurrent;
  bool timerReturnInterval;
};
}  // namespace SFG
