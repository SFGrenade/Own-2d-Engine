#ifndef _ENGINE_SDLRENDERER_H_
#define _ENGINE_SDLRENDERER_H_

#include <mutex>

#include "_globals/sdlInclude.h"
#include "_globals/spdlogInclude.h"


namespace SFG {
namespace Engine {

class SdlWindow;

struct DebugInfoStruct {
  std::string message_;
  bool drawNew_;
  SDL_Texture* texture_;
  SDL_Rect textureRect_;
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
  spdlogger logger_;

  SFG::Engine::SdlWindow* sdlWindow_;
  bool done_;
  std::mutex doneMutex_;
  DebugInfoStruct debugInfoTopLeft_;
  DebugInfoStruct debugInfoTopRight_;
  DebugInfoStruct debugInfoBottomLeft_;
  DebugInfoStruct debugInfoBottomRight_;

  TTF_Font* sdlFont_;
  SDL_Renderer* sdlRenderer_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_SDLRENDERER_H_ */
