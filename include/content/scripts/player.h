#ifndef _CONTENT_PLAYER_H_
#define _CONTENT_PLAYER_H_

#include "_globals/spdlogInclude.h"
#include "engine/script.h"


namespace SFG {
namespace Content {

struct PlayerStruct {
  std::string message_;
  bool drawNew_;
  SDL_Texture* texture_;
  SDL_Rect textureRect_;
};

class Player : public SFG::Engine::Script {
  public:
  Player( SFG::Engine::SdlWindow* sdlWindow );
  virtual ~Player();

  virtual void start();

  virtual void frame_update( SDL_Renderer* renderer );
  virtual void input_update( SDL_Event const& input );
  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime );
  virtual void fixed_update();
  virtual void network_update();  // todo

  virtual void end();

  private:
  spdlogger logger_;

  bool rendering_;
  long double positionX_;
  long double positionY_;
  long double speed_;
  SDL_Rect playerRect_;

  bool inputUp_;
  bool inputDown_;
  bool inputLeft_;
  bool inputRight_;
};

}  // namespace Content
}  // namespace SFG

#endif /* _CONTENT_PLAYER_H_ */
