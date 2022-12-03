#pragma once

// Include general stuff
#include "../globals.h"
#include "../performance/performance.h"

namespace SFG {
typedef std::function<void(SDL_Renderer* windowRenderer)> DrawCallback;

class GraphicsHandler {
   private:
    std::vector<DrawCallback> drawCallbacks;
    bool* quitFlag;
    int rendererIndex;
    SDL_Window* window;
    SDL_Renderer* windowRenderer;
    std::thread graphicsThread;
    static void Draw(GraphicsHandler* self);

   public:
    GraphicsHandler(SDL_Window* window);
    ~GraphicsHandler();

    void SetQuitFlag(bool* quitFlag);
    void SetRendererIndex(int index);
    void StartDraw();
    void StopDraw();
    void RegisterDrawEvent(DrawCallback callback);
};
}  // namespace SFG
