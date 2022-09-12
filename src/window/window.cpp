#include "window.h"

// Include our overall Objects
#include "../globals.h"

// Include more SDL headers
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#define SDL_IMG_INIT_EVERYTHING (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP)

#define SDL_MIX_INIT_EVERYTHING (MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS)
#define SDL_MIX_INIT_MP3_OGG (MIX_INIT_MP3 | MIX_INIT_OGG)

namespace SFG {
    int Window::xPos = SDL_WINDOWPOS_CENTERED;
    int Window::yPos = SDL_WINDOWPOS_CENTERED;
    int Window::width = 25 * gridWidth;
    int Window::height = 19 * gridWidth;
    std::string Window::windowTitle = "Own 2D Engine - Exit with ESC";
    SDL_Window* Window::window = nullptr;
    GraphicsHandler* Window::graphicsHandler = nullptr;

    void Window::Initialize() {
        spdlog::trace("Window::Initialize()");
        spdlog::trace("Window::Initialize()~");
    }

    void Window::Destroy() {
        spdlog::trace("Window::Destroy()");
        if (Window::graphicsHandler) delete Window::graphicsHandler;
        if (Window::window) SDL_DestroyWindow(Window::window);

        Mix_Quit();
        IMG_Quit();
        SDL_Quit();
        spdlog::trace("Window::Destroy()~");
    }

    bool Window::InitializeSDL() {
        spdlog::trace("Window::InitializeSDL()");
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            fmt::print(stderr, "SDL initialize error! {}\n", SDL_GetError());
            spdlog::trace("Window::InitializeSDL()~");
            return false;
        }
        if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
            fmt::print(stderr, "SDL_image initialize error! {}\n", IMG_GetError());
            spdlog::trace("Window::InitializeSDL()~");
            return false;
        }
        if ((Mix_Init(SDL_MIX_INIT_MP3_OGG) != SDL_MIX_INIT_MP3_OGG) || Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            fmt::print(stderr, "SDL_mixer initialize error! {}\n", Mix_GetError());
            spdlog::trace("Window::InitializeSDL()~");
            return false;
        }
        spdlog::trace("Window::InitializeSDL()~");
        return true;
    }

    bool Window::InitializeWindow() {
        spdlog::trace("Window::InitializeWindow()");
        Window::window = SDL_CreateWindow(Window::windowTitle.c_str(), Window::xPos, Window::yPos, Window::width, Window::height, SDL_WINDOW_HIDDEN);
        if (!Window::window) {
            fmt::print(stderr, "Window creation error! {}\n", SDL_GetError());
            spdlog::trace("Window::InitializeWindow()~");
            return false;
        }
        Window::graphicsHandler = new GraphicsHandler(Window::window);
        spdlog::trace("Window::InitializeWindow()~");
        return true;
    }

    bool Window::ShowWindow() {
        spdlog::trace("Window::ShowWindow()");
        SDL_ShowWindow(Window::window);
        spdlog::trace("Window::ShowWindow()~");
        return true;
    }

    SDL_Window* Window::GetSdlWindow() {
        spdlog::trace("Window::GetSdlWindow()");
        spdlog::trace("Window::GetSdlWindow()~");
        return Window::window;
    }

    GraphicsHandler* Window::GetGraphicsHandler() {
        spdlog::trace("Window::GetGraphicsHandler()");
        spdlog::trace("Window::GetGraphicsHandler()~");
        return Window::graphicsHandler;
    }
}

