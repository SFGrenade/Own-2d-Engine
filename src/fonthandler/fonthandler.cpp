#include "fonthandler.h"

namespace SFG {
std::map<FontType, TTF_Font*> FontHandler::fonts =
    std::map<FontType, TTF_Font*>();

void FontHandler::Initialize() {
    spdlog::trace("FontHandler::Initialize()");
    TTF_Init();

    FontHandler::fonts[FontType::Console] =
        TTF_OpenFont("./Resources/Fonts/NotoSansMono-Regular.ttf", 18);
    FontHandler::fonts[FontType::Ui] =
        TTF_OpenFont("./Resources/Fonts/NotoSerif-Regular.ttf", 18);
    spdlog::trace("FontHandler::Initialize()~");
}

void FontHandler::Destroy() {
    spdlog::trace("FontHandler::Destroy()");
    for (auto pair : FontHandler::fonts) {
        TTF_CloseFont(pair.second);
    }
    FontHandler::fonts.clear();

    TTF_Quit();
    spdlog::trace("FontHandler::Destroy()~");
}

TTF_Font* FontHandler::GetFont(FontType type) {
    spdlog::trace("FontHandler::GetFont(FontType type = {})", (int)type);
    spdlog::trace("FontHandler::GetFont()~");
    return FontHandler::fonts[type];
}
}  // namespace SFG
