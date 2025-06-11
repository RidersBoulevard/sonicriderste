#pragma once

#include "riders/object.hpp"
#include "riders/player.hpp"

struct HUDIconObject1 {
    std::array<u8, 4> opacity;
    std::array<u8, 4> opacityWithTricking; // this is the opacity for if you want the HUD element to be visible while tricking (like the air gauge)
    std::array<bool, 4> isFullyOpaque; // used for the "blinking" effect
};
static_assert(sizeof(HUDIconObject1) < 0x20);

ASMDefined ObjectNode * gpsCurrentTask;
ASMDefined void* texList_GTSE_0;

using HUDVisibleFunc = bool (*)(const Player& player);