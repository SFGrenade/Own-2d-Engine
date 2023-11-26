#ifndef _ENGINE_SDLENGINE_H_
#define _ENGINE_SDLENGINE_H_

#include <mutex>
#include <queue>
#include <set>
#include <vector>

#include "_globals/sdlInclude.h"
#include "_globals/spdlogInclude.h"


namespace SFG {
namespace Engine {

class SdlWindow;

class SdlEngine {
  public:
  SdlEngine( uint32_t sdlInitFlags = SDL_INIT_EVERYTHING,
             IMG_InitFlags imgInitFlags = IMG_InitFlags::IMG_INIT_PNG,
             MIX_InitFlags mixInitFlags = MIX_InitFlags::MIX_INIT_OGG );
  ~SdlEngine();

  std::vector< std::string > get_renderer_names() const;

  SFG::Engine::SdlWindow* add_window( std::string const& title,
                                      uint32_t width,
                                      uint32_t height,
                                      SDL_WindowFlags flags = SDL_WindowFlags::SDL_WINDOW_HIDDEN,
                                      uint32_t x = SDL_WINDOWPOS_CENTERED,
                                      uint32_t y = SDL_WINDOWPOS_CENTERED,
                                      bool initialize = true );
  void destroy_window( uint32_t windowId );
  void run_input_loop();

  private:
  bool run_input_loop( SDL_Event const& e );
  bool has_window( uint32_t windowId );
  SFG::Engine::SdlWindow* get_window( uint32_t windowId );

  private:
  __declspec( align( 64 ) ) spdlogger logger_;

  __declspec( align( 64 ) ) std::vector< SFG::Engine::SdlWindow* > windows_;
  __declspec( align( 64 ) ) std::mutex windowsMutex_;
  __declspec( align( 64 ) ) std::queue< uint32_t > windowIdsToDestroy_;
  __declspec( align( 64 ) ) std::mutex windowIdsToDestroyMutex_;
  __declspec( align( 64 ) ) std::set< uint32_t > closedWindowIds_;
  __declspec( align( 64 ) ) std::mutex closedWindowIdsMutex_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_SDLENGINE_H_ */
