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

ASMUsed u32 AntiItemCampHandler(Player *player, u32 item);

ASMUsed void Player_AirTank(Player *player);
