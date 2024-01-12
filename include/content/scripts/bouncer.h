#ifndef _CONTENT_BOUNCER_H_
#define _CONTENT_BOUNCER_H_

#include "_globals/sdlInclude.h"
#include "_globals/spdlogInclude.h"
#include "engine/scriptcollider.h"


namespace SFG {
namespace Content {

class Bouncer : public SFG::Engine::ScriptCollider {
  using _base_ = SFG::Engine::ScriptCollider;
  using _base_::_base_;

  public:
  Bouncer();
  virtual ~Bouncer();

  virtual void frame_update( SDL_Renderer* renderer ) override;
  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime ) override;

  void set_speed( long double x, long double y );

  private:
  spdlogger logger_;

  bool rendering_;
  __declspec( align( 64 ) ) SDL_Rect bouncerRect_;  // used for graphics and logic
  SDL_Texture* bouncerTextureTouching_;
  SDL_Texture* bouncerTextureColliding_;
  SDL_Texture* bouncerTextureFree_;

  SFG::Engine::Vector2 moving_;
};

}  // namespace Content
}  // namespace SFG

#endif /* _CONTENT_BOUNCER_H_ */