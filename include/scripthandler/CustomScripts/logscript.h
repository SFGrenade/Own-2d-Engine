#pragma once

// Including our headers
#include "_globals/spdlogInclude.h"
#include "scripthandler/script.h"

namespace SFG {
class LogScript : public Script {
  private:
  spdlogger logger;

  public:
  LogScript();
  virtual void Start();
  virtual void UpdateFrame();
  virtual void UpdateLogicFrame();
  virtual void End();
};
}  // namespace SFG
