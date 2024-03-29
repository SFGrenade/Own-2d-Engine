#ifndef _CONTENT_PLAYER_H_
#define _CONTENT_PLAYER_H_

#include "_globals/align.h"
#include "_globals/sdlInclude.h"
#include "_globals/spdlogInclude.h"
#include "engine/scriptcollider.h"


namespace SFG {
namespace Content {

class Player : public SFG::Engine::ScriptCollider {
  using _base_ = SFG::Engine::ScriptCollider;
  using _base_::_base_;

  public:
  Player();
  virtual ~Player();

  virtual void frame_update( SDL_Renderer* renderer ) override;
  virtual void input_update( SDL_Event const& input ) override;
  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime ) override;

  private:
  spdlogger logger_;

  bool rendering_;
  SFG_ENGINE_ALIGN( 64 ) bool pressesUp_;       // used for input and logic
  SFG_ENGINE_ALIGN( 64 ) bool pressesDown_;     // used for input and logic
  SFG_ENGINE_ALIGN( 64 ) bool pressesLeft_;     // used for input and logic
  SFG_ENGINE_ALIGN( 64 ) bool pressesRight_;    // used for input and logic
  SFG_ENGINE_ALIGN( 64 ) SDL_Rect playerRect_;  // used for graphics and logic
  SDL_Texture* playerTexture_;
};

}  // namespace Content
}  // namespace SFG

#endif /* _CONTENT_PLAYER_H_ */
