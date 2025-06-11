#include "afterburner.hpp"

void Player_Afterburner(Player *player) {
	if(player->characterArchetype != CharacterArchetype::Trickster) { return; }

	if(player->state == PlayerState::StartLine) {
		MI::impulseData[player->index].afterburnerTimer = 0;
	}

	// if(MI::impulseData[player->index].afterburnerTimer % 60 == 0 &&
	//    MI::impulseData[player->index].afterburnerTimer != 0 &&
	//    ((player->state >= PlayerState::Cruise && player->state <= PlayerState::Fall) || player->state == PlayerState::Run)) {
	// 	if(player->extremeGear == ExtremeGear::SuperHangOn) {
	// 		player->currentAir += 1000;
	// 	} else if(player->extremeGear == ExtremeGear::AdvantageF) {
	// 		player->currentAir += (player->gearStats[player->level].maxAir * 1) / 100;
	// 	} else if(player->specialFlags.hasAny(SpecialFlags::ringGear)) {
	// 		player->currentAir += (player->gearStats[player->level].maxAir * 2) / 100;
	// 	} else {
	// 		player->currentAir += 5000;
	// 	}
	// }
}