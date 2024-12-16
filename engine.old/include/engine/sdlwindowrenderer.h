#ifndef _ENGINE_SDLRENDERER_H_
#define _ENGINE_SDLRENDERER_H_

#include <mutex>

#include "_globals/sdlInclude.h"
#include "_globals/spdlogInclude.h"


namespace SFG {
namespace Engine {

class SdlWindow;

class SdlWindowRenderer {
  public:
  SdlWindowRenderer( SFG::Engine::SdlWindow* sdlWindow );
  ~SdlWindowRenderer();

  void initialize_sdl_renderer( int32_t rendererIndex, SDL_RendererFlags flags );
  void signal_quit();
  void run_loop();

  private:
  spdlogger logger_;

  SFG::Engine::SdlWindow* sdlWindow_;
  bool done_;
  std::mutex doneMutex_;

  SDL_Renderer* sdlRenderer_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_SDLRENDERER_H_ */
