#ifndef _ENGINE_SCRIPTCOLLIDER_H_
#define _ENGINE_SCRIPTCOLLIDER_H_

#include <list>

#include "_globals/moreChrono.h"
#include "_globals/sdlInclude.h"
#include "engine/scriptentity.h"


namespace SFG {
namespace Engine {

class ScriptCollider : public SFG::Engine::ScriptEntity {
  using _base_ = SFG::Engine::ScriptEntity;
  using _base_::_base_;

  public:
  ScriptCollider();
  virtual ~ScriptCollider();

  virtual void start();

  virtual void frame_update( SDL_Renderer* renderer );
  virtual void input_update( SDL_Event const& input );
  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime );
  virtual void network_update();

  virtual void end();

  protected:
  bool isTrigger_;

  std::list< SFG::Engine::ScriptCollider* > enteredCollidersFromAbove_;
  std::list< SFG::Engine::ScriptCollider* > enteredCollidersFromBelow_;
  std::list< SFG::Engine::ScriptCollider* > enteredCollidersFromLeft_;
  std::list< SFG::Engine::ScriptCollider* > enteredCollidersFromRight_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_SCRIPTCOLLIDER_H_ */
