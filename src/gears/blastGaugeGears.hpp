#pragma once

#include "riders/player.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct BlastGaugeInfo {
	s32 currentGauge; // stores gauge value
};
#pragma GCC diagnostic pop

extern std::array<BlastGaugeInfo, MaxPlayerCount> PlayerBlastGaugeInfo;

ASMUsed void Player_checkGearIfUsesBlastGauge(Player *player);
ASMUsed s32 Player_blastGaugeHandler(Player *player);
ASMUsed void Player_resetGauge(Player *player);
//ASMDefined void lbl_drawDynamicGauge();
//ASMDefined void lbl_airGearDrawAirGauge();
ASMDefined void lbl_drawAirGauge(); // allows drawing the air gauge for any gear if called
ASMDefined void lbl_0002AD54(); // skips drawing the gauge