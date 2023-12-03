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

  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime );

  void set_position( long double x, long double y );
  void set_size( long double w, long double h );
  void set_velocity( long double x, long double y );

  protected:
  Vector2 position_;
  Vector2 size_;
  Vector2 velocity_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_SCRIPTENTITY_H_ */
