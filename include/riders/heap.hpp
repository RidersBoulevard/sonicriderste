#pragma once

#include "globalDefs.hpp"
#include "types.hpp"

struct HeapInfo {
    u32 freeBytes;
    u32 hiHeapUsedBytes;
    void *loHeapFreePtr;
    void *loHeapStartPtr;
    void *hiHeapFreePtr;
    void *hiHeapStartPtr;

    void updateLowHeap(void *newLoHeapStart) {
        loHeapFreePtr = newLoHeapStart;
        loHeapStartPtr = newLoHeapStart;
        freeBytes = reinterpret_cast<size_t>(hiHeapStartPtr) - reinterpret_cast<size_t>(newLoHeapStart) + 0xC;
    }
};
static_assert(sizeof(HeapInfo) == 0x18);

ASMDefined HeapInfo gsHeapInfo;