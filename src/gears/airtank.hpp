#pragma once

#include "types.hpp"
#include "riders/player.hpp"
#include "handlers/player/specialflagtweaks.hpp"
#include "lib/stdlib.hpp"

#include <array>

struct AirTankInfo {
	std::array<u32, 3> itemStorage;
    u32 stoSize;
    u32 itemUsed;
};

extern std::array<AirTankInfo, MaxPlayerCount> PlayerAirTankInfo;

ASMUsed void Player_TrapGear(Player *player);

void Player_AirTank(Player &player);

