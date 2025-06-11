#include "turbostar.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"



void Player_TurboStar(Player &player) {
	if(player.extremeGear != ExtremeGear::TurboStar) { return; }
	
	if(player.characterArchetype == CharacterArchetype::Boost && player.gearSpecificFlags[TurboStar::Level4]) {
		player.gearStats[player.level].boostSpeed = pSpeed(263.0F);
	} else if(player.characterArchetype == CharacterArchetype::Boost) {
		player.gearStats[player.level].boostSpeed = player.gearptr->levelStats[player.level].boostSpeed + BoostArchetypeBoostSpeeds[player.level];
	}
}