#pragma once

// Include general stuff
#include "../globals.h"

namespace SFG {
    class Performance
    {
    private:
        static uint16_t graphicsLoops;
        static char padding1[64];
        static uint16_t lastGraphicsLoops;
        static char padding2[64];
        static uint32_t inputLoops;
        static char padding3[64];
        static uint32_t lastInputLoops;
        static char padding4[64];
        static uint32_t logicLoops;
        static char padding5[64];
        static uint32_t lastLogicLoops;
        static char padding6[64];
    public:
        static void AddGraphicsLoop();
        static void AddInputLoop();
        static void AddLogicLoop();
        static uint16_t GetGraphicsLoop();
        static uint32_t GetInputLoop();
        static uint32_t GetLogicLoop();
    };
}
