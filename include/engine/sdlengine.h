#ifndef _ENGINE_SDLENGINE_H_
#define _ENGINE_SDLENGINE_H_

#include <map>
#include <mutex>
#include <queue>
#include <set>

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

  SFG::Engine::SdlWindow* add_window( std::string const& title,
                                      uint32_t width,
                                      uint32_t height,
                                      SDL_WindowFlags flags = SDL_WindowFlags::SDL_WINDOW_HIDDEN,
                                      uint32_t x = SDL_WINDOWPOS_CENTERED,
                                      uint32_t y = SDL_WINDOWPOS_CENTERED );
  void destroy_window( uint32_t windowId );
  void run_input_loop();

  private:
  bool run_input_loop( SDL_Event const& e, bool* donePtr );
  bool has_window( uint32_t windowId );
  SFG::Engine::SdlWindow* get_window( uint32_t windowId );

  private:
  spdlogger logger_;
  std::map< uint32_t, SFG::Engine::SdlWindow* > windows_;
  std::mutex windowsMutex_;
  std::queue< uint32_t > windowIdsToDestroy_;
  std::mutex windowIdsToDestroyMutex_;
  std::set< uint32_t > closedWindowIds_;
  std::mutex closedWindowIdsMutex_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_SDLENGINE_H_ */
