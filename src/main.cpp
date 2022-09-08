// Include our overall Objects
#include "globals.h"

#include "fonthandler/fonthandler.h"
#include "inputhandler/inputhandler.h"
#include "logichandler/logichandler.h"
#include "performance/performance.h"
#include "window/window.h"

#include <SDL2/SDL_ttf.h>

// library headers
#include <chrono>
#include <cstdint>
#include <cstring>
#include <functional>
#include <string>
#include <thread>
#include <fmt/core.h>
#include "pugiXML/pugixml.hpp"

using namespace std::chrono_literals;

typedef struct {
    Uint8 r;
    Uint8 g;
    Uint8 b;
} BetterColor;

int main(/*int argc, char* argv[]*/) {
    bool quit = false;

    SFG::FontHandler fontHandler = SFG::FontHandler::GetInstance();
    SFG::InputHandler inputHandler = SFG::InputHandler::GetInstance();
    SFG::LogicHandler* logicHandler = SFG::LogicHandler::GetInstance();
    SFG::Window window = SFG::Window::GetInstance();
    window.InitializeSDL();
    window.InitializeWindow();
    window.ShowWindow();
    SFG::GraphicsHandler* graphicsHandler = window.GetGraphicsHandler();

    std::string performanceString;
    bool makeNewPerformanceTexture = false;
    SDL_Texture* performanceTexture = nullptr;
    SDL_Rect performanceTextureRect;
    performanceTextureRect.x = 5;
    performanceTextureRect.y = 5;
    performanceTextureRect.w = 0;
    performanceTextureRect.h = 0;
    graphicsHandler->RegisterDrawEvent([&performanceString, &makeNewPerformanceTexture, &performanceTexture, &performanceTextureRect, &fontHandler](SDL_Renderer* windowRenderer) {
        if (makeNewPerformanceTexture) {
            SDL_Color white;
            white.r = 255;
            white.g = 255;
            white.b = 255;
            white.a = 255;
            SDL_Color black;
            black.r = 0;
            black.g = 0;
            black.b = 0;
            black.a = 0;
            SDL_Surface* textSurface = TTF_RenderUTF8_Shaded_Wrapped(fontHandler.GetFont(SFG::FontType::Console), performanceString.c_str(), white, black, 0);

            if (performanceTexture != nullptr) {
                SDL_DestroyTexture(performanceTexture);
            }
            performanceTexture = SDL_CreateTextureFromSurface(windowRenderer, textSurface);
            performanceTextureRect.w = textSurface->w;
            performanceTextureRect.h = textSurface->h;
            SDL_FreeSurface(textSurface);
            makeNewPerformanceTexture = false;
        }

        if (performanceTexture != nullptr) {
            SDL_RenderCopy(windowRenderer, performanceTexture, NULL, &performanceTextureRect);
        }
    });
    graphicsHandler->SetQuitFlag(&quit);

    inputHandler.RegisterQuitEvent([&quit]() {
        quit = true;
    });
    inputHandler.RegisterWindowEvent([&quit](SDL_WindowEvent& window) {
        quit |= window.event == SDL_WINDOWEVENT_CLOSE;
    });
    inputHandler.RegisterKeyDownEvent([&quit](SDL_KeyboardEvent& key) {
        quit |= key.keysym.sym == SDLK_ESCAPE;
    });

    logicHandler->AddTimer([]() {
        // 50 hz test timer
        fmt::print("1");
    }, std::chrono::duration_cast<std::chrono::nanoseconds>(1s / 20));
    logicHandler->SetQuitFlag(&quit);

    auto printFunc = [&quit, &performanceString, &makeNewPerformanceTexture]() {
        using namespace std::chrono_literals;
        while (!quit) {
            performanceString = fmt::format(
R"(Performance (per second):
{:>9} Frames
{:>9} Input checks
{:>9} Logic loops)", SFG::Performance::GetGraphicsLoop(), SFG::Performance::GetInputLoop(), SFG::Performance::GetLogicLoop());
            makeNewPerformanceTexture = true;

            std::this_thread::sleep_for(1000ms);
        }
    };
    std::thread printThread(printFunc);

    graphicsHandler->StartDraw();
    logicHandler->StartLogic();

    // inputs have to be checked in the main thread
    while (!quit) {
        inputHandler.CheckInputs();
        SFG::Performance::AddInputLoop();
    }

    logicHandler->StopLogic();
    graphicsHandler->StopDraw();
    printThread.join();

    return EXIT_SUCCESS;
}
