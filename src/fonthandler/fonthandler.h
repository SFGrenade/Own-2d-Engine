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
  static std::map< FontType, std::shared_ptr< TTF_Font > > fonts;

  static void deleteFont( TTF_Font* ptr );

  public:
  static void Initialize();
  static void Destroy();
  static std::shared_ptr< TTF_Font > GetFont( FontType type );
};
}  // namespace SFG
