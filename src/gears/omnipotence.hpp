#pragma once

#include "riders/player.hpp"
#include "lib/stdlib.hpp"
#include "riders/object.hpp"

struct OmnipotenceInfo {
	u8 lastShortcutType; // 1 = speed, 2 = fly, 3 = power
    bool FirstRail;
    bool jumpedFromRail;
    bool hasFlown;
    // f32 railJumpDelay;
    bool qteEntered;
    bool firstPowerObj;
};

extern std::array<OmnipotenceInfo, MaxPlayerCount> PlayerOMNInfo;

void Player_Omnipotence(Player &player);