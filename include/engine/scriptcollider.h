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

  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime ) override;

  protected:
  bool isTrigger_;
  bool isTouching_;
  bool isColliding_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_SCRIPTCOLLIDER_H_ */
