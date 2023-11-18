#pragma once

#include <functional>
#include <thread>
#include <vector>

// Including our headers
#include "_globals/sdlInclude.h"
#include "_globals/spdlogInclude.h"

namespace SFG {
typedef std::function< void( std::shared_ptr< SDL_Renderer > windowRenderer ) > DrawCallback;

class GraphicsHandler {
  private:
  void deleteRenderer( SDL_Renderer* ptr );

  public:
  GraphicsHandler( std::shared_ptr< SDL_Window > window );
  ~GraphicsHandler();

  void SetRendererIndex( int index );
  void StartDraw();
  void Draw();
  void RegisterDrawEvent( DrawCallback callback );
  void SetDebugString( std::string debugString );

  private:
  spdlogger logger;
  std::vector< DrawCallback > drawCallbacks;
  int rendererIndex;
  std::shared_ptr< SDL_Window > window;
  std::shared_ptr< SDL_Renderer > windowRenderer;
  SDL_Texture* debugInfo_texture_;
  SDL_Rect debugInfo_textureRect_;
  SDL_Color debugInfo_foregroundColor_;
  SDL_Color debugInfo_backgroundColor_;
};
}  // namespace SFG
