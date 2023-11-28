#include "turbostar.hpp"
#include "handlers/player/initgeardata.hpp"

void Player_TurboStar(Player *player) {
	if(player->extremeGear != ExtremeGear::TurboStar) { return; }
	if(player->characterArchetype == BoostArchetype && player->gearSpecificFlags[TurboStar::Level4]) {
		player->gearStats[player->level].boostSpeed = pSpeed(263.0F);
	} else if(player->characterArchetype == BoostArchetype) {
		player->gearStats[player->level].boostSpeed = player->gearptr->levelStats[player->level].boostSpeed + BoostArchetypeBoostSpeeds[player->level];
	}
}