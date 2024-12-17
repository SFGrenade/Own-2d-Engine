#ifndef ENGINE_RENDERERMANAGER_H_
#define ENGINE_RENDERERMANAGER_H_

#include <SDL2/SDL.h>
#include <SFG/Own2dEngine/Logger/_include.h>
#include <functional>
#include <map>
#include <string>

namespace SFG {
namespace Own2dEngine {
namespace Engine {

class RendererManager {
  public:
  static void init();

  static void GetRendererInfos( SDL_Renderer* renderer = nullptr );
  static SDL_Renderer* CreateRenderer( SDL_Window* window,
                                       SDL_RendererFlags flags = static_cast< SDL_RendererFlags >( SDL_RENDERER_ACCELERATED ),
                                       std::string const& wantedRenderer = "default" );
  static void DoRender( SDL_Renderer* renderer, std::function< void( SDL_Renderer* ) > callback = nullptr );
  static void DestroyRendererForWindow( SDL_Window* window );
  static void Shutdown();

  private:
  static SFG::Own2dEngine::Logger::spdlogger logger_;
  static std::map< SDL_Window*, SDL_Renderer* > renderers_;
};

}  // namespace Engine
}  // namespace Own2dEngine
}  // namespace SFG

#endif /* ENGINE_RENDERERMANAGER_H_ */
