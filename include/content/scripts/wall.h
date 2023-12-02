#ifndef _CONTENT_WALL_H_
#define _CONTENT_WALL_H_

#include "_globals/spdlogInclude.h"
#include "engine/scriptcollider.h"


namespace SFG {
namespace Content {

class Wall : public SFG::Engine::ScriptCollider {
  using _base_ = SFG::Engine::ScriptCollider;
  using _base_::_base_;

  public:
  Wall();
  virtual ~Wall();

  virtual void start();

  virtual void frame_update( SDL_Renderer* renderer );
  virtual void input_update( SDL_Event const& input );
  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime );
  virtual void network_update();  // todo

  virtual void end();

  private:
  spdlogger logger_;

  bool rendering_;
  SDL_Rect wallRect_;
};

}  // namespace Content
}  // namespace SFG

#endif /* _CONTENT_WALL_H_ */
