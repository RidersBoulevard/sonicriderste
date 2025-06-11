#include "thecrazy.hpp"
#include "cosmetics/player/exloads.hpp"
#include "handlers/player/initgeardata.hpp"
#include "supertails.hpp"
#include <cmath>

constexpr std::array<s32, 4> TheCrazy_DriftCost = {-25, -50, -75, -100};
// constexpr std::array<s32, 5> TheCrazy_BoostCost = {3000, 5000, 10000, 20000, 30000};
// constexpr std::array<f32, 5> TheCrazy_BoostSpeeds = {pSpeed(220.0F), pSpeed(245.0F), pSpeed(270.0F), pSpeed(295.0F), pSpeed(310.0F)};
constexpr std::array<s32, 4> TheCrazy_BoostCost2 = {10000, 15000, 20000, 30000};
constexpr std::array<f32, 4> TheCrazy_BoostSpeeds2 = {pSpeed(240.0f), pSpeed(275.0f), pSpeed(290.0f), pSpeed(300.0f)};

void Player_DriftTips(Player *player) {
	// Checks length of time drifted for, increases ring gain rate as length of time drifted increases
	if(player->gearExload().exLoadID == EXLoad::TheBeast) { return; }
	if(player->extremeGear != ExtremeGear::TheCrazy) { return; }
	//s32 driftDashFrames;
	s32 driftCost = 0;
	if(player->driftDashFrames <= 40) {
		driftCost = TheCrazy_DriftCost[0];
	} else if(player->driftDashFrames <= 80) {
		driftCost = TheCrazy_DriftCost[1];
	} else if(player->driftDashFrames <= 120) {
		driftCost = TheCrazy_DriftCost[2];
	} else if(player->driftDashFrames > 120) {
		driftCost = TheCrazy_DriftCost[3];
	}
	player->gearStats[player->level].driftCost = (player->characterArchetype == CharacterArchetype::Drift) ? (driftCost * 103) / 100 : driftCost;
}

void Player_LimiterCut(Player *player) {
	// Checks current speed and adjusts boost speed and cost accordingly. Has boost archetype support.
	if(player->extremeGear != ExtremeGear::TheCrazy) { return; }
	if(player->movementFlags.hasAny(MovementFlags::boosting)) { return; }
	if(player->gearExload().exLoadID == EXLoad::TheBeast) { return; }
	const auto playerTopSpeed = static_cast<u32>(std::round(player->gearStats[player->level].topSpeed * SPEED_DIVISOR));
	s32 &boostCost = player->gearStats[player->level].boostCost;
	f32 &boostSpeed = player->gearStats[player->level].boostSpeed;
	if(player->unkABC >= playerTopSpeed - 1) {
		boostCost = TheCrazy_BoostCost2[0];
		boostSpeed = TheCrazy_BoostSpeeds2[0];
	} else if(player->unkABC < (playerTopSpeed - 1) && player->unkABC >= (playerTopSpeed - 35)) {
		boostCost = TheCrazy_BoostCost2[1];
		boostSpeed = TheCrazy_BoostSpeeds2[1];
	} else if(player->unkABC < (playerTopSpeed - 35) && player->unkABC >= (playerTopSpeed - 85)) {
		boostCost = TheCrazy_BoostCost2[2];
		boostSpeed = TheCrazy_BoostSpeeds2[2];
	}
	else if(player->unkABC < (playerTopSpeed - 85)) {
		boostCost = TheCrazy_BoostCost2[3];
		boostSpeed = TheCrazy_BoostSpeeds2[3];
	}
	if(player->characterArchetype == CharacterArchetype::Boost) {
		boostSpeed += BoostArchetypeBoostSpeeds[player->level];
	}
}

