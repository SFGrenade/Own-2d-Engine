// Include our overall Objects

#include "fonthandler/fonthandler.h"
#include "globals.h"
#include "inputhandler/inputhandler.h"
#include "logichandler/logichandler.h"
#include "performance/performance.h"
#include "scripthandler/CustomScripts/logscript.h"
#include "scripthandler/scripthandler.h"
#include "window/window.h"

#pragma region Debug Output

constexpr std::string GetRendererFlags(uint32_t rendererFlags) {
    std::string ret = "";
    if (rendererFlags & SDL_RENDERER_SOFTWARE) {
        ret += "SDL_RENDERER_SOFTWARE | ";
    }
    if (rendererFlags & SDL_RENDERER_ACCELERATED) {
        ret += "SDL_RENDERER_ACCELERATED | ";
    }
    if (rendererFlags & SDL_RENDERER_PRESENTVSYNC) {
        ret += "SDL_RENDERER_PRESENTVSYNC | ";
    }
    if (rendererFlags & SDL_RENDERER_TARGETTEXTURE) {
        ret += "SDL_RENDERER_TARGETTEXTURE | ";
    }
    return ret.substr(0, ret.size() - 3);
}

constexpr std::string GetPixelFormatEnum(uint32_t textureFlags) {
    std::string ret = "";
    if (textureFlags == SDL_PIXELFORMAT_UNKNOWN) {
        ret += "SDL_PIXELFORMAT_UNKNOWN | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_INDEX1LSB) {
        ret += "SDL_PIXELFORMAT_INDEX1LSB | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_INDEX1MSB) {
        ret += "SDL_PIXELFORMAT_INDEX1MSB | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_INDEX4LSB) {
        ret += "SDL_PIXELFORMAT_INDEX4LSB | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_INDEX4MSB) {
        ret += "SDL_PIXELFORMAT_INDEX4MSB | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_INDEX8) {
        ret += "SDL_PIXELFORMAT_INDEX8 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_RGB332) {
        ret += "SDL_PIXELFORMAT_RGB332 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_XRGB4444) {
        ret += "SDL_PIXELFORMAT_XRGB4444 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_RGB444) {
        ret += "SDL_PIXELFORMAT_RGB444 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_XBGR4444) {
        ret += "SDL_PIXELFORMAT_XBGR4444 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_BGR444) {
        ret += "SDL_PIXELFORMAT_BGR444 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_XRGB1555) {
        ret += "SDL_PIXELFORMAT_XRGB1555 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_RGB555) {
        ret += "SDL_PIXELFORMAT_RGB555 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_XBGR1555) {
        ret += "SDL_PIXELFORMAT_XBGR1555 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_BGR555) {
        ret += "SDL_PIXELFORMAT_BGR555 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_ARGB4444) {
        ret += "SDL_PIXELFORMAT_ARGB4444 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_RGBA4444) {
        ret += "SDL_PIXELFORMAT_RGBA4444 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_ABGR4444) {
        ret += "SDL_PIXELFORMAT_ABGR4444 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_BGRA4444) {
        ret += "SDL_PIXELFORMAT_BGRA4444 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_ARGB1555) {
        ret += "SDL_PIXELFORMAT_ARGB1555 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_RGBA5551) {
        ret += "SDL_PIXELFORMAT_RGBA5551 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_ABGR1555) {
        ret += "SDL_PIXELFORMAT_ABGR1555 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_BGRA5551) {
        ret += "SDL_PIXELFORMAT_BGRA5551 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_RGB565) {
        ret += "SDL_PIXELFORMAT_RGB565 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_BGR565) {
        ret += "SDL_PIXELFORMAT_BGR565 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_RGB24) {
        ret += "SDL_PIXELFORMAT_RGB24 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_BGR24) {
        ret += "SDL_PIXELFORMAT_BGR24 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_XRGB8888) {
        ret += "SDL_PIXELFORMAT_XRGB8888 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_RGB888) {
        ret += "SDL_PIXELFORMAT_RGB888 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_RGBX8888) {
        ret += "SDL_PIXELFORMAT_RGBX8888 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_XBGR8888) {
        ret += "SDL_PIXELFORMAT_XBGR8888 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_BGR888) {
        ret += "SDL_PIXELFORMAT_BGR888 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_BGRX8888) {
        ret += "SDL_PIXELFORMAT_BGRX8888 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_ARGB8888) {
        ret += "SDL_PIXELFORMAT_ARGB8888 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_RGBA8888) {
        ret += "SDL_PIXELFORMAT_RGBA8888 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_ABGR8888) {
        ret += "SDL_PIXELFORMAT_ABGR8888 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_BGRA8888) {
        ret += "SDL_PIXELFORMAT_BGRA8888 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_ARGB2101010) {
        ret += "SDL_PIXELFORMAT_ARGB2101010 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_RGBA32) {
        ret += "SDL_PIXELFORMAT_RGBA32 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_ARGB32) {
        ret += "SDL_PIXELFORMAT_ARGB32 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_BGRA32) {
        ret += "SDL_PIXELFORMAT_BGRA32 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_ABGR32) {
        ret += "SDL_PIXELFORMAT_ABGR32 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_YV12) {
        ret += "SDL_PIXELFORMAT_YV12 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_IYUV) {
        ret += "SDL_PIXELFORMAT_IYUV | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_YUY2) {
        ret += "SDL_PIXELFORMAT_YUY2 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_UYVY) {
        ret += "SDL_PIXELFORMAT_UYVY | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_YVYU) {
        ret += "SDL_PIXELFORMAT_YVYU | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_NV12) {
        ret += "SDL_PIXELFORMAT_NV12 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_NV21) {
        ret += "SDL_PIXELFORMAT_NV21 | ";
    }
    if (textureFlags == SDL_PIXELFORMAT_EXTERNAL_OES) {
        ret += "SDL_PIXELFORMAT_EXTERNAL_OES | ";
    }
    return ret.substr(0, ret.size() - 3);
}

