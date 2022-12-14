#include "fonthandler.h"

namespace SFG {
spdlogger FontHandler::logger = nullptr;
std::map<FontType, TTF_Font*> FontHandler::fonts = std::map<FontType, TTF_Font*>();

void FontHandler::Initialize() {
    FontHandler::logger = spdlog::get("FontHandler");
    FontHandler::logger->trace("Initialize()");
    TTF_Init();

    FontHandler::fonts[FontType::Console] = TTF_OpenFont("./Resources/Fonts/NotoSansMono-Regular.ttf", 18);
    FontHandler::fonts[FontType::Ui] = TTF_OpenFont("./Resources/Fonts/NotoSerif-Regular.ttf", 18);
    FontHandler::logger->trace("Initialize()~");
}

void FontHandler::Destroy() {
    FontHandler::logger->trace("Destroy()");
    for (auto pair : FontHandler::fonts) {
        TTF_CloseFont(pair.second);
    }
    FontHandler::fonts.clear();

    TTF_Quit();
    FontHandler::logger->trace("Destroy()~");
}

TTF_Font* FontHandler::GetFont(FontType type) {
    FontHandler::logger->trace("GetFont(FontType type = {:d})", type);
    FontHandler::logger->trace("GetFont()~");
    return FontHandler::fonts[type];
}
}  // namespace SFG