void Player_CrazyStop(Player *player) {
	if(player->extremeGear != ExtremeGear::TheCrazy) { return; }
	if(player->gearExload().exLoadID == EXLoad::TheBeast) { return; }

	// If the player brakes above 170 and is only braking and not drifting, immediately cap speed at 170 and end boost state.
	if(player->movementFlags.hasAny(MovementFlags::braking) && (!player->movementFlags.hasAny(MovementFlags::drifting))) {
		if(player->movementFlags.hasAny(MovementFlags::boosting)) {lbl_Player_BoostEndFunction(player);}
		if(player->speed >= player->gearStats[player->level].topSpeed) {
			player->speed = player->gearStats[player->level].topSpeed;
		}
	}
}

void Player_TheBeast(Player *player) {
	if(player->extremeGear != ExtremeGear::TheCrazy) { return; }
	if(player->gearExload().exLoadID != EXLoad::TheBeast) { return; }

	SNDF_SeqIdStop(player->index + 50);
	SNDF_SeqIdStop(player->index + 46);

	if(player->state == PlayerState::StartLine || player->state == PlayerState::Run) {
		player->specialFlags &= ~(SpecialFlags::ringGear | SpecialFlags::fiftyPercentAir | SpecialFlags::stickyFingers);
		switch(player->y_toggle) { // Change to if/else?
			case 0:
				player->specialFlags &= ~SpecialFlags::noPits;
				break;
			case 1:
				player->specialFlags |= SpecialFlags::noPits;
				break;
			default: break;
		}
		player->level = 2;
		player->currentAir = 0;
		player->shortcutAirGainMultiplier = 0;
		player->trickAirGainMultiplier = 0;
		player->QTEAirGainMultiplier = 0;
		player->specialFlags |= (SpecialFlags::legendEffect | SpecialFlags::noBoosting);
		if(player->specialFlags.hasAny(SpecialFlags::berserkerEffect) &&
		   !player->movementFlags.hasAny(MovementFlags::boosting) &&
		   player->state != PlayerState::AttackingPlayer) {
			player->attackProperties = nullptr;
		}
		player->specialFlags &= ~SpecialFlags::berserkerEffect;
	}

	if(player->input->toggleFaceButtons.hasAny(Buttons::X) && player->state == PlayerState::Run) {
		const auto &rings = player->rings;
		player->currentAir += static_cast<s32>(rings * 2000);
		player->gearStats[player->level].baseTopSpeed = pSpeed(static_cast<f32>(rings) + 200.0F);
		player->gearStats[player->level].topSpeed = pSpeed(static_cast<f32>(rings) + 200.0F);
		player->specialFlags &= ~SpecialFlags::legendEffect;
	}
	if(player->flags.hasAny(PlayerFlags::InAPit)) {
		f32 maxSpeedCalc = pSpeed(((static_cast<f32>(player->currentAir) / player->gearStats[player->level].maxAir) * 100.0F) + 200.0F);
		if(maxSpeedCalc > pSpeed(300.0F)) { maxSpeedCalc = pSpeed(300.0f); }
		player->gearStats[player->level].baseTopSpeed = maxSpeedCalc;
		player->gearStats[player->level].topSpeed = maxSpeedCalc;
		player->specialFlags |= SpecialFlags::legendEffect;
	}
	if(player->currentAir >= 1 && player->state != PlayerState::Run && !player->flags.hasAny(PlayerFlags::InAPit) && player->state != PlayerState::QTE && player->state != PlayerState::QTE2) {
		player->specialFlags |= (SpecialFlags::berserkerEffect | SpecialFlags::noBoosting | SpecialFlags::ringGear | SpecialFlags::stickyFingers | SpecialFlags::fiftyPercentAir);
		player->shortcutAirGainMultiplier = 0.5;
		player->trickAirGainMultiplier = 0.5;// trick air gain multiplier
		player->QTEAirGainMultiplier = 0;       // QTE air gain multiplier
		player->gearStats[player->level].airDrain = 333;
		player->gearStats[player->level].driftCost = 20;
		player->gearStats[player->level].tornadoCost = 10000;
	}
}