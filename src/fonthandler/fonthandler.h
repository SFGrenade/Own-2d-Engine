#pragma once

// Include general stuff
#include "../globals.h"

// Including SDL headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <map>
#include <string>

namespace SFG {
    enum FontType {
        Console,
        Ui,
    };

    class FontHandler
    {
    private:
        static FontHandler* instance;
        std::map<FontType, TTF_Font*> fonts;
        FontHandler();
    public:
        ~FontHandler();
        static FontHandler* GetInstance();
        TTF_Font* GetFont(FontType type);
    };
}
