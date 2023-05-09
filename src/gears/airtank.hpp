#pragma once

#include "context.hpp"

struct AirTankInfo {
	u32 itemStorage[3];
    u32 stoSize;
    u32 itemUsed;
};

extern struct AirTankInfo PlayerAirTankInfo[8];

extern "C" u32 AntiItemCampHandler(struct Player *player, u32 item);

extern "C" void Player_AirTank(struct Player *player);
