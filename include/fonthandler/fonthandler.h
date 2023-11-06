#pragma once

#include <map>

// Including our headers
#include "_globals/spdlogInclude.h"
#include "_globals/sdlInclude.h"

namespace SFG {
enum class FontType {
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
