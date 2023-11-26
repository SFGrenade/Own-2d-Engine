#ifndef _ENGINE_PERFORMANCECONTROLLER_H_
#define _ENGINE_PERFORMANCECONTROLLER_H_

#include <atomic>

#include "_globals/moreChrono.h"
#include "_globals/spdlogInclude.h"

#define CACHE_LINE 64
#define CACHE_ALIGN __declspec( align( CACHE_LINE ) )

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

  CACHE_ALIGN std::atomic< uint64_t > counterInputLoops_;
  CACHE_ALIGN std::chrono::high_resolution_clock::time_point counterInputLoopsTimePoint_;
  CACHE_ALIGN std::atomic< uint64_t > counterLogicLoops_;
  CACHE_ALIGN std::chrono::high_resolution_clock::time_point counterLogicLoopsTimePoint_;
  CACHE_ALIGN std::atomic< uint64_t > counterNetworkLoops_;
  CACHE_ALIGN std::chrono::high_resolution_clock::time_point counterNetworkLoopsTimePoint_;
  CACHE_ALIGN std::atomic< uint64_t > counterRenderLoops_;
  CACHE_ALIGN std::chrono::high_resolution_clock::time_point counterRenderLoopsTimePoint_;

  SFG::Engine::SdlWindow* sdlWindow_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_PERFORMANCECONTROLLER_H_ */
