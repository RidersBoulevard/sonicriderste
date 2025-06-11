#include "advantagef.hpp"
#include "cosmetics/player/exloads.hpp"
#include "handlers/player/initgeardata.hpp"

constexpr std::array AdvantageF_BoostSpeeds = {pSpeed(200.0f), pSpeed(210.0f), pSpeed(220.0f), pSpeed(230.0f),
                                               pSpeed(240.0f), pSpeed(250.0f), pSpeed(255.0f)};

void Player_AdvantageF(Player &player) {
	// if(player.movementFlags.hasAny(0x0400)) { return; }
	// Checks current speed and adjusts boost speed and cost accordingly. Has boost archetype support.
	if(player.extremeGear != ExtremeGear::AdvantageF || player.gearExload().exLoadID == EXLoad::TheProfessional) {
		return;
	}
	// player.specialFlags |= (SpecialFlags::ringGear | SpecialFlags::noSpeedLossChargingJump | SpecialFlags::iceImmunity);

	f32 boostSpeed;
	if(player.rings <= 9) {
		boostSpeed = AdvantageF_BoostSpeeds[0];
	} else if(player.rings <= 19) {
		boostSpeed = AdvantageF_BoostSpeeds[1];
	} else if(player.rings <= 39) {
		boostSpeed = AdvantageF_BoostSpeeds[2];
	} else if(player.rings <= 59) {
		boostSpeed = AdvantageF_BoostSpeeds[3];
	} else if(player.rings <= 79) {
		boostSpeed = AdvantageF_BoostSpeeds[4];
	} else if(player.rings <= 89) {
		boostSpeed = AdvantageF_BoostSpeeds[5];
	} else {//if (player.rings <= 100)
		boostSpeed = AdvantageF_BoostSpeeds[6];
	}
	if(player.characterArchetype == CharacterArchetype::Boost) {
		boostSpeed += BoostArchetypeBoostSpeeds[player.level];
	}

	player.gearptr->levelStats[player.level].boostSpeed = boostSpeed;
	if (player.movementFlags.hasAny(MovementFlags::boosting)) { return; }

	player.gearStats[player.level].boostSpeed = boostSpeed;
}

// void Player_TheProfessional(Player &player) {
// 	if(player.gearExload().exLoadID != EXLoad::TheProfessional) { return; }
//
// 	player.specialFlags &= ~SpecialFlags::fiftyPercentAir;
// 	player.typeAttributes = Type::None;
// 	if(player.state == PlayerState::StartLine) {
// 		player.currentAir = player.gearStats[player.level].maxAir;
// 	}
// 	player.specialFlags |= (SpecialFlags::alwaysOnIce | SpecialFlags::noSpeedLossChargingJump | SpecialFlags::legendEffect);
// 	player.gearStats[0].airDrain = 0x00000010;
// 	player.gearStats[1].airDrain = 0x00000010;
// 	player.gearStats[2].airDrain = 0x00000010;
// 	player.gearStats[0].boostCost = 0x000061A8;
// 	player.gearStats[1].boostCost = 0x00007530;
// 	player.gearStats[2].boostCost = 0x00009C40;
// 	player.gearStats[0].tornadoCost = 0x000061A8;
// 	player.gearStats[1].tornadoCost = 0x00007530;
// 	player.gearStats[2].tornadoCost = 0x00009C40;
// 	player.gearStats[0].driftCost = 0x000000A6;
// 	player.gearStats[1].driftCost = 0x000000FA;
// 	player.gearStats[2].driftCost = 0x0000014D;
// 	player.gearStats[0].boostSpeed = player.gearStats[0].topSpeed;
// 	player.gearStats[1].boostSpeed = pSpeed(230.0f);
// 	player.gearStats[2].boostSpeed = pSpeed(250.0f);
// 	// player.gearStats[player.level].offroadSpeedCap = (f32)0xC1A00000; // speed balancer effect
// 	// player.gearStats[0].baseTopSpeed = pSpeed(162.0f);
// 	// player.gearStats[0].topSpeed = pSpeed(162.0f);
// }