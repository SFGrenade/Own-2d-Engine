#include "fonthandler.h"

namespace SFG {
    FontHandler* FontHandler::instance = nullptr;

    FontHandler::FontHandler()
      : fonts() {
        spdlog::trace("FontHandler::FontHandler()");
        TTF_Init();

        fonts[FontType::Console] = TTF_OpenFont("./Resources/Fonts/NotoSansMono-Regular.ttf", 18);
        fonts[FontType::Ui] = TTF_OpenFont("./Resources/Fonts/NotoSerif-Regular.ttf", 18);
        spdlog::trace("FontHandler::FontHandler()~");
    }

    FontHandler::~FontHandler() {
        spdlog::trace("FontHandler::~FontHandler()");
        for (auto pair : fonts) {
            TTF_CloseFont(pair.second);
        }
        fonts.clear();

        TTF_Quit();
        spdlog::trace("FontHandler::~FontHandler()~");
    }

    FontHandler* FontHandler::GetInstance() {
        spdlog::trace("FontHandler::GetInstance()");
        if (!FontHandler::instance) FontHandler::instance = new FontHandler();
        spdlog::trace("FontHandler::GetInstance()~");
        return FontHandler::instance;
    }

    TTF_Font* FontHandler::GetFont(FontType type) {
        spdlog::trace("FontHandler::GetFont(FontType type = {})", (int) type);
        spdlog::trace("FontHandler::GetFont()~");
        return fonts[type];
    }
}
