// Include our overall Objects
#include "globals.h"
#include <spdlog/sinks/basic_file_sink.h>

#include "fonthandler/fonthandler.h"
#include "inputhandler/inputhandler.h"
#include "logichandler/logichandler.h"
#include "performance/performance.h"
#include "window/window.h"
#include "scripthandler/scripthandler.h"
#include "scripthandler/CustomScripts/logscript.h"

#include <SDL2/SDL_ttf.h>

// library headers
#include <cstdint>
#include <cstring>
#include <functional>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include "pugiXML/pugixml.hpp"

std::string GetRendererFlags(uint32_t rendererFlags) {
    std::string ret = "";
    if (rendererFlags & SDL_RENDERER_SOFTWARE) { ret += "SDL_RENDERER_SOFTWARE | "; }
    if (rendererFlags & SDL_RENDERER_ACCELERATED) { ret += "SDL_RENDERER_ACCELERATED | "; }
    if (rendererFlags & SDL_RENDERER_PRESENTVSYNC) { ret += "SDL_RENDERER_PRESENTVSYNC | "; }
    if (rendererFlags & SDL_RENDERER_TARGETTEXTURE) { ret += "SDL_RENDERER_TARGETTEXTURE | "; }
    return ret.substr(0, ret.size() - 3);
}

std::string GetPixelFormatEnum(uint32_t textureFlags) {
    std::string ret = "";
    if (textureFlags & SDL_PIXELFORMAT_UNKNOWN) { ret += "SDL_PIXELFORMAT_UNKNOWN | "; }
    if (textureFlags & SDL_PIXELFORMAT_INDEX1LSB) { ret += "SDL_PIXELFORMAT_INDEX1LSB | "; }
    if (textureFlags & SDL_PIXELFORMAT_INDEX1MSB) { ret += "SDL_PIXELFORMAT_INDEX1MSB | "; }
    if (textureFlags & SDL_PIXELFORMAT_INDEX4LSB) { ret += "SDL_PIXELFORMAT_INDEX4LSB | "; }
    if (textureFlags & SDL_PIXELFORMAT_INDEX4MSB) { ret += "SDL_PIXELFORMAT_INDEX4MSB | "; }
    if (textureFlags & SDL_PIXELFORMAT_INDEX8) { ret += "SDL_PIXELFORMAT_INDEX8 | "; }
    if (textureFlags & SDL_PIXELFORMAT_RGB332) { ret += "SDL_PIXELFORMAT_RGB332 | "; }
    if (textureFlags & SDL_PIXELFORMAT_RGB444) { ret += "SDL_PIXELFORMAT_RGB444 | "; }
    if (textureFlags & SDL_PIXELFORMAT_RGB555) { ret += "SDL_PIXELFORMAT_RGB555 | "; }
    if (textureFlags & SDL_PIXELFORMAT_BGR555) { ret += "SDL_PIXELFORMAT_BGR555 | "; }
    if (textureFlags & SDL_PIXELFORMAT_ARGB4444) { ret += "SDL_PIXELFORMAT_ARGB4444 | "; }
    if (textureFlags & SDL_PIXELFORMAT_RGBA4444) { ret += "SDL_PIXELFORMAT_RGBA4444 | "; }
    if (textureFlags & SDL_PIXELFORMAT_ABGR4444) { ret += "SDL_PIXELFORMAT_ABGR4444 | "; }
    if (textureFlags & SDL_PIXELFORMAT_BGRA4444) { ret += "SDL_PIXELFORMAT_BGRA4444 | "; }
    if (textureFlags & SDL_PIXELFORMAT_ARGB1555) { ret += "SDL_PIXELFORMAT_ARGB1555 | "; }
    if (textureFlags & SDL_PIXELFORMAT_RGBA5551) { ret += "SDL_PIXELFORMAT_RGBA5551 | "; }
    if (textureFlags & SDL_PIXELFORMAT_ABGR1555) { ret += "SDL_PIXELFORMAT_ABGR1555 | "; }
    if (textureFlags & SDL_PIXELFORMAT_BGRA5551) { ret += "SDL_PIXELFORMAT_BGRA5551 | "; }
    if (textureFlags & SDL_PIXELFORMAT_RGB565) { ret += "SDL_PIXELFORMAT_RGB565 | "; }
    if (textureFlags & SDL_PIXELFORMAT_BGR565) { ret += "SDL_PIXELFORMAT_BGR565 | "; }
    if (textureFlags & SDL_PIXELFORMAT_RGB24) { ret += "SDL_PIXELFORMAT_RGB24 | "; }
    if (textureFlags & SDL_PIXELFORMAT_BGR24) { ret += "SDL_PIXELFORMAT_BGR24 | "; }
    if (textureFlags & SDL_PIXELFORMAT_RGB888) { ret += "SDL_PIXELFORMAT_RGB888 | "; }
    if (textureFlags & SDL_PIXELFORMAT_RGBX8888) { ret += "SDL_PIXELFORMAT_RGBX8888 | "; }
    if (textureFlags & SDL_PIXELFORMAT_BGR888) { ret += "SDL_PIXELFORMAT_BGR888 | "; }
    if (textureFlags & SDL_PIXELFORMAT_BGRX8888) { ret += "SDL_PIXELFORMAT_BGRX8888 | "; }
    if (textureFlags & SDL_PIXELFORMAT_ARGB8888) { ret += "SDL_PIXELFORMAT_ARGB8888 | "; }
    if (textureFlags & SDL_PIXELFORMAT_RGBA8888) { ret += "SDL_PIXELFORMAT_RGBA8888 | "; }
    if (textureFlags & SDL_PIXELFORMAT_ABGR8888) { ret += "SDL_PIXELFORMAT_ABGR8888 | "; }
    if (textureFlags & SDL_PIXELFORMAT_BGRA8888) { ret += "SDL_PIXELFORMAT_BGRA8888 | "; }
    if (textureFlags & SDL_PIXELFORMAT_ARGB2101010) { ret += "SDL_PIXELFORMAT_ARGB2101010 | "; }
    if (textureFlags & SDL_PIXELFORMAT_RGBA32) { ret += "SDL_PIXELFORMAT_RGBA32 | "; }
    if (textureFlags & SDL_PIXELFORMAT_ARGB32) { ret += "SDL_PIXELFORMAT_ARGB32 | "; }
    if (textureFlags & SDL_PIXELFORMAT_BGRA32) { ret += "SDL_PIXELFORMAT_BGRA32 | "; }
    if (textureFlags & SDL_PIXELFORMAT_ABGR32) { ret += "SDL_PIXELFORMAT_ABGR32 | "; }
    if (textureFlags & SDL_PIXELFORMAT_YV12) { ret += "SDL_PIXELFORMAT_YV12 | "; }
    if (textureFlags & SDL_PIXELFORMAT_IYUV) { ret += "SDL_PIXELFORMAT_IYUV | "; }
    if (textureFlags & SDL_PIXELFORMAT_YUY2) { ret += "SDL_PIXELFORMAT_YUY2 | "; }
    if (textureFlags & SDL_PIXELFORMAT_UYVY) { ret += "SDL_PIXELFORMAT_UYVY | "; }
    if (textureFlags & SDL_PIXELFORMAT_YVYU) { ret += "SDL_PIXELFORMAT_YVYU | "; }
    if (textureFlags & SDL_PIXELFORMAT_NV12) { ret += "SDL_PIXELFORMAT_NV12 | "; }
    if (textureFlags & SDL_PIXELFORMAT_NV21) { ret += "SDL_PIXELFORMAT_NV21 | "; }
    return ret.substr(0, ret.size() - 3);
}

