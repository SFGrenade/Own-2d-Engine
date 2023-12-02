#ifndef _ENGINE_SCRIPT_H_
#define _ENGINE_SCRIPT_H_

#include "_globals/moreChrono.h"
#include "_globals/sdlInclude.h"


namespace SFG {
namespace Engine {

class SdlWindow;
class ScriptManager;

class Script {
  // using _base_ = ;
  // using _base_::_base_;

  public:
  Script();
  virtual ~Script();

  virtual void start();

  virtual void frame_update( SDL_Renderer* renderer );
  virtual void input_update( SDL_Event const& input );
  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime );
  virtual void network_update();

  virtual void end();

  void set_sdlWindow( SFG::Engine::SdlWindow* sdlWindow );
  void set_scriptManager( SFG::Engine::ScriptManager* scriptManager );
  void set_scriptList( std::vector< SFG::Engine::Script* > const* scriptList );

  protected:
  SFG::Engine::SdlWindow* sdlWindow_;
  SFG::Engine::ScriptManager* scriptManager_;
  std::vector< SFG::Engine::Script* > const* scriptList_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_SCRIPT_H_ */
