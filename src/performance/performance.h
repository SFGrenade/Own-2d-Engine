#pragma once

// Include general stuff
#include "../globals.h"

#include <mutex>          // std::mutex

namespace SFG {
    class Performance
    {
    private:
        static uint32_t graphicsLoops;
        static std::mutex graphicsMtx;
        static uint32_t inputLoops;
        static std::mutex inputMtx;
        static uint32_t logicLoops;
        static std::mutex logicMtx;
    public:
        static void AddGraphicsLoop();
        static void AddInputLoop();
        static void AddLogicLoop();
        static uint32_t GetGraphicsLoop();
        static uint32_t GetInputLoop();
        static uint32_t GetLogicLoop();
    };
}
