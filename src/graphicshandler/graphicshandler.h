#pragma once

// Include general stuff
#include "../globals.h"
#include "../performance/performance.h"

namespace SFG {
typedef std::function< void( std::shared_ptr< SDL_Renderer > windowRenderer ) > DrawCallback;

class GraphicsHandler {
  private:
  spdlogger logger;
  std::vector< DrawCallback > drawCallbacks;
  std::shared_ptr< bool > quitFlag;
  int rendererIndex;
  std::shared_ptr< SDL_Window > window;
  std::shared_ptr< SDL_Renderer > windowRenderer;
  std::thread graphicsThread;

  void deleteRenderer( SDL_Renderer* ptr );
  static void Draw( GraphicsHandler* self );

  public:
  GraphicsHandler( std::shared_ptr< SDL_Window > window );
  ~GraphicsHandler();

  void SetQuitFlag( std::shared_ptr< bool > quitFlag );
  void SetRendererIndex( int index );
  void StartDraw();
  void StopDraw();
  void RegisterDrawEvent( DrawCallback callback );
};
}  // namespace SFG
