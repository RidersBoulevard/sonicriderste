#include "speedbalancer.hpp"
#include "handlers/player/initgeardata.hpp"
#include "handlers/player/specialflagtweaks.hpp"

USED void Player_SpeedBalancer(Player *player) {
	SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[player->index];
	if (spfInfo->lightBoardEffect == TRUE && player->flags.hasAny(0x20)) {
		spfInfo->lightBoardEffect = FALSE;
		if (player->movementFlags.hasAny(boosting))
		{lbl_Player_BoostEndFunction(player);}
	}
	// if (player->extremeGear != ExtremeGear::SpeedBalancer) { return; }
	// if (player->movementFlags.hasAny(boosting) && player->flags.hasAny(0x20)) {
	// 	player->gearStats[player->level].boostSpeed += pSpeed(1.0f);
	// }
}

USED void Player_SpeedBalancerResetBoostSpeed(Player *player) {
	if (player->extremeGear != ExtremeGear::SpeedBalancer) { return; }
	if (!player->movementFlags.hasAny(boosting) && !player->flags.hasAny(0x20)) {
		player->gearStats[player->level].boostSpeed = player->gearptr->levelStats[player->level].boostSpeed;
		if (player->characterArchetype == BoostArchetype) {
			player->gearStats[player->level].boostSpeed = player->gearptr->levelStats[player->level].boostSpeed + BoostArchetypeBoostSpeeds[player->level];
		}
	}
}