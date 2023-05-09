#include "thecrazy.hpp"
#include "cosmetics/player/exloads.hpp"
#include "handlers/player/initgeardata.hpp"
#include "supertails.hpp"
#include <cmath>

global void lbl_Player_BoostEndFunction(Player *player);

constexpr s32 TheCrazy_DriftCost[4] = {-25, -50, -75, -100};
constexpr s32 TheCrazy_BoostCost[5] = {3000, 5000, 10000, 20000, 30000};
constexpr f32 TheCrazy_BoostSpeeds[5] = {pSpeed(220.0f), pSpeed(245.0f), pSpeed(270.0f), pSpeed(295.0f), pSpeed(310.0f)};
constexpr s32 TheCrazy_BoostCost2[3] = {10000, 15000, 20000};
constexpr f32 TheCrazy_BoostSpeeds2[3] = {pSpeed(240.0f), pSpeed(285.0f), pSpeed(300.0f)};

void Player_DriftTips(Player *player) {
	// Checks length of time drifted for, increases ring gain rate as length of time drifted increases
	s32 driftDashFrames;
	s32 driftCost;
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
	if(exLoads.gearExLoadID == TheBeastEXLoad) return;
	if(player->extremeGear != TheCrazy) return;
	if(player->driftDashFrames <= 40) {
		driftCost = TheCrazy_DriftCost[0];
	} else if(player->driftDashFrames <= 80) {
		driftCost = TheCrazy_DriftCost[1];
	} else if(player->driftDashFrames <= 120) {
		driftCost = TheCrazy_DriftCost[2];
	} else if(player->driftDashFrames > 120) {
		driftCost = TheCrazy_DriftCost[3];
	}
	player->gearStats[player->level].driftCost = driftCost;
}

void Player_LimiterCut(Player *player) {
	// Checks current speed and adjusts boost speed and cost accordingly. Has boost archetype support.
	f32 boostSpeed;
	s32 boostCost;
	u32 playerTopSpeed = u32(std::round(player->gearStats[player->level].topSpeed * SPEED_DIVISOR));
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
	if(exLoads.gearExLoadID == TheBeastEXLoad) return;
	if(player->extremeGear != TheCrazy) return;
	if(player->movementFlags & 0x0400) return;
	if(player->unkABC >= playerTopSpeed - 1) {
		boostCost = TheCrazy_BoostCost2[0];
		boostSpeed = TheCrazy_BoostSpeeds2[0];
	} else if(player->unkABC < (playerTopSpeed - 1) && player->unkABC >= (playerTopSpeed - 35)) {
		boostCost = TheCrazy_BoostCost2[1];
		boostSpeed = TheCrazy_BoostSpeeds2[1];
	}
	// else if (player->unkABC >= playerTopSpeed - 60)
	// {
	//     boostCost = TheCrazy_BoostCost2[2];
	//     boostSpeed = TheCrazy_BoostSpeeds2[2];
	// }
	// else if (player->unkABC >= playerTopSpeed - 60)
	// {
	//     boostCost = TheCrazy_BoostCost[3];
	//     boostSpeed = TheCrazy_BoostSpeeds[3];
	// }
	else if(player->unkABC < playerTopSpeed - 35) {
		boostCost = TheCrazy_BoostCost2[2];
		boostSpeed = TheCrazy_BoostSpeeds2[2];
		// boostCost = TheCrazy_BoostCost[4];
		// boostSpeed = TheCrazy_BoostSpeeds[4];
	}
	player->gearStats[player->level].boostCost = boostCost;
	if(player->characterArchetype == BoostArchetype) {
		boostSpeed += BoostArchetypeBoostSpeeds[player->level];
		player->gearStats[player->level].boostSpeed = boostSpeed;
	} else {
		player->gearStats[player->level].boostSpeed = boostSpeed;
	}
}

