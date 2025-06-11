#include "stardustspeederII.hpp"
#include "cosmetics/player/exloads.hpp"
#include "handlers/player/initgeardata.hpp"

ASMDefined void lbl_Player_BoostEndFunction(Player *);

void Player_StardustSpeederII(Player *player) {
	if(player->extremeGear != ExtremeGear::GunGear) { return; }

	if(player->state == PlayerState::StartLine) { player->currentAir = player->gearStats[1].maxAir; }
	player->level = 1;

	if(player->stardustspeederII_1frameboost) {
		player->changeInAir_loss = 0;
		player->stardustspeederII_1frameboost = FALSE;
		player->gearStats[player->level].boostCost = player->gearptr->levelStats[1].boostCost;
	}

	s32 boostCost;
	if(player->unkABC <= 220) {
		boostCost = 63750;
	} else if(player->unkABC <= 240) {
		boostCost = 52500;
	} else if(player->unkABC <= 280) {
		boostCost = 33750;
	} else {
		boostCost = 22500;
	}
	player->gearStats[player->level].boostCost = boostCost;
}

void Player_SSIIRingBoost(Player *player) {
	if(player->extremeGear != ExtremeGear::GunGear) { return; }

	if (player->input->toggleFaceButtons.hasAny(Buttons::X) &&
	   player->state == PlayerState::Cruise &&
	   player->rings >= 25 &&
	   !player->movementFlags.hasAny(MovementFlags::boosting) &&
	   player->unkB90 <= 0
	   ) {
		//if (player->rings < 25) { return; } // Redundant check, above if already checks ring count

		player->rings -= 25;
		player->gearStats[player->level].boostCost = 0;
		player->gearStats[player->level].boostSpeed = pSpeed(320);
		if(player->characterArchetype == CharacterArchetype::Boost) { player->gearStats[player->level].boostSpeed = pSpeed(320) + BoostArchetypeBoostSpeeds[player->level]; }
		player->stardustspeederII_1frameboost = TRUE;
	} else if(!player->movementFlags.hasAny(MovementFlags::boosting)) {
		player->gearStats[player->level].boostSpeed = pSpeed(245);
		if(player->characterArchetype == CharacterArchetype::Boost) { player->gearStats[player->level].boostSpeed = pSpeed(245) + BoostArchetypeBoostSpeeds[player->level]; }
	}
}