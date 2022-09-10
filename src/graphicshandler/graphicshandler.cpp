#include "graphicshandler.h"

// Include our overall Objects
#include "../globals.h"

namespace SFG {
    GraphicsHandler::GraphicsHandler(SDL_Window* window)
        : drawCallbacks()
        , quitFlag(nullptr)
        , window(window)
        , windowRenderer(nullptr) {
        spdlog::trace("GraphicsHandler::GraphicsHandler(SDL_Window* window = {})", (void*) window);
        spdlog::trace("GraphicsHandler::GraphicsHandler()~");
    }

    GraphicsHandler::~GraphicsHandler() {
        spdlog::trace("GraphicsHandler::~GraphicsHandler()");
        spdlog::trace("GraphicsHandler::~GraphicsHandler()~");
    }

    void GraphicsHandler::Draw(GraphicsHandler* self) {
        spdlog::trace("GraphicsHandler::Draw(GraphicsHandler* self = {})", (void*) self);
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
        spdlog::trace("GraphicsHandler::Draw()~");
    }

    void GraphicsHandler::SetQuitFlag(bool* quitFlag) {
        spdlog::trace("GraphicsHandler::SetQuitFlag(bool* quitFlag = {})", (void*) quitFlag);
        if (quitFlag) this->quitFlag = quitFlag;
        spdlog::trace("GraphicsHandler::SetQuitFlag()~");
    }

    void GraphicsHandler::StartDraw() {
        spdlog::trace("GraphicsHandler::StartDraw()");
        graphicsThread = std::thread(Draw, this);
        spdlog::trace("GraphicsHandler::StartDraw()~");
    }

    void GraphicsHandler::StopDraw() {
        spdlog::trace("GraphicsHandler::StopDraw()");
        graphicsThread.join();
        spdlog::trace("GraphicsHandler::StopDraw()~");
    }

    void GraphicsHandler::RegisterDrawEvent(DrawCallback callback) {
        spdlog::trace("GraphicsHandler::RegisterDrawEvent(DrawCallback callback)");
        if (callback) drawCallbacks.push_back(callback);
        spdlog::trace("GraphicsHandler::RegisterDrawEvent()~");
    }
}
