#ifndef _ENGINE_SCRIPT_H_
#define _ENGINE_SCRIPT_H_

#include "_globals/moreChrono.h"
#include "_globals/sdlInclude.h"


namespace SFG {
namespace Engine {

class SdlWindow;

class Script {
  public:
  Script( SFG::Engine::SdlWindow* sdlWindow );
  virtual ~Script();

  virtual void start();

  virtual void frame_update( SDL_Renderer* renderer );
  virtual void input_update( SDL_Event const& input );
  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime );
  virtual void fixed_update();
  virtual void network_update();

  virtual void end();

  protected:
  SFG::Engine::SdlWindow* sdlWindow_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_SCRIPT_H_ */
