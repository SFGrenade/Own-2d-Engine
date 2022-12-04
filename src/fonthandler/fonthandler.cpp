#include "fonthandler.h"

namespace SFG {
spdlogger FontHandler::logger = nullptr;
std::map<FontType, TTF_Font*> FontHandler::fonts = std::map<FontType, TTF_Font*>();

void FontHandler::Initialize() {
    FontHandler::logger = spdlog::get("FontHandler");
    FontHandler::logger->trace("FontHandler::Initialize()");
    TTF_Init();

    FontHandler::fonts[FontType::Console] = TTF_OpenFont("./Resources/Fonts/NotoSansMono-Regular.ttf", 18);
    FontHandler::fonts[FontType::Ui] = TTF_OpenFont("./Resources/Fonts/NotoSerif-Regular.ttf", 18);
    FontHandler::logger->trace("FontHandler::Initialize()~");
}

void FontHandler::Destroy() {
    FontHandler::logger->trace("FontHandler::Destroy()");
    for (auto pair : FontHandler::fonts) {
        TTF_CloseFont(pair.second);
    }
    FontHandler::fonts.clear();

    TTF_Quit();
    FontHandler::logger->trace("FontHandler::Destroy()~");
}

TTF_Font* FontHandler::GetFont(FontType type) {
    FontHandler::logger->trace("FontHandler::GetFont(FontType type = {})", (int)type);
    FontHandler::logger->trace("FontHandler::GetFont()~");
    return FontHandler::fonts[type];
}
}  // namespace SFG
