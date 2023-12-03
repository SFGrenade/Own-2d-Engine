#ifndef _CONTENT_WALL_H_
#define _CONTENT_WALL_H_

#include "_globals/sdlInclude.h"
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

  virtual void frame_update( SDL_Renderer* renderer );
  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime );

  private:
  spdlogger logger_;

  bool rendering_;
  __declspec( align( 64 ) ) SDL_Rect wallRect_;  // used for graphics and logic
  SDL_Texture* wallTextureCollision_;
  SDL_Texture* wallTextureNoCollision_;
};

}  // namespace Content
}  // namespace SFG

#endif /* _CONTENT_WALL_H_ */