void Player_CrazyStop(Player *player) {
	// If the player brakes above 170 and is only braking and not drifting, immediately cap speed at 170 and end boost state.
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
	if(exLoads.gearExLoadID == TheBeastEXLoad) return;
	if(player->extremeGear != TheCrazy) return;

	if((player->movementFlags & braking) && (!(player->movementFlags & drifting))) {
		lbl_Player_BoostEndFunction(player);
		if(player->speed >= player->gearStats[player->level].topSpeed) player->speed = player->gearStats[player->level].topSpeed;
	}
}

void Player_TheBeast(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
	if(exLoads.gearExLoadID != TheBeastEXLoad) { return; }
	if(player->extremeGear != TheCrazy) { return; }

	SNDF_SeqIdStop(player->index + 50);
	SNDF_SeqIdStop(player->index + 46);

	if(player->state == StartLine || player->state == Run) {
		player->specialFlags &= ~(ringGear | fiftyPercentAir | stickyFingers);
		switch(player->y_toggle) {
			case 0:
				player->specialFlags &= ~noPits;
				break;
			case 1:
				player->specialFlags |= noPits;
				break;
		}
		player->level = 2;
		player->currentAir = 0;
		player->shortcutAirGainMultiplier = 0x0U;
		player->unk9C8 = 0x0U;// trick air gain multiplier
		player->unk9D0 = 0x0U;// QTE air gain multiplier
		player->specialFlags |= (legendEffect | noBoosting);
		if(player->specialFlags & berserkerEffect && !(player->movementFlags & boosting) && player->state != PlayerState::AttackingPlayer) { player->unkF40 = 0; }
		player->specialFlags &= ~berserkerEffect;
	}
	// else if (player->state == QTE || player->state == QTE2)
	// {
	//     player->level = 2;
	//     player->currentAir = 0;
	//     player->shortcutAirGainMultiplier = (s32)0x0;
	//     player->unk9D0 = (u32)0x0; // QTE air gain multiplier
	//     player->unk9C8 = (u32)0x0; // trick air gain multiplier
	//     player->specialFlags |= (legendEffect | noPits | noBoosting);
	//     if (player->specialFlags & berserkerEffect && !(player->movementFlags & boosting) && player->state != AttackingPlayer) player->unkF40 = 0;
	//     player->specialFlags &= ~berserkerEffect;
	// }

	if(player->input->toggleFaceButtons & XButton && player->state == Run) {// player->input->toggleFaceButtons & XButton && player->state == Run
		// if (player->rings < 30) {
		//     player->level = 0;
		// } else if (player->rings < 60) {
		//     player->level = 1;
		// } else if (player->rings <= 100) {
		//     player->level = 2;
		// }
		s32 signedRings = (s32) player->rings;
		player->currentAir += player->rings * 2000;
		player->gearStats[player->level].baseTopSpeed = pSpeed((f32) (signedRings) + 200.0f);
		player->gearStats[player->level].topSpeed = pSpeed((f32) (signedRings) + 200.0f);
		player->specialFlags &= ~(legendEffect);
	}
	if(player->flags & InAPit) {
		f32 maxSpeedCalc = pSpeed((f32) ((player->currentAir / player->gearStats[player->level].maxAir) * 100.0f) + 200.0f);
		if(maxSpeedCalc > pSpeed(300.0f)) maxSpeedCalc = pSpeed(300.0f);
		player->gearStats[player->level].baseTopSpeed = maxSpeedCalc;
		player->gearStats[player->level].topSpeed = maxSpeedCalc;
		player->specialFlags |= legendEffect;
	}
	if(player->currentAir >= 1 && player->state != Run && !(player->flags & InAPit) && player->state != PlayerState::QTE && player->state != PlayerState::QTE2) {
		player->specialFlags |= (berserkerEffect | noBoosting | ringGear | stickyFingers | fiftyPercentAir);
		player->shortcutAirGainMultiplier = (s32) 0x3F000000;
		player->unk9C8 = (u32) 0x3F000000;// trick air gain multiplier
		player->unk9D0 = (u32) 0x0;       // QTE air gain multiplier
		player->gearStats[player->level].airDrain = 333;
		player->gearStats[player->level].driftCost = 20;
		player->gearStats[player->level].tornadoCost = 10000;
		// player->rings = 0;
	}
}