#ifndef _ENGINE_SCRIPTMANAGER_H_
#define _ENGINE_SCRIPTMANAGER_H_

#include <mutex>
#include <vector>

#include "_globals/moreChrono.h"
#include "_globals/sdlInclude.h"
#include "_globals/spdlogInclude.h"


namespace SFG {
namespace Engine {

class Script;
class SdlWindow;
class SdlWindowRenderer;

class ScriptManager {
  public:
  ScriptManager( SFG::Engine::SdlWindow* sdlWindow );
  ~ScriptManager();

  void frame_update( SDL_Renderer* renderer );
  void input_update( SDL_Event const& input );
  void logic_update( std::chrono::secondsLongDouble const& deltaTime );
  void network_update();  // todo

  template < class T >
  T* add_script();

  private:
  spdlogger logger_;

  SFG::Engine::SdlWindow* sdlWindow_;

  std::vector< SFG::Engine::Script* > scripts_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_SCRIPTMANAGER_H_ */
