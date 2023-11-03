#pragma once

// Include general stuff
#include "globals.h"

namespace SFG {
class ConfigHandler {
  private:
  static spdlogger logger;

  public:
  static void Initialize();
  static void Destroy();
};
}  // namespace SFG
