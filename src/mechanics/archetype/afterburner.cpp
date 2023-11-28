#include "afterburner.hpp"

void Player_Afterburner(Player *player) {
	if(player->characterArchetype != TricksterArchetype) { return; }

	if(player->state == StartLine) {
		MI::impulseData[player->index].afterburnerTimer = 0;
	}

	if(MI::impulseData[player->index].afterburnerTimer % 60 == 0 &&
	   MI::impulseData[player->index].afterburnerTimer != 0 &&
	   ((player->state >= Cruise && player->state <= Fall) || player->state == Run)) {
		if(player->extremeGear == ExtremeGear::SuperHangOn) {
			player->currentAir += 1000;
		} else if(player->extremeGear == ExtremeGear::AdvantageF) {
			player->currentAir += (player->gearStats[player->level].maxAir * 1) / 100;
		} else if(player->specialFlags.hasAny(ringGear)) {
			player->currentAir += (player->gearStats[player->level].maxAir * 2) / 100;
		} else {
			player->currentAir += 5000;
		}
	}
}