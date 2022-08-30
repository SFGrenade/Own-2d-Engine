#include "performance.h"

namespace SFG {
    uint32_t Performance::graphicsLoops = 0;
    std::mutex Performance::graphicsMtx;
    uint32_t Performance::inputLoops = 0;
    std::mutex Performance::inputMtx;
    uint32_t Performance::logicLoops = 0;
    std::mutex Performance::logicMtx;

    void Performance::AddGraphicsLoop() {
        Performance::graphicsMtx.lock();
        Performance::graphicsLoops += 1;
        Performance::graphicsMtx.unlock();
    }

    void Performance::AddInputLoop() {
        Performance::inputMtx.lock();
        Performance::inputLoops += 1;
        Performance::inputMtx.unlock();
    }

    void Performance::AddLogicLoop() {
        Performance::logicMtx.lock();
        Performance::logicLoops += 1;
        Performance::logicMtx.unlock();
    }

    uint32_t Performance::GetGraphicsLoop() {
        Performance::graphicsMtx.lock();
        uint32_t tmp = Performance::graphicsLoops;
        Performance::graphicsLoops = 0;
        Performance::graphicsMtx.unlock();
        return tmp;
    }

    uint32_t Performance::GetInputLoop() {
        Performance::inputMtx.lock();
        uint32_t tmp = Performance::inputLoops;
        Performance::inputLoops = 0;
        Performance::inputMtx.unlock();
        return tmp;
    }

    uint32_t Performance::GetLogicLoop() {
        Performance::logicMtx.lock();
        uint32_t tmp = Performance::logicLoops;
        Performance::logicLoops = 0;
        Performance::logicMtx.unlock();
        return tmp;
    }
}
