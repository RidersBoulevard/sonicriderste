#include "ringgearlevels.hpp"

#include <handlers/player/initgeardata.hpp>

#include "cosmetics/player/exloads.hpp"

constexpr u8 extraLap = 3;

void Player_RingGearLevels(Player &player) {
	if(!player.specialFlags.hasAny(SpecialFlags::ringGear) || player.extremeGear == ExtremeGear::HangOn || player.extremeGear == ExtremeGear::ChaosEmerald) { return; }
    f32 boostArchBonus = player.characterArchetype == CharacterArchetype::Boost ? BoostArchetypeBoostSpeeds[player.level] : 0.0f;
	switch(player.currentLap) {
		case 0:
		case 1:
			player.level = 0;
			break;

		case 2:
			player.level = 1;
			break;

		case 3:
		default:
			player.level = 2;
			break;
	}

	switch(player.extremeGear) {
		using namespace ExtremeGear;
		case AdvantageF:
			if(player.gearExload().exLoadID == EXLoad::TheProfessional) { return; }
			player.gearStats[player.level].boostCost = (player.gearStats[player.level].maxAir * 5) / 100;
			player.gearStats[player.level].topSpeed = player.gearStats[0].topSpeed;
			player.gearStats[player.level].airDrain = 0x9;// 0.6 rings.sec
			player.gearStats[player.level].driftCost = (player.characterArchetype == CharacterArchetype::Drift) ? 29 : 32;// 0.21 rings/sec
			player.gearStats[player.level].tornadoCost = (player.gearStats[player.level].maxAir * 5) / 100;
			player.gearStats[player.level].maxAir = 100000;
			player.gearStats[player.level].driftDashSpeed = player.gearStats[2].driftDashSpeed;
			break;

		case TheCrazy:
			player.gearStats[player.level].topSpeed = player.gearStats[0].topSpeed;
			// player.gearStats[player.level].offroadSpeedCap = player.gearStats[0].offroadSpeedCap;
			player.gearStats[player.level].maxAir = player.gearStats[0].maxAir;
			player.gearStats[player.level].airDrain = player.gearStats[0].airDrain;
			player.gearStats[player.level].driftCost = 0;
			player.gearStats[player.level].tornadoCost = player.gearStats[0].tornadoCost;
			player.gearStats[player.level].driftDashSpeed = player.gearStats[0].driftDashSpeed;
			break;

		case Accelerator:
			// player.gearStats[player.level].baseTopSpeed = player.gearStats[0].baseTopSpeed;
			// player.gearStats[player.level].baseAccel = player.gearStats[0].baseAccel;
			player.gearStats[player.level].topSpeed = player.gearStats[0].topSpeed;
			// player.gearStats[player.level].tier1Accel = player.gearStats[0].tier1Accel;
			// player.gearStats[player.level].tier2Accel = player.gearStats[0].tier2Accel;
			// player.gearStats[player.level].tier3Accel = player.gearStats[0].tier3Accel;
			// player.gearStats[player.level].offroadSpeedCap = player.gearStats[0].offroadSpeedCap;
			player.gearStats[player.level].maxAir = player.gearStats[0].maxAir;
	        player.gearptr->jumpChargeCostMultiplier = (player.gearExload().exLoadID == EXLoad::HyperHangOn) ? 0.0f : 3.0f;
			player.gearStats[player.level].airDrain = player.gearStats[0].airDrain;
	        if (player.gearExload().exLoadID != EXLoad::HyperHangOn) {
	            player.gearStats[player.level].driftCost = (player.characterArchetype == CharacterArchetype::Drift) ? (player.gearptr->levelStats[0].driftingAirCost * 90) / 100 : player.gearStats[0].driftCost;
	        }
			player.gearStats[player.level].boostCost = player.gearStats[0].boostCost;
	        player.gearStats[player.level].boostSpeed = (player.gearExload().exLoadID != EXLoad::HyperHangOn) ? pSpeed(450.2f) + boostArchBonus : player.gearStats[player.level].boostSpeed;
			player.gearStats[player.level].tornadoCost = player.gearStats[0].tornadoCost;
			player.gearStats[player.level].driftDashSpeed = player.gearStats[0].driftDashSpeed;
			break;

		default:
			player.gearStats[player.level] = player.gearStats[0];
			break;
	}
}