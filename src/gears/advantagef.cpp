#include "advantagef.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"

constexpr f32 AdvantageF_BoostSpeeds[] = {pSpeed(200.0f), pSpeed(210.0f), pSpeed(220.0f), pSpeed(230.0f),
										   pSpeed(240.0f), pSpeed(250.0f), pSpeed(260.0f)};

void Player_AdvantageF(Player *player) {
	if (player->movementFlags & 0x0400) return;
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
	// Checks current speed and adjusts boost speed and cost accordingly. Has boost archetype support.
	if (player->extremeGear != AdvantageF || exLoads.gearExLoadID == TheProfessionalEXLoad) return;
	player->specialFlags |= (ringGear | noSpeedLossChargingJump | iceImmunity);

	f32 boostSpeed;
	if (player->rings <= 9) {
		boostSpeed = AdvantageF_BoostSpeeds[0];
	} else if (player->rings <= 19) {
		boostSpeed = AdvantageF_BoostSpeeds[1];
	} else if (player->rings <= 39) {
		boostSpeed = AdvantageF_BoostSpeeds[2];
	} else if (player->rings <= 59) {
		boostSpeed = AdvantageF_BoostSpeeds[3];
	} else if (player->rings <= 79) {
		boostSpeed = AdvantageF_BoostSpeeds[4];
	} else if (player->rings <= 89) {
		boostSpeed = AdvantageF_BoostSpeeds[5];
	} else { //if (player->rings <= 100)
		boostSpeed = AdvantageF_BoostSpeeds[6];
	}
	if (player->characterArchetype == BoostArchetype) {
		boostSpeed += BoostArchetypeBoostSpeeds[player->level];
	}

	player->gearStats[player->level].boostSpeed = boostSpeed;
}

void Player_TheProfessional(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
	if (exLoads.gearExLoadID != TheProfessionalEXLoad) return;
	player->specialFlags &= ~fiftyPercentAir;
	player->typeAttributes = 0;
	if (player->state == StartLine) player->currentAir = player->gearStats[player->level].maxAir;
	player->specialFlags |= (alwaysOnIce | noSpeedLossChargingJump | legendEffect);
	player->gearStats[0].airDrain = (s32)0x00000010;
	player->gearStats[1].airDrain = (s32)0x00000010;
	player->gearStats[2].airDrain = (s32)0x00000010;
	player->gearStats[0].boostCost = (s32)0x000061A8;
	player->gearStats[1].boostCost = (s32)0x00007530;
	player->gearStats[2].boostCost = (s32)0x00009C40;
	player->gearStats[0].tornadoCost = (s32)0x000061A8;
	player->gearStats[1].tornadoCost = (s32)0x00007530;
	player->gearStats[2].tornadoCost = (s32)0x00009C40;
	player->gearStats[0].driftCost = (s32)0x000000A6;
	player->gearStats[1].driftCost = (s32)0x000000FA;
	player->gearStats[2].driftCost = (s32)0x0000014D;
	player->gearStats[0].boostSpeed = player->gearStats[0].topSpeed;
	player->gearStats[1].boostSpeed = pSpeed(230.0f);
	player->gearStats[2].boostSpeed = pSpeed(250.0f);
	// player->gearStats[player->level].offroadSpeedCap = (f32)0xC1A00000; // speed balancer effect
	// player->gearStats[0].baseTopSpeed = pSpeed(162.0f);
	// player->gearStats[0].topSpeed = pSpeed(162.0f);
}