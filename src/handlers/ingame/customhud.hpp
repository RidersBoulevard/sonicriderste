#pragma once

#include "riders/object.hpp"

struct EmerlTypeIconObject1 {
    std::array<u8, 4> opacity;
    std::array<bool, 4> isFullyOpaque; // used for the "blinking" effect
    [[gnu::unused]] fillerData<0x17> empty;
    [[gnu::unused]] bool endOfStruct;
};
static_assert(sizeof(EmerlTypeIconObject1) == 0x20);

ASMDefined ObjectNode * gpsCurrentTask;
ASMDefined void* texList_GTSE_0;