#ifndef ENGINE_WINDOWMANAGER_H_
#define ENGINE_WINDOWMANAGER_H_

#include <SDL2/SDL.h>
#include <SFG/Own2dEngine/Logger/_include.h>
#include <map>
#include <string>

namespace SFG {
namespace Own2dEngine {
namespace Engine {

class WindowManager {
  public:
  static SDL_Window* AddWindow( std::string const& title, int x, int y, int w, int h, SDL_WindowFlags flags = static_cast< SDL_WindowFlags >( 0 ) );
  static void RunEvent( SDL_WindowEvent const& event );
  static void Shutdown();

  private:
  static SFG::Own2dEngine::Logger::spdlogger logger_;
  static std::map< uint32_t, SDL_Window* > windows_;
};

}  // namespace Engine
}  // namespace Own2dEngine
}  // namespace SFG

#endif /* ENGINE_WINDOWMANAGER_H_ */
