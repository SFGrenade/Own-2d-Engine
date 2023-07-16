#pragma once

// Including our headers
#include "../globals.h"

namespace SFG {
class Script {
  protected:
  // some things
  public:
  Script() {}
  virtual ~Script() {}
  virtual void Start() {}
  virtual void UpdateFrame() {}
  virtual void UpdateLogicFrame() {}
  virtual void End() {}
};
}  // namespace SFG