void PrintRendererInfo(const SDL_RendererInfo &rendererInfo) {
    spdlog::debug("- Renderer \"{:s}\":", rendererInfo.name);
    std::string tmpString1 = GetRendererFlags(rendererInfo.flags);
    spdlog::debug("  - Flags: {:#034b} / {:s}", rendererInfo.flags, tmpString1);
    spdlog::debug("  - NumTextureFormats: {:d}", rendererInfo.num_texture_formats);
    spdlog::debug("  - TextureFormats:");
    for (uint32_t j = 0; j < rendererInfo.num_texture_formats; j++) {
        std::string tmpString2 = GetPixelFormatEnum(rendererInfo.texture_formats[j]);
        spdlog::debug("    - TextureFormat {:d}: {:#034b} / {:s}", j, rendererInfo.texture_formats[j], tmpString2);
    }
    spdlog::debug("  - MaxTextureWidth: {:d}", rendererInfo.max_texture_width);
    spdlog::debug("  - MaxTextureHeight: {:d}", rendererInfo.max_texture_height);
}

void CheckAndSelectRenderer(SFG::GraphicsHandler *graphicsHandler, const std::string &cliSelection) {
    int numRDevices = SDL_GetNumRenderDrivers();
    spdlog::debug("SDL_GetNumRenderDrivers() => {:d}", numRDevices);
    for (int i = 0; i < numRDevices; i++) {
        SDL_Renderer *renderer = SDL_CreateRenderer(SFG::Window::GetSdlWindow(), i, 0);
        SDL_RendererInfo info;
        SDL_GetRendererInfo(renderer, &info);
        SDL_DestroyRenderer(renderer);
        PrintRendererInfo(info);
        if (cliSelection == info.name) {
            graphicsHandler->SetRendererIndex(i);
        }
    }
}

#pragma endregion

#pragma region Initialization

void InitializeLoggers() {
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_level(spdlog::level::warn);

    auto truncatedFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.log", true);
    truncatedFileSink->set_level(spdlog::level::trace);
    spdlog::sinks_init_list truncatedSinkList = {truncatedFileSink, consoleSink};

    spdlogger mainLogger = std::make_shared<spdlog::logger>("main", truncatedSinkList.begin(), truncatedSinkList.end());
    mainLogger->set_level(spdlog::level::trace);
    mainLogger->flush_on(spdlog::level::trace);
    spdlog::register_logger(mainLogger);
    spdlog::set_default_logger(mainLogger);

    auto normalFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.log", false);
    normalFileSink->set_level(spdlog::level::trace);
    spdlog::sinks_init_list normalSinkList = {normalFileSink, consoleSink};

    std::vector<std::string> allLoggerNames = {"FontHandler", "GraphicsHandler", "InputHandler", "LogicHandler",
                                               "Timer",       "ScriptHandler",   "LogScript",    "Window"};
    for (auto name : allLoggerNames) {
        spdlogger logger = std::make_shared<spdlog::logger>(name, normalSinkList.begin(), normalSinkList.end());
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);
        spdlog::register_logger(logger);
    }
    spdlog::get("LogScript")->set_level(spdlog::level::warn);
}

