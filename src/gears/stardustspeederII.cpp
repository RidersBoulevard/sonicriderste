#include "stardustspeederII.hpp"
#include "cosmetics/player/exloads.hpp"
#include "handlers/player/initgeardata.hpp"

void Player_StardustSpeederII(Player &player) {
	if(player.extremeGear != ExtremeGear::GunGear) { return; }

	if(player.state == PlayerState::StartLine) { player.currentAir = player.gearStats[1].maxAir; }
	player.level = 1;

	if(player.stardustspeederII_1frameboost) {
		player.changeInAir_loss = 0;
		player.stardustspeederII_1frameboost = false;
		player.gearStats[player.level].boostCost = player.gearptr->levelStats[1].boostCost;
	}

	s32 boostCost;
	if(player.unkABC <= 220) {
		boostCost = 60000;
	} else if(player.unkABC <= 240) {
		boostCost = 50000;
	} else if(player.unkABC <= 280) {
		boostCost = 39000;
	} else {
		boostCost = 31000;
	}
	player.gearStats[player.level].boostCost = boostCost;
}

void Player_SSIIRingBoost(Player &player) {
	if(player.extremeGear != ExtremeGear::GunGear) { return; }

	if (player.input->toggleFaceButtons.hasAny(Buttons::X) &&
	   player.state == PlayerState::Cruise &&
	   player.rings >= 25 &&
	   !player.movementFlags.hasAny(MovementFlags::boosting) &&
	   player.unkB90 <= 0
	   && player.unkB08 & 0x2400
	   ) {
		//if (player.rings < 25) { return; } // Redundant check, above if already checks ring count

	    // See customboostduration
		// player.rings -= 25;
		player.gearStats[player.level].boostCost = 0;
		player.gearStats[player.level].boostSpeed = pSpeed(310);
		if(player.characterArchetype == CharacterArchetype::Boost) { player.gearStats[player.level].boostSpeed = pSpeed(310) + BoostArchetypeBoostSpeeds[player.level]; }
		player.stardustspeederII_1frameboost = true;
	} else if(!player.movementFlags.hasAny(MovementFlags::boosting)) {
		player.gearStats[player.level].boostSpeed = pSpeed(245);
		if(player.characterArchetype == CharacterArchetype::Boost) { player.gearStats[player.level].boostSpeed = pSpeed(245) + BoostArchetypeBoostSpeeds[player.level]; }
	}
}