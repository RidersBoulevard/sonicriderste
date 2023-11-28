#include "faster.hpp"

void Player_Faster(Player *player) {
	if (player->character == E10G && player->extremeGear == ExtremeGear::AdvantageP) {
		player->trickAirGainMultiplier = 0.9;
	}

	if(player->extremeGear != ExtremeGear::Faster) { return; }
	if(player->characterArchetype == BoostArchetype) {
		player->gearStats[0].boostSpeed = pSpeed(240.0f);
		player->gearStats[1].boostSpeed = pSpeed(255.0f);
		player->gearStats[2].boostSpeed = pSpeed(270.0f);
	}

	if(player->characterArchetype == TricksterArchetype) {
		player->shortcutAirGainMultiplier = 1.0;
		player->trickAirGainMultiplier = 1.0;
	}
}