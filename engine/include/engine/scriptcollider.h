#ifndef _ENGINE_SCRIPTCOLLIDER_H_
#define _ENGINE_SCRIPTCOLLIDER_H_

#include "_globals/moreChrono.h"
#include "_globals/sdlInclude.h"
#include "engine/scriptentity.h"


namespace SFG {
namespace Engine {

enum class InteractSide { Top, Left, Right, Bottom };

class ScriptCollider : public SFG::Engine::ScriptEntity {
  using _base_ = SFG::Engine::ScriptEntity;
  using _base_::_base_;

  public:
  ScriptCollider();
  virtual ~ScriptCollider();

  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime ) override;

  virtual void interact( SFG::Engine::ScriptCollider const* contact, SFG::Engine::InteractSide interactionSide );

  protected:
  bool isStatic_;
  bool isTrigger_;
  bool isTouching_;
  bool isColliding_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_SCRIPTCOLLIDER_H_ */
