#ifndef _CONTENT_LOGSCRIPT_H_
#define _CONTENT_LOGSCRIPT_H_

#include "_globals/spdlogInclude.h"
#include "engine/script.h"


namespace SFG {
namespace Content {

class LogScript : public SFG::Engine::Script {
  public:
  LogScript( SFG::Engine::SdlWindow* sdlWindow );
  virtual ~LogScript();

  virtual void start();

  virtual void frame_update( SDL_Renderer* renderer );
  virtual void input_update( SDL_Event const& input );
  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime );
  virtual void fixed_update();
  virtual void network_update();  // todo

  virtual void end();

  private:
  spdlogger logger_;
};

}  // namespace Content
}  // namespace SFG

#endif /* _CONTENT_LOGSCRIPT_H_ */
