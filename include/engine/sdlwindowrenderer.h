#ifndef _ENGINE_SDLRENDERER_H_
#define _ENGINE_SDLRENDERER_H_

#include <mutex>

#include "_globals/sdlInclude.h"
#include "_globals/spdlogInclude.h"


namespace SFG {
namespace Engine {

class SdlWindow;

struct DebugInfoStruct {
  __declspec( align( 64 ) ) std::string message_;
  __declspec( align( 64 ) ) bool drawNew_;
  __declspec( align( 64 ) ) SDL_Texture* texture_;
  __declspec( align( 64 ) ) SDL_Rect textureRect_;
};

class SdlWindowRenderer {
  public:
  SdlWindowRenderer( SFG::Engine::SdlWindow* sdlWindow );
  ~SdlWindowRenderer();

  void initialize_sdl_renderer( int32_t rendererIndex, SDL_RendererFlags flags );
  void signal_quit();
  void run_loop();

  void set_debugInfo_topLeft( std::string const& debugInfo );
  void set_debugInfo_topRight( std::string const& debugInfo );
  void set_debugInfo_bottomLeft( std::string const& debugInfo );
  void set_debugInfo_bottomRight( std::string const& debugInfo );

  private:
  void renderDebugInfo( DebugInfoStruct& debugInfo );

  private:
  __declspec( align( 64 ) ) spdlogger logger_;

  __declspec( align( 64 ) ) SFG::Engine::SdlWindow* sdlWindow_;
  __declspec( align( 64 ) ) bool done_;
  __declspec( align( 64 ) ) std::mutex doneMutex_;
  __declspec( align( 64 ) ) DebugInfoStruct debugInfoTopLeft_;
  __declspec( align( 64 ) ) DebugInfoStruct debugInfoTopRight_;
  __declspec( align( 64 ) ) DebugInfoStruct debugInfoBottomLeft_;
  __declspec( align( 64 ) ) DebugInfoStruct debugInfoBottomRight_;

  __declspec( align( 64 ) ) TTF_Font* sdlFont_;
  __declspec( align( 64 ) ) SDL_Renderer* sdlRenderer_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_SDLRENDERER_H_ */
