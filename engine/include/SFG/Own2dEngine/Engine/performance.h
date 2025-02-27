#ifndef ENGINE_PERFORMANCE_H_
#define ENGINE_PERFORMANCE_H_

#include <SFG/Own2dEngine/Logger/_include.h>
#include <SFG/Own2dEngine/Utils/_include.h>
#include <chrono>
#include <map>
#include <string>

namespace SFG {
namespace Own2dEngine {
namespace Engine {

class Performance {
  private:
  struct PerformanceData {
    std::chrono::high_resolution_clock::time_point timingPoint;
    bool initialized;
  };

  public:
  static void init();

  static void startProgram();
  static std::chrono::secondsLongDouble endProgram();

  static void startTiming( std::string const& category );
  static std::chrono::secondsLongDouble endTiming( std::string const& category );

  private:
  static SFG::Own2dEngine::Logger::spdlogger logger_;
  static std::map< std::string, PerformanceData > timingPoints_;
};

}  // namespace Engine
}  // namespace Own2dEngine
}  // namespace SFG

#endif /* ENGINE_PERFORMANCE_H_ */
