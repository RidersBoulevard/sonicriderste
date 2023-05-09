#include "context.hpp"
#include "slidebooster.hpp"

ASMUsed u32 Player_SlideBoosterCantLeaveTurbulence(Player *player){
	const Player &turbPlayer = players[player->closestTurbulenceIndex];
	u32 canLeaveTurbulence = turbPlayer.extremeGear != SlideBooster;
	return canLeaveTurbulence;
}

ASMUsed void Player_SlideBooster(Player *player){
	if(player->extremeGear != SlideBooster) return;
	if(player->characterArchetype == BoostArchetype){
		player->gearStats[0].boostSpeed = pSpeed(285.0f);
		player->gearStats[1].boostSpeed = pSpeed(325.0f);
		player->gearStats[2].boostSpeed = pSpeed(425.0f);
	}

	if(player->characterArchetype == LateBooster){
		player->gearStats[player->level].boostCost = (player->gearptr->levelStats[player->level].boostCost * 80) / 100;
	}
}