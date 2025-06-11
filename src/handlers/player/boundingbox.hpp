#pragma once

#include "riders/object.hpp"
#include "riders/player.hpp"

// NOTE: make this a 12-length array for all possible players
ASMDefined float asm_BoundingBoxDownwardsYVal;

ASMUsed float Player_BoundingBoxAdjustments(Player &player);
