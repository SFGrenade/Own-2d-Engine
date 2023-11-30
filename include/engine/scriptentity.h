#ifndef _ENGINE_SCRIPTENTITY_H_
#define _ENGINE_SCRIPTENTITY_H_

#include "_globals/moreChrono.h"
#include "_globals/sdlInclude.h"
#include "engine/script.h"


namespace SFG {
namespace Engine {

class SdlWindow;

struct Vector2 {
  long double x, y;
};

class ScriptEntity : public Script {
  using _base_ = SFG::Engine::Script;
  using _base_::_base_;

  public:
  ScriptEntity();
  virtual ~ScriptEntity();

  virtual void start();

  virtual void frame_update( SDL_Renderer* renderer );
  virtual void input_update( SDL_Event const& input );
  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime );
  virtual void network_update();

  virtual void end();

  protected:
  Vector2 position_;
  Vector2 size_;
  Vector2 velocity_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_SCRIPTENTITY_H_ */