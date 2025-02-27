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
  private:
  struct RendererData {
    SDL_Renderer* renderer;
    std::thread thread;
    std::function< void( SDL_Renderer* ) > callback;
    bool done;
  };

  public:
  static void init();

  static void GetRendererInfos( SDL_Renderer* renderer = nullptr );
  static SDL_Renderer* CreateRenderer( SDL_Window* window,
                                       std::function< void( SDL_Renderer* ) > callback,
                                       SDL_RendererFlags flags = static_cast< SDL_RendererFlags >( SDL_RENDERER_ACCELERATED ),
                                       std::string const& wantedRenderer = "default" );
  static void DestroyRendererForWindow( SDL_Window* window );
  static void Shutdown();

  private:
  static void ThreadRun( RendererData* data );

  private:
  static SFG::Own2dEngine::Logger::spdlogger logger_;
  static std::map< SDL_Window*, RendererData* > renderers_;
};

}  // namespace Engine
}  // namespace Own2dEngine
}  // namespace SFG

#endif /* ENGINE_RENDERERMANAGER_H_ */
