#include "stardustspeederII.hpp"
#include "cosmetics/player/exloads.hpp"
#include "handlers/player/initgeardata.hpp"

global void lbl_Player_BoostEndFunction(Player *);

void Player_StardustSpeederII(Player *player) {
	s32 boostCost;
	//u32 playerTopSpeed = u32(player->gearStats[player->level].topSpeed * SPEED_DIVISOR);
	//u8 controllerPort = player->input->port;
	EnabledEXLoads exLoads{};
	FetchEnabledEXLoadIDs(player, exLoads);

	if (exLoads.gearExLoadID != StardustSpeederEXLoad) return;

	// player->specialFlags |= lowerDecel;
	if (player->state == StartLine) player->currentAir = player->gearStats[1].maxAir;
	player->level = 1;

	if (player->stardustspeederII_1frameboost == TRUE) {
		player->changeInAir_loss = 0;
		player->stardustspeederII_1frameboost = FALSE;
		player->gearStats[player->level].boostCost = player->gearptr->levelStats[1].boostCost;
	}


	// player->gearStats[player->level].driftCost

	// Checks current speed and adjusts boost speed and cost accordingly. Has boost archetype support.

	// if (player->movementFlags & 0x0400) return;

	if (player->unkABC <= 220) {
		boostCost = player->gearptr->levelStats[player->level].boostCost;
	} else if (player->unkABC <= 240) {
		boostCost = 56250;
	} else if (player->unkABC <= 280) {
		boostCost = 37500;
	} else {//if (player->unkABC > 280) {
		boostCost = 18750;
	}
	// else if (player->unkABC >= 280)
	// {
	//     boostCost = 37500;
	// }
	player->gearStats[player->level].boostCost = boostCost;
}

void Player_SSIIRingBoost(Player *player) {
	//s32 newAir;
	//u8 controllerPort = player->input->port;
	EnabledEXLoads exLoads{};
	FetchEnabledEXLoadIDs(player, exLoads);
	if (exLoads.gearExLoadID != StardustSpeederEXLoad) return;

	if ((player->input->toggleFaceButtons & XButton) && (player->state == Cruise) && player->rings >= 25 &&
		!(player->movementFlags & boosting)) {
		if (player->rings < 25) return;
		// if (!(player->currentAir < player->gearStats[player->level].boostCost))
		// {
		player->rings -= 25;
		player->movementFlags & boosting;
		player->gearStats[player->level].boostCost = 0;
		player->gearStats[player->level].boostSpeed = pSpeed(320);
		if (player->characterArchetype == BoostArchetype) 
		{player->gearStats[player->level].boostSpeed = pSpeed(320) + BoostArchetypeBoostSpeeds[player->level];}
		player->stardustspeederII_1frameboost = TRUE;
		// player->boostDuration = -1;
		// }
	} else if (!(player->movementFlags & boosting)) {
		// player->gearStats[player->level].boostCost = player->gearptr->levelStats[player->level].boostCost;
		player->gearStats[player->level].boostSpeed = pSpeed(245);
		if (player->characterArchetype == BoostArchetype) 
		{player->gearStats[player->level].boostSpeed = pSpeed(245) + BoostArchetypeBoostSpeeds[player->level];}
	}
}