int main(int argc, char* argv[]) {
    std::vector<std::string> argvVec;
    for (int i = 0; i < argc; i++) {
        argvVec.push_back(std::string(argv[i]));
    }

    std::fstream logFileToEmptyContents;
	logFileToEmptyContents.open("log.log", std::ios::out);
    logFileToEmptyContents.close();
    auto logfile = spdlog::basic_logger_mt("main", "log.log");
    spdlog::set_default_logger(logfile);
    spdlog::set_level(spdlog::level::trace);

    spdlog::trace("main(int argc = {}, char* argv[] = {} {} {})", argc, "{", fmt::join(argvVec, ", "), "}");

    bool quit = false;
    bool* quitPtr = &quit;

    SFG::FontHandler::Initialize();
    SFG::InputHandler::Initialize();
    SFG::LogicHandler::Initialize();
    SFG::Window::Initialize();
    SFG::Window::InitializeSDL();
    SFG::Window::InitializeWindow();
    SFG::Window::ShowWindow();
    SFG::GraphicsHandler* graphicsHandler = SFG::Window::GetGraphicsHandler();
    SFG::ScriptHandler::Initialize();

    int numRDevices = SDL_GetNumRenderDrivers();
    spdlog::debug("SDL_GetNumRenderDrivers() => {}", numRDevices);
    for (int i = 0; i < numRDevices; i++) {
        SDL_Renderer* renderer = SDL_CreateRenderer(SFG::Window::GetSdlWindow(), i, 0);
        SDL_RendererInfo info;
        SDL_GetRendererInfo(renderer, &info);
        if (argvVec.size() > 1 && argvVec[1] == info.name) {
            graphicsHandler->SetRendererIndex(i);
        }
        spdlog::debug("Renderer {}:", i);
        spdlog::debug(" - Name: \"{}\"", info.name);
        spdlog::debug(" - Flags: {}", GetRendererFlags(info.flags));
        spdlog::debug(" - NumTextureFormats: {}", info.num_texture_formats);
        spdlog::debug(" - TextureFormats:");
        for (int j = 0; j < info.num_texture_formats; j++) {
            spdlog::debug("    - TextureFormat {}: {} / {}", j, info.texture_formats[j], GetPixelFormatEnum(info.texture_formats[j]));
        }
        spdlog::debug(" - MaxTextureWidth: {}", info.max_texture_width);
        spdlog::debug(" - MaxTextureHeight: {}", info.max_texture_height);
        SDL_DestroyRenderer(renderer);
    }

    std::string performanceString;
    bool makeNewPerformanceTexture = false;
    SDL_Texture* performanceTexture = nullptr;
    SDL_Rect performanceTextureRect;
    performanceTextureRect.x = 5;
    performanceTextureRect.y = 5;
    performanceTextureRect.w = 0;
    performanceTextureRect.h = 0;
    graphicsHandler->RegisterDrawEvent([](SDL_Renderer* /*windowRenderer*/) {
        SFG::ScriptHandler::UpdateScriptsFrame();
    });
    graphicsHandler->RegisterDrawEvent([&performanceString, &makeNewPerformanceTexture, &performanceTexture, &performanceTextureRect](SDL_Renderer* windowRenderer) {
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
            SDL_Surface* textSurface = TTF_RenderUTF8_Shaded_Wrapped(SFG::FontHandler::GetFont(SFG::FontType::Console), performanceString.c_str(), white, black, 0);

            if (performanceTexture) {
                SDL_DestroyTexture(performanceTexture);
            }
            performanceTexture = SDL_CreateTextureFromSurface(windowRenderer, textSurface);
            performanceTextureRect.w = textSurface->w;
            performanceTextureRect.h = textSurface->h;
            SDL_FreeSurface(textSurface);
            makeNewPerformanceTexture = false;
        }

        if (performanceTexture) {
            SDL_RenderCopy(windowRenderer, performanceTexture, NULL, &performanceTextureRect);
        }
    });
    graphicsHandler->SetQuitFlag(quitPtr);

    SFG::InputHandler::RegisterQuitEvent([&quit]() {
        quit = true;
    });
    SFG::InputHandler::RegisterWindowEvent([&quit](SDL_WindowEvent& window) {
        quit |= window.event == SDL_WINDOWEVENT_CLOSE;
    });
    SFG::InputHandler::RegisterKeyDownEvent([&quit](SDL_KeyboardEvent& key) {
        quit |= key.keysym.sym == SDLK_ESCAPE;
    });

    SFG::LogicHandler::AddTimer([](std::optional<std::chrono::secondsLongDouble> /*interval*/) {
        // 50 hz test timer
        SFG::ScriptHandler::UpdateScriptsLogicFrame();
        //fmt::print("Interval: {:.9f} seconds\n", interval.value().count());
    }, std::chrono::duration_cast<std::chrono::nanoseconds>(1.0s / 50.0), false);
    SFG::LogicHandler::AddTimer([&performanceString, &makeNewPerformanceTexture](std::optional<std::chrono::secondsLongDouble> /*interval*/) {
        performanceString = fmt::format(
R"(Performance (per second):
{:>9} Frames drawn
{:>9} Input checks
{:>9} Logic loops)", SFG::Performance::GetGraphicsLoop(), SFG::Performance::GetInputLoop(), SFG::Performance::GetLogicLoop());
        makeNewPerformanceTexture = true;
    }, std::chrono::duration_cast<std::chrono::nanoseconds>(1.0s), false);
    SFG::LogicHandler::SetQuitFlag(quitPtr);

    SFG::LogScript* testScript = SFG::ScriptHandler::AddScript<SFG::LogScript>();

    graphicsHandler->StartDraw();
    SFG::LogicHandler::StartLogic();

    // inputs have to be checked in the main thread
    while (!quit) {
        SFG::InputHandler::CheckInputs();
        SFG::Performance::AddInputLoop();
    }

    SFG::LogicHandler::StopLogic();
    graphicsHandler->StopDraw();
    if (performanceTexture) {
        SDL_DestroyTexture(performanceTexture);
    }

    SFG::ScriptHandler::Destroy();
    SFG::Window::Destroy();
    SFG::LogicHandler::Destroy();
    SFG::InputHandler::Destroy();
    SFG::FontHandler::Destroy();

    spdlog::trace("main()~");
    return EXIT_SUCCESS;
}

