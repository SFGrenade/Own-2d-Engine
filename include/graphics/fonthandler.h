#pragma once

#include <map>

// Including our headers
#include "_globals/sdlInclude.h"
#include "_globals/spdlogInclude.h"

namespace SFG {
enum class FontType {
  Console,
  Ui,
};

class FontHandler {
  private:
  void deleteFont( TTF_Font* ptr );

  public:
  FontHandler();
  ~FontHandler();
  std::shared_ptr< TTF_Font > GetFont( FontType type );

  private:
  spdlogger logger_;
  std::map< FontType, std::shared_ptr< TTF_Font > > fonts_;
};
}  // namespace SFG
