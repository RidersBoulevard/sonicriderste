/** \file gx.cpp
 * Contains wrappers around GX functions from Dolphin SDK for backwards compatibility and an altered GX init function.
 */

#include "gx.hpp"

#include <lib/stdlib.hpp>
#include <ogc/gx_struct.h>
#include <ogc/gx.h>
#include <ogc/system.h>
#include <ogc/video_types.h>
#include <ogc/video.h>
#include <cstring>

#include "globalDefs.hpp"
#include "types.hpp"
#include "alignment.hpp"
#include "main.hpp"
#include "riders/heap.hpp"


/// Initializes the GX subsystem.
ASMUsed void RIDERS_InitGX() {
    void *arenaLo = gsHeapInfo.loHeapFreePtr;

    // setup 1st framebuffer
    auto *nextFb = align_ptr<void*>(arenaLo, 32);
    lbl_801AF600.framebuffer[0] = MEM_K0_TO_K1(nextFb);
    lbl_801AF600.ptr3 = MEM_K0_TO_K1(nextFb);

    auto *rmode = lbl_801AF600.rmode;
    const auto fbSize = VIDEO_GetFrameBufferSize(rmode);

    // setup 2nd framebuffer
    auto *fb = reinterpret_cast<void*>(reinterpret_cast<u32>(nextFb) + fbSize);
    fb = align_ptr<void*>(fb, 32);

    // setup gx's work area
    auto *gxBase = reinterpret_cast<void*>(reinterpret_cast<u32>(fb) + fbSize);
    gxBase = align_ptr<void*>(gxBase, 32);
    memset(gxBase, 0, DEFAULT_FIFO_SIZE);

    lbl_801AF600.framebuffer[1] = MEM_K0_TO_K1(fb);
    lbl_801AF600.ptr4 = gxBase;

    // init gx
    auto *gxFifo = GX_Init(gxBase, DEFAULT_FIFO_SIZE);
    lbl_801AF600.fifo = gxFifo;

    auto *newArenaLo = reinterpret_cast<void*>(reinterpret_cast<u32>(gxBase) + DEFAULT_FIFO_SIZE);
    gNp_InitHeap(newArenaLo, SYS_GetArenaHi()); // reinit heap with new arenas
    SYS_SetArenaLo(SYS_GetArenaHi());
}

/// Wrapper around GX_GetYScaleFactor.
ASMUsed f32 RIDERS_GXGetYScaleFactor() {
    return GX_GetYScaleFactor(lbl_801AF600.rmode->efbHeight, lbl_801AF600.rmode->xfbHeight);
}