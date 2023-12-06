#ifndef _ENGINE_SCRIPTENTITY_H_
#define _ENGINE_SCRIPTENTITY_H_

#include "_globals/moreChrono.h"
#include "_globals/sdlInclude.h"
#include "engine/script.h"
#include "engine/vector2.h"


namespace SFG {
namespace Engine {

class SdlWindow;

class ScriptEntity : public Script {
  using _base_ = SFG::Engine::Script;
  using _base_::_base_;

  public:
  ScriptEntity();
  virtual ~ScriptEntity();

  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime ) override;

  void set_position( long double x, long double y );
  void set_size( long double w, long double h );
  void set_velocity( long double x, long double y );

  long double get_top();
  long double get_bottom();
  long double get_left();
  long double get_right();
  SFG::Engine::Vector2 get_middle();

  protected:
  SFG::Engine::Vector2 position_;
  SFG::Engine::Vector2 size_;
  SFG::Engine::Vector2 velocity_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_SCRIPTENTITY_H_ */
