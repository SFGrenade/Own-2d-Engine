#ifndef _ENGINE_PERFORMANCECONTROLLER_H_
#define _ENGINE_PERFORMANCECONTROLLER_H_

#include "_globals/align.h"
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
  spdlogger logger_;

  UTILS_ALIGN( 64 ) uint64_t counterInputLoops_;
  UTILS_ALIGN( 64 ) std::chrono::high_resolution_clock::time_point counterInputLoopsTimePoint_;
  UTILS_ALIGN( 64 ) uint64_t counterLogicLoops_;
  UTILS_ALIGN( 64 ) std::chrono::high_resolution_clock::time_point counterLogicLoopsTimePoint_;
  UTILS_ALIGN( 64 ) uint64_t counterNetworkLoops_;
  UTILS_ALIGN( 64 ) std::chrono::high_resolution_clock::time_point counterNetworkLoopsTimePoint_;
  UTILS_ALIGN( 64 ) uint64_t counterRenderLoops_;
  UTILS_ALIGN( 64 ) std::chrono::high_resolution_clock::time_point counterRenderLoopsTimePoint_;

  SFG::Engine::SdlWindow* sdlWindow_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_PERFORMANCECONTROLLER_H_ */
