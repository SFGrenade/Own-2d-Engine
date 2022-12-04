#include "window.h"

#define SDL_IMG_INIT_EVERYTHING (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP)

#define SDL_MIX_INIT_EVERYTHING (MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS)
#define SDL_MIX_INIT_MP3_OGG (MIX_INIT_MP3 | MIX_INIT_OGG)

namespace SFG {
spdlogger Window::logger = nullptr;
int Window::xPos = SDL_WINDOWPOS_CENTERED;
int Window::yPos = SDL_WINDOWPOS_CENTERED;
int Window::width = 25 * gridWidth;
int Window::height = 19 * gridWidth;
std::string Window::windowTitle = "Own 2D Engine - Exit with ESC";
SDL_Window* Window::window = nullptr;
GraphicsHandler* Window::graphicsHandler = nullptr;

void Window::SetSize(int newWidth, int newHeight) {
    Window::logger->trace("SetSize({:d}, {:d})", newWidth, newHeight);
    Window::width = newWidth;
    Window::height = newHeight;
    Window::logger->trace("Initialize()~");
}

void Window::Initialize() {
    Window::logger = spdlog::get("Window");
    Window::logger->trace("Initialize()");
    Window::logger->trace("Initialize()~");
}

void Window::Destroy() {
    Window::logger->trace("Destroy()");
    if (Window::graphicsHandler) delete Window::graphicsHandler;
    if (Window::window) SDL_DestroyWindow(Window::window);

    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    Window::logger->trace("Destroy()~");
}

bool Window::InitializeSDL() {
    Window::logger->trace("InitializeSDL()");
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Window::logger->error("SDL initialize error! {:s}", SDL_GetError());
        Window::logger->trace("InitializeSDL()~");
        return false;
    }
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        Window::logger->error("SDL_image initialize error! {:s}", IMG_GetError());
        Window::logger->trace("InitializeSDL()~");
        return false;
    }
    if ((Mix_Init(SDL_MIX_INIT_MP3_OGG) != SDL_MIX_INIT_MP3_OGG) || Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        Window::logger->error("SDL_mixer initialize error! {:s}", Mix_GetError());
        Window::logger->trace("InitializeSDL()~");
        return false;
    }
    Window::logger->trace("InitializeSDL()~");
    return true;
}

bool Window::InitializeWindow() {
    Window::logger->trace("InitializeWindow()");
    Window::window = SDL_CreateWindow(Window::windowTitle.c_str(), Window::xPos, Window::yPos, Window::width, Window::height, SDL_WINDOW_HIDDEN);
    if (!Window::window) {
        Window::logger->error("Window creation error! {:s}", SDL_GetError());
        Window::logger->trace("InitializeWindow()~");
        return false;
    }
    Window::graphicsHandler = new GraphicsHandler(Window::window);
    Window::logger->trace("InitializeWindow()~");
    return true;
}

bool Window::ShowWindow() {
    Window::logger->trace("ShowWindow()");
    SDL_ShowWindow(Window::window);
    Window::logger->trace("ShowWindow()~");
    return true;
}

SDL_Window* Window::GetSdlWindow() {
    Window::logger->trace("GetSdlWindow()");
    Window::logger->trace("GetSdlWindow()~");
    return Window::window;
}

GraphicsHandler* Window::GetGraphicsHandler() {
    Window::logger->trace("GetGraphicsHandler()");
    Window::logger->trace("GetGraphicsHandler()~");
    return Window::graphicsHandler;
}
}  // namespace SFG
