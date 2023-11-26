#ifndef _ENGINE_PERFORMANCECONTROLLER_H_
#define _ENGINE_PERFORMANCECONTROLLER_H_

#include <atomic>

#include "_globals/moreChrono.h"
#include "_globals/spdlogInclude.h"


namespace SFG {
namespace Engine {

class SdlWindow;

class PerformanceController {
  public:
  PerformanceController( SFG::Engine::SdlWindow* sdlWindow );
  ~PerformanceController();

  void incInputLoops();
  long double getInputLoops();
  void incLogicLoops();
  long double getLogicLoops();
  void incNetworkLoops();
  long double getNetworkLoops();
  void incRenderLoops();
  long double getRenderLoops();

  private:
  __declspec( align( 64 ) ) spdlogger logger_;

  __declspec( align( 64 ) ) std::atomic< uint64_t > counterInputLoops_;
  __declspec( align( 64 ) ) std::chrono::high_resolution_clock::time_point counterInputLoopsTimePoint_;
  __declspec( align( 64 ) ) std::atomic< uint64_t > counterLogicLoops_;
  __declspec( align( 64 ) ) std::chrono::high_resolution_clock::time_point counterLogicLoopsTimePoint_;
  __declspec( align( 64 ) ) std::atomic< uint64_t > counterNetworkLoops_;
  __declspec( align( 64 ) ) std::chrono::high_resolution_clock::time_point counterNetworkLoopsTimePoint_;
  __declspec( align( 64 ) ) std::atomic< uint64_t > counterRenderLoops_;
  __declspec( align( 64 ) ) std::chrono::high_resolution_clock::time_point counterRenderLoopsTimePoint_;

  __declspec( align( 64 ) ) SFG::Engine::SdlWindow* sdlWindow_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_PERFORMANCECONTROLLER_H_ */
