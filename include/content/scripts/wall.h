#ifndef _CONTENT_WALL_H_
#define _CONTENT_WALL_H_

#include "_globals/align.h"
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

  virtual void frame_update( SDL_Renderer* renderer ) override;
  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime ) override;

  private:
  spdlogger logger_;

  bool rendering_;
  SFG_ENGINE_ALIGN( 64 ) SDL_Rect wallRect_;  // used for graphics and logic
  SDL_Texture* wallTexture_;
};

}  // namespace Content
}  // namespace SFG

#endif /* _CONTENT_WALL_H_ */
