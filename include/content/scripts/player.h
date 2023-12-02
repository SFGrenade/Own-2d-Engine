#ifndef _CONTENT_PLAYER_H_
#define _CONTENT_PLAYER_H_

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

  virtual void start();

  virtual void frame_update( SDL_Renderer* renderer );
  virtual void input_update( SDL_Event const& input );
  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime );
  virtual void network_update();  // todo

  virtual void end();

  private:
  spdlogger logger_;

  bool rendering_;
  bool pressesUp_;
  bool pressesDown_;
  bool pressesLeft_;
  bool pressesRight_;
  SDL_Rect playerRect_;
  SDL_Texture* playerTextureCollision_;
  SDL_Texture* playerTextureNoCollision_;
};

}  // namespace Content
}  // namespace SFG

#endif /* _CONTENT_PLAYER_H_ */
