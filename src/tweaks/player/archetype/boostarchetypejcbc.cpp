#include "boostarchetypejcbc.hpp"

std::array<DriftInfo, MaxPlayerCount> PlayerDriftInfo;

ASMUsed void Player_BoostArchetypeJCBC(Player *player) {
	if(player->characterArchetype != BoostArchetype) { return; }
	DriftInfo *driftInfo = &PlayerDriftInfo[player->index];
	if(player->movementFlags.hasAny(drifting)) {
		if(!driftInfo->hasHeldDrift &&
		   player->input->toggleFaceButtons.hasAny(BButton, XButton) &&
		   player->input->holdFaceButtons.hasAny(AButton) &&
		   player->jumpCharge > 0) {
			lbl_Player_BoostEndFunction(player);
			driftInfo->hasHeldDrift = TRUE;
		}
	} else {
		driftInfo->hasHeldDrift = FALSE;
	}
}

void Player_windcatcher(Player *player) {
	if(player->characterArchetype != Windcatcher) { return; }
	auto &windcatcherTurbGain = MI::impulseData[player->index].windcatcherTurbGain;
	if(player->state == TurbulenceRide) {
		windcatcherTurbGain += 1;
	} else {
		windcatcherTurbGain = 0;
	}

	if(windcatcherTurbGain % 30 == 0 && windcatcherTurbGain != 0 && (player->state == TurbulenceRide)) {
		if(player->extremeGear == ExtremeGear::SuperHangOn) {
			player->currentAir += 4000;
		} else if(player->specialFlags.hasAny(ringGear)) {
			player->currentAir += (player->gearStats[player->level].maxAir * 1) / 100;
		} else {
			player->currentAir += 10000;
		}
	}
}