#ifndef _ENGINE_LOGICCONTROLLER_H_
#define _ENGINE_LOGICCONTROLLER_H_

#include <mutex>

#include "_globals/spdlogInclude.h"


namespace SFG {
namespace Engine {

class SdlWindow;

class LogicController {
  public:
  LogicController( SFG::Engine::SdlWindow* sdlWindow );
  ~LogicController();

  void signal_quit();
  void run_loop();

  private:
  __declspec( align( 64 ) ) spdlogger logger_;

  __declspec( align( 64 ) ) SFG::Engine::SdlWindow* sdlWindow_;
  __declspec( align( 64 ) ) bool done_;
  __declspec( align( 64 ) ) std::mutex doneMutex_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_LOGICCONTROLLER_H_ */
