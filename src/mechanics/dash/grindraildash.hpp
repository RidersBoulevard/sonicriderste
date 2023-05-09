#pragma once

#include "main.hpp"

extern const f32 GrindRailDashAirCosts[3];
extern const s32 GrindRailDashAirCostsRingGear;

struct FillerAttackStruct {
	char filler[0x34];
};

struct BoostParticleObject1 {
	char filler[0x38];
	Player *player;
	struct FillerAttackStruct *attackStruct;
	u32 unknown;
	f32 unkfloat;
};

void Player_GrindRailDashHandler(Player *player);
void Player_InitBoostParticles(Player *player);