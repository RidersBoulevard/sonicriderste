#include "slidebooster.hpp"

ASMUsed u32 Player_SlideBoosterCantLeaveTurbulence(Player *player){
	return 1;
	// const Player &turbPlayer = players[player->closestTurbulenceIndex];
	// return static_cast<u32>(turbPlayer.extremeGear != ExtremeGear::SlideBooster);
}

ASMUsed void Player_SlideBooster(Player *player){
	if(player->extremeGear != ExtremeGear::SlideBooster) { return; }
	if(player->characterArchetype == BoostArchetype){
		player->gearStats[0].boostSpeed = pSpeed(290.0f);
		player->gearStats[1].boostSpeed = pSpeed(340.0f);
		player->gearStats[2].boostSpeed = pSpeed(390.0f);
	}

	if(player->characterArchetype == LateBooster){
		player->gearStats[player->level].boostCost = (player->gearptr->levelStats[player->level].boostCost * 80) / 100;
	}
}