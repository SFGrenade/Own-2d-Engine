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
    Window* Window::instance = nullptr;

    Window::Window(/* args */)
        : xPos(SDL_WINDOWPOS_CENTERED)
        , yPos(SDL_WINDOWPOS_CENTERED)
        , width(25 * gridWidth)
        , height(19 * gridWidth)
        , windowTitle("Own 2D Engine - Exit with ESC")
        , window(nullptr)
        , graphicsHandler(nullptr) {
    }

    Window::~Window() {
        if (graphicsHandler) delete graphicsHandler;
        if (window) SDL_DestroyWindow(window);

        Mix_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    Window* Window::GetInstance() {
        if (!Window::instance) Window::instance = new Window();
        return Window::instance;
    }

    bool Window::InitializeSDL() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            fmt::print(stderr, "SDL initialize error! {}\n", SDL_GetError());
            return false;
        }
        if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
            fmt::print(stderr, "SDL_image initialize error! {}\n", IMG_GetError());
            return false;
        }
        if ((Mix_Init(SDL_MIX_INIT_MP3_OGG) != SDL_MIX_INIT_MP3_OGG) || Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            fmt::print(stderr, "SDL_mixer initialize error! {}\n", Mix_GetError());
            return false;
        }
        return true;
    }

    bool Window::InitializeWindow() {
        window = SDL_CreateWindow(windowTitle.c_str(), xPos, yPos, width, height, SDL_WINDOW_HIDDEN);
        if (!window) {
            fmt::print(stderr, "Window creation error! {}\n", SDL_GetError());
            return false;
        }
        graphicsHandler = new GraphicsHandler(window);
        return true;
    }

    bool Window::ShowWindow() {
        SDL_ShowWindow(window);
        return true;
    }

    SDL_Window* Window::GetSdlWindow() {
        return window;
    }

    GraphicsHandler* Window::GetGraphicsHandler() {
        return graphicsHandler;
    }
}
