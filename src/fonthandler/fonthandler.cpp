#include "fonthandler.h"

namespace SFG {
    FontHandler* FontHandler::instance;

    FontHandler::FontHandler()
      : fonts() {
        TTF_Init();

        fonts[FontType::Console] = TTF_OpenFont("./Resources/Fonts/NotoSansMono-Regular.ttf", 18);
        fonts[FontType::Ui] = TTF_OpenFont("./Resources/Fonts/NotoSerif-Regular.ttf", 18);
    }

    FontHandler::~FontHandler() {
        for (auto pair : fonts) {
            TTF_CloseFont(pair.second);
        }
        fonts.clear();

        TTF_Quit();
    }

    FontHandler* FontHandler::GetInstance() {
        if (!FontHandler::instance) FontHandler::instance = new FontHandler();
        return FontHandler::instance;
    }

    TTF_Font* FontHandler::GetFont(FontType type) {
        return fonts[type];
    }
}
