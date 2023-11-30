#ifndef _CONTENT_PLAYER_H_
#define _CONTENT_PLAYER_H_

#include "_globals/spdlogInclude.h"
#include "engine/scriptentity.h"


namespace SFG {
namespace Content {

struct PlayerStruct {
  std::string message_;
  bool drawNew_;
  SDL_Texture* texture_;
  SDL_Rect textureRect_;
};

class Player : public SFG::Engine::ScriptEntity {
  using _base_ = SFG::Engine::ScriptEntity;
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
  SDL_Rect playerRect_;
};

}  // namespace Content
}  // namespace SFG

#endif /* _CONTENT_PLAYER_H_ */
