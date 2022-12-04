#include "graphicshandler.h"

// Include our overall Objects
#include "../globals.h"

namespace SFG {
GraphicsHandler::GraphicsHandler(SDL_Window* window)
    : logger(spdlog::get("GraphicsHandler")), drawCallbacks(), quitFlag(nullptr), rendererIndex(-1), window(window), windowRenderer(nullptr) {
    logger->trace("GraphicsHandler::GraphicsHandler(SDL_Window* window = {})", (void*)window);
    logger->trace("GraphicsHandler::GraphicsHandler()~");
}

GraphicsHandler::~GraphicsHandler() {
    logger->trace("GraphicsHandler::~GraphicsHandler()");
    logger->trace("GraphicsHandler::~GraphicsHandler()~");
}

void GraphicsHandler::Draw(GraphicsHandler* self) {
    self->logger->trace("GraphicsHandler::Draw(GraphicsHandler* self = {})", (void*)self);
    self->windowRenderer = SDL_CreateRenderer(self->window, self->rendererIndex, 0);
    SDL_SetRenderDrawColor(self->windowRenderer, 0, 0, 0, 255);
    while (!(*self->quitFlag)) {
        SDL_RenderClear(self->windowRenderer);

        for (auto callback : self->drawCallbacks) callback(self->windowRenderer);

        SDL_RenderPresent(self->windowRenderer);
        Performance::AddGraphicsLoop();
    }
    SDL_DestroyRenderer(self->windowRenderer);
    self->windowRenderer = nullptr;
    self->logger->trace("GraphicsHandler::Draw()~");
}

void GraphicsHandler::SetQuitFlag(bool* newQuitFlag) {
    logger->trace("GraphicsHandler::SetQuitFlag(bool* quitFlag = {})", (void*)newQuitFlag);
    if (newQuitFlag) this->quitFlag = newQuitFlag;
    logger->trace("GraphicsHandler::SetQuitFlag()~");
}

void GraphicsHandler::SetRendererIndex(int index) {
    logger->trace("GraphicsHandler::SetRendererIndex(int index = {})", index);
    if (index >= 0) this->rendererIndex = index;
    logger->trace("GraphicsHandler::SetRendererIndex()~");
}

void GraphicsHandler::StartDraw() {
    logger->trace("GraphicsHandler::StartDraw()");
    graphicsThread = std::thread(Draw, this);
    logger->trace("GraphicsHandler::StartDraw()~");
}

void GraphicsHandler::StopDraw() {
    logger->trace("GraphicsHandler::StopDraw()");
    graphicsThread.join();
    logger->trace("GraphicsHandler::StopDraw()~");
}

void GraphicsHandler::RegisterDrawEvent(DrawCallback callback) {
    logger->trace("GraphicsHandler::RegisterDrawEvent(DrawCallback callback)");
    if (callback) drawCallbacks.push_back(callback);
    logger->trace("GraphicsHandler::RegisterDrawEvent()~");
}
}  // namespace SFG
