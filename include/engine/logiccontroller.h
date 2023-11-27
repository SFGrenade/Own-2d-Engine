#ifndef _ENGINE_LOGICCONTROLLER_H_
#define _ENGINE_LOGICCONTROLLER_H_

#include <mutex>

#include "_globals/spdlogInclude.h"


namespace SFG {
namespace Engine {

class SdlWindow;
class Script;

class LogicController {
  public:
  LogicController( SFG::Engine::SdlWindow* sdlWindow );
  ~LogicController();

  void signal_quit();
  void run_loop();

  template < class T >
  T* add_script();

  private:
  spdlogger logger_;

  SFG::Engine::SdlWindow* sdlWindow_;
  bool done_;
  std::mutex doneMutex_;

  std::vector< SFG::Engine::Script* > scripts_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_LOGICCONTROLLER_H_ */
