#include "faster.hpp"

void Player_Faster(Player *player) {
	// if (player->character == Character::E10G && player->extremeGear == ExtremeGear::AdvantageP) {
	// 	player->trickAirGainMultiplier = 0.5;
	// }

	// if(player->extremeGear != ExtremeGear::Faster) { return; }
	if(player->extremeGear != ExtremeGear::Accelerator) { return; }
	
	player->specialFlags &= ~SpecialFlags::ringGear;

	// if(player->characterArchetype == CharacterArchetype::Boost) {
	// 	player->gearStats[0].boostSpeed = pSpeed(240.0f);
	// 	player->gearStats[1].boostSpeed = pSpeed(255.0f);
	// 	player->gearStats[2].boostSpeed = pSpeed(270.0f);
	// }

	// if(player->characterArchetype == CharacterArchetype::Trickster) {
	// 	player->shortcutAirGainMultiplier = 1.0;
	// 	player->trickAirGainMultiplier = 1.0;
	// }
}