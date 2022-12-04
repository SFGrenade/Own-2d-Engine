#include "graphicshandler.h"

namespace SFG {
GraphicsHandler::GraphicsHandler(SDL_Window* window)
    : logger(spdlog::get("GraphicsHandler")), drawCallbacks(), quitFlag(nullptr), rendererIndex(-1), window(window), windowRenderer(nullptr) {
    logger->trace("GraphicsHandler(SDL_Window* window = {:p})", static_cast<void*>(window));
    logger->trace("GraphicsHandler()~");
}

GraphicsHandler::~GraphicsHandler() {
    logger->trace("~GraphicsHandler()");
    logger->trace("~GraphicsHandler()~");
}

void GraphicsHandler::Draw(GraphicsHandler* self) {
    self->logger->trace("Draw(GraphicsHandler* self = {:p})", static_cast<void*>(self));
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
    self->logger->trace("Draw()~");
}

void GraphicsHandler::SetQuitFlag(bool* newQuitFlag) {
    logger->trace("SetQuitFlag(bool* quitFlag = {:p})", static_cast<void*>(newQuitFlag));
    if (newQuitFlag) this->quitFlag = newQuitFlag;
    logger->trace("SetQuitFlag()~");
}

void GraphicsHandler::SetRendererIndex(int index) {
    logger->trace("SetRendererIndex(int index = {:d})", index);
    if (index >= 0) this->rendererIndex = index;
    logger->trace("SetRendererIndex()~");
}

void GraphicsHandler::StartDraw() {
    logger->trace("StartDraw()");
    graphicsThread = std::thread(Draw, this);
    logger->trace("StartDraw()~");
}

void GraphicsHandler::StopDraw() {
    logger->trace("StopDraw()");
    graphicsThread.join();
    logger->trace("StopDraw()~");
}

void GraphicsHandler::RegisterDrawEvent(DrawCallback callback) {
    logger->trace("RegisterDrawEvent(DrawCallback callback)");
    if (callback) drawCallbacks.push_back(callback);
    logger->trace("RegisterDrawEvent()~");
}
}  // namespace SFG
