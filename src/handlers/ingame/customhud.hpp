#pragma once

#include "context.hpp"

struct EmerlTypeIconObject1 {
    u8 opacity[4];
    bool isFullyOpaque[4]; // used for the "blinking" effect
    [[gnu::unused]] char empty[0x17];
    [[gnu::unused]] bool endOfStruct;
};
static_assert(sizeof(EmerlTypeIconObject1) == 0x20);

global Object* gpsCurrentTask;
global void* texList_GTSE_0;