#pragma once

#include "riders/player.hpp"

struct HHOInfo {
	u8 attackCount; // stores successful attacks
	u8 trickCount; // stores tricks
    u8 pitCount; // stores pits
    u8 saturnMegadriveStatus; // 0 = init, 1 = inactive, 2 = active, 3 = had previously
	u32 extraType;
};

extern HHOInfo PlayerHHOInfo[8];
extern const f32 HHO_BoostSpeeds[4];
extern const f32 HHOBoostControlSpeeds[4];
extern const f32 HHOBoostControlSpeedsBoostArchetype[4];

ASMDefined void lbl_Player_BoostEndFunction(Player *);

void Player_HyperHangOn(Player *player);