void InitializeComponents() {
    SFG::FontHandler::Initialize();
    SFG::InputHandler::Initialize();
    SFG::LogicHandler::Initialize();
    SFG::Window::Initialize();
    SFG::ScriptHandler::Initialize();
}

void InitializeWindow() {
    SFG::Window::InitializeSDL();
    SFG::Window::InitializeWindow();
}

void UninitializeComponents() {
    SFG::ScriptHandler::Destroy();
    SFG::Window::Destroy();
    SFG::LogicHandler::Destroy();
    SFG::InputHandler::Destroy();
    SFG::FontHandler::Destroy();
}

#pragma endregion

int main(int argc, char *argv[]) {
    std::vector<std::string> argvVec;
    for (int i = 0; i < argc; i++) {
        argvVec.push_back(std::string(argv[i]));
    }

    InitializeLoggers();

    spdlog::trace("main(int argc = {:d}, char* argv[] = {:c} {:s} {:c})", argc, '{', fmt::join(argvVec, ", "), '}');

    bool quit = false;
    bool *quitPtr = &quit;

    InitializeComponents();
    SFG::Window::SetSize(1600, 900);
    InitializeWindow();
    SFG::Window::ShowWindow();
    SFG::GraphicsHandler *graphicsHandler = SFG::Window::GetGraphicsHandler();

    if (argvVec.size() > 1) {
        CheckAndSelectRenderer(graphicsHandler, argvVec[1]);
    } else {
        CheckAndSelectRenderer(graphicsHandler, "");
    }

    std::string performanceString;
    bool makeNewPerformanceTexture = false;
    SDL_Texture *performanceTexture = nullptr;
    SDL_Rect performanceTextureRect;
    performanceTextureRect.x = 5;
    performanceTextureRect.y = 5;
    performanceTextureRect.w = 0;
    performanceTextureRect.h = 0;
    graphicsHandler->RegisterDrawEvent([](SDL_Renderer * /*windowRenderer*/) { SFG::ScriptHandler::UpdateScriptsFrame(); });
    graphicsHandler->RegisterDrawEvent(
        [&performanceString, &makeNewPerformanceTexture, &performanceTexture, &performanceTextureRect](SDL_Renderer *windowRenderer) {
            if (makeNewPerformanceTexture) {
                SDL_Color foreground;
                foreground.r = 255;
                foreground.g = 255;
                foreground.b = 255;
                foreground.a = 255;
                SDL_Color background;
                background.r = 0;
                background.g = 0;
                background.b = 0;
                background.a = 0;
                SDL_Surface *textSurface =
                    TTF_RenderUTF8_Shaded_Wrapped(SFG::FontHandler::GetFont(SFG::FontType::Console), performanceString.c_str(), foreground, background, 0);
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

    SFG::InputHandler::RegisterQuitEvent([&quit]() { quit = true; });
    SFG::InputHandler::RegisterWindowEvent([&quit](SDL_WindowEvent &window) { quit |= window.event == SDL_WINDOWEVENT_CLOSE; });
    SFG::InputHandler::RegisterKeyDownEvent([&quit](SDL_KeyboardEvent &key) { quit |= key.keysym.sym == SDLK_ESCAPE; });

    SFG::LogicHandler::AddTimer(
        [](std::optional<std::chrono::secondsLongDouble>
           /*interval*/) {
            // 50 hz test timer
            SFG::ScriptHandler::UpdateScriptsLogicFrame();
        },
        std::chrono::duration_cast<std::chrono::nanoseconds>(1.0s / 50.0), false);
    SFG::LogicHandler::AddTimer(
        [&performanceString, &makeNewPerformanceTexture](std::optional<std::chrono::secondsLongDouble> /*interval*/) {
            performanceString = fmt::format(
                R"(Performance (per second):
{:>20d} Frames drawn
{:>20d} Input checks
{:>20d} Logic loops)",
                SFG::Performance::GetGraphicsLoop(), SFG::Performance::GetInputLoop(), SFG::Performance::GetLogicLoop());
            spdlog::debug(performanceString);
            makeNewPerformanceTexture = true;
        },
        std::chrono::duration_cast<std::chrono::nanoseconds>(1.0s), false);
    SFG::LogicHandler::SetQuitFlag(quitPtr);

    SFG::ScriptHandler::AddScript<SFG::LogScript>();

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

    UninitializeComponents();

    spdlog::trace("main()~");
    return EXIT_SUCCESS;
}
