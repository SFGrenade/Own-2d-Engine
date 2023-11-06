#pragma once

#include <cstdint>

namespace SFG {
class Performance {
  private:
  static uint64_t graphicsLoops;
  static char padding1[64];
  static uint64_t lastGraphicsLoops;
  static char padding2[64];
  static uint64_t inputLoops;
  static char padding3[64];
  static uint64_t lastInputLoops;
  static char padding4[64];
  static uint64_t logicLoops;
  static char padding5[64];
  static uint64_t lastLogicLoops;
  static char padding6[64];

  public:
  static void AddGraphicsLoop();
  static void AddInputLoop();
  static void AddLogicLoop();
  static uint64_t GetGraphicsLoop();
  static uint64_t GetInputLoop();
  static uint64_t GetLogicLoop();
};
}  // namespace SFG
