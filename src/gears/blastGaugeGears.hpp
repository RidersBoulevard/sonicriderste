#pragma once

#include "globalDefs.hpp"
#include "riders/player.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct BlastGaugeInfo {
	s32 currentGauge; // stores gauge value
};
#pragma GCC diagnostic pop

extern std::array<BlastGaugeInfo, MaxPlayerCount> PlayerBlastGaugeInfo;

void Player_BlastGaugeUpdateStatus(Player &player);