#include "performance.h"

namespace SFG {
uint16_t Performance::graphicsLoops = 0;
char Performance::padding1[64];
uint16_t Performance::lastGraphicsLoops = 0;
char Performance::padding2[64];
uint32_t Performance::inputLoops = 0;
char Performance::padding3[64];
uint32_t Performance::lastInputLoops = 0;
char Performance::padding4[64];
uint32_t Performance::logicLoops = 0;
char Performance::padding5[64];
uint32_t Performance::lastLogicLoops = 0;
char Performance::padding6[64];

void Performance::AddGraphicsLoop() { ++Performance::graphicsLoops; }

void Performance::AddInputLoop() { ++Performance::inputLoops; }

void Performance::AddLogicLoop() { ++Performance::logicLoops; }

uint16_t Performance::GetGraphicsLoop() {
    uint16_t tmp = Performance::lastGraphicsLoops;
    Performance::lastGraphicsLoops = Performance::graphicsLoops;
    return Performance::graphicsLoops - tmp;
}

uint32_t Performance::GetInputLoop() {
    uint32_t tmp = Performance::lastInputLoops;
    Performance::lastInputLoops = Performance::inputLoops;
    return Performance::inputLoops - tmp;
}

uint32_t Performance::GetLogicLoop() {
    uint32_t tmp = Performance::lastLogicLoops;
    Performance::lastLogicLoops = Performance::logicLoops;
    return Performance::logicLoops - tmp;
}
}  // namespace SFG
