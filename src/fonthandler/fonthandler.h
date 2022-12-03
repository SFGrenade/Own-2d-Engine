#pragma once

// Include general stuff
#include "../globals.h"

namespace SFG {
enum FontType {
    Console,
    Ui,
};

class FontHandler {
   private:
    static spdlogger logger;
    static std::map<FontType, TTF_Font*> fonts;

   public:
    static void Initialize();
    static void Destroy();
    static TTF_Font* GetFont(FontType type);
};
}  // namespace SFG
