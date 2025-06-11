#pragma once

#include "globalDefs.hpp"
#include "types.hpp"
#include "riders/player.hpp"

ASMDefined void func_ClearPlayerMotionTable(Player &player);
ASMDefined void func__RegistPlayerMotionTable(Player &player, u32 animationID, u32 unknown, f32 smoothingMaybe, f32 unknown2);
ASMDefined void func_StartPlayerMotionTable(Player &player);