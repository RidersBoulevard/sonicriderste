#include "boostarchetypejcbc.hpp"

std::array<DriftInfo, MaxPlayerCount> PlayerDriftInfo;

ASMUsed void Player_BoostArchetypeJCBC(Player *player) {
	if(player->characterArchetype != CharacterArchetype::Boost) { return; }
	if(player->state != PlayerState::Cruise) { return; }
	DriftInfo *driftInfo = &PlayerDriftInfo[player->index];
	// player->movementFlags.hasAny(MovementFlags::drifting)
	if(player->input->holdFaceButtons.hasAny(Buttons::L, Buttons::R)
		|| player->input->toggleFaceButtons.hasAny(Buttons::L, Buttons::R)) {
		if(!driftInfo->hasHeldDrift &&
		   player->input->toggleFaceButtons.hasAny(Buttons::B, Buttons::X) &&
		   player->input->holdFaceButtons.hasAny(Buttons::A) &&
		   player->jumpCharge > 0) {
			lbl_Player_BoostEndFunction(player);
			driftInfo->hasHeldDrift = TRUE;
		}
	} else {
		driftInfo->hasHeldDrift = FALSE;
	}
}

void Player_windcatcher(Player *player) {
	if(player->characterArchetype != CharacterArchetype::Windcatcher) { return; }
	auto &windcatcherTurbGain = MI::impulseData[player->index].windcatcherTurbGain;
	if(player->state == PlayerState::TurbulenceRide) {
		windcatcherTurbGain += 1;
	} else {
		windcatcherTurbGain = 0;
	}

	if(windcatcherTurbGain % 30 == 0 && windcatcherTurbGain != 0 && (player->state == PlayerState::TurbulenceRide)) {
		if(player->extremeGear == ExtremeGear::SuperHangOn) {
			player->currentAir += 4000;
		} else if(player->specialFlags.hasAny(SpecialFlags::ringGear)) {
			player->currentAir += (player->gearStats[player->level].maxAir * 1) / 100;
		} else {
			player->currentAir += 10000;
		}
	}
}