#pragma once

#include "types.hpp"
#include <ogc/gx_struct.h>
#include <ogc/gx.h>

constexpr auto DEFAULT_FIFO_SIZE = 256 * 1024;

struct RidersVideoData {
    /* 0x0 */ std::array<void*, 2> framebuffer;
    /* 0x8 */ void* ptr3;
    /* 0xC */ void* ptr4;
    /* 0x10 */ GXFifoObj *fifo;
    /* 0x14 */ GXRModeObj *rmode;
};

extern RidersVideoData lbl_801AF600;