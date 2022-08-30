#include "graphicshandler.h"

// Include our overall Objects
#include "../globals.h"

namespace SFG {
    GraphicsHandler::GraphicsHandler(SDL_Window* window)
        : drawCallbacks()
        , quitFlag(nullptr)
        , window(window)
        , windowRenderer(nullptr) {
    }

    GraphicsHandler::~GraphicsHandler() {
    }

    void GraphicsHandler::Draw(GraphicsHandler* self) {
        self->windowRenderer = SDL_CreateRenderer(self->window, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetRenderDrawColor(self->windowRenderer, 0, 0, 0, 255);
        while (!(*self->quitFlag)) {
            SDL_RenderClear(self->windowRenderer);

            for (auto callback : self->drawCallbacks) callback(self->windowRenderer);

            SDL_RenderPresent(self->windowRenderer);
            Performance::AddGraphicsLoop();
        }
        SDL_DestroyRenderer(self->windowRenderer);
        self->windowRenderer = nullptr;
    }

    void GraphicsHandler::SetQuitFlag(bool* quitFlag) {
        if (quitFlag == nullptr) return;
        this->quitFlag = quitFlag;
    }

    void GraphicsHandler::StartDraw() {
        graphicsThread = std::thread(Draw, this);
    }

    void GraphicsHandler::StopDraw() {
        graphicsThread.join();
    }

    void GraphicsHandler::RegisterDrawEvent(DrawCallback callback) {
        if (callback == nullptr) return;
        drawCallbacks.push_back(callback);
    }
}
