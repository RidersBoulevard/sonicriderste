// 100% match

#include "initgeardata.hpp"

#include <handlers/menu/debugmenu/debugmenu.hpp>

#include "cosmetics/player/exloads.hpp"
#include "gears/berserker.hpp"
#include "gears/blastGaugeGears.hpp"
#include "gears/supers.hpp"
#include "handlers/files/separatemodelloading.hpp"
#include "lib/stdlib.hpp"
#include "mechanics/characters/gizoidreplication.hpp"
#include "mechanics/dash/flyhoopdash.hpp"
#include "mechanics/magneticimpulse.hpp"
#include "riders/gamemode.hpp"
#include "riders/stage.hpp"

inline f32 CustomBoostSpeeds(const Player &player, const f32 &boostSpeed, const u32 &level) {
	f32 newBoostSpeed = boostSpeed;
	if(player.extremeGear == ExtremeGear::LightBoard) {
		// switch(player.characterArchetype) {
		// 	case CharacterArchetype::Combat:
		// 		newBoostSpeed += pSpeed(2);
		// 		break;
		// 	case CharacterArchetype::LateBooster:
		// 	case CharacterArchetype::NoType:
		// 		newBoostSpeed += pSpeed(5);
		// 		break;
		// 	default: break;
		// }
	} else {
		if(player.extremeGear == ExtremeGear::ChaosEmerald) {
			switch(player.character) {
				case Character::Tails:
					newBoostSpeed = pSpeed(200);
					break;
				case Character::Knuckles:
					newBoostSpeed = pSpeed(250);
					break;
				case Character::Shadow:
					newBoostSpeed = pSpeed(285);
					// newBoostSpeed = pSpeed(300);
					break;
				case Character::MetalSonic:
					newBoostSpeed = pSpeed(235); // 245 NEO II
					break;
				default: break;
			}
		}
	}

	if(player.characterArchetype == CharacterArchetype::Boost && player.extremeGear != ExtremeGear::ERider) {
		newBoostSpeed += BoostArchetypeBoostSpeeds[level];
	}
	return newBoostSpeed;
}

inline f32 CustomTopSpeeds(const Player &player, const f32 &topSpeed, const u32 &level) {
	// this top speed is additive, usually adds on top of 162 speed
	if(player.character == Character::Emerl) {
		if (player.gearptr->specialFlags.hasAny(SpecialFlags::ringGear)) {
			return topSpeed + Archetype_TopSpeedBonus[std::to_underlying(player.characterArchetype)];
		} else {
			return topSpeed + EmerlTopSpeedBonus[level];
		}
	}
	if(player.extremeGear == ExtremeGear::ChaosEmerald) {
		switch(player.character) {
			case Character::Tails: return pSpeed(45);
			case Character::Knuckles: return pSpeed(37);
			case Character::Shadow: return pSpeed(18); // experiment III
			// case Character::Shadow: return pSpeed(58.5);
			case Character::MetalSonic: return pSpeed(28); // 43 NEO II
			default: break;
		}
		return topSpeed;// Retains previous behavior. todo: Should this return actually be here?
	}
	return topSpeed + Archetype_TopSpeedBonus[std::to_underlying(player.characterArchetype)];
	// return player.gearptr->topSpeed + Archetype_TopSpeedBonus[std::to_underlying(player.characterArchetype)];
}

inline s32 CustomTornadoCosts(const Player &player, const s32 &tornadoCost) {
	if(player.extremeGear == ExtremeGear::ChaosEmerald) {
		switch(player.character) {
			case Character::Knuckles:
				return 5500;
			case Character::Shadow:
				return 7700;
			case Character::Tails:
				return 20000;
			case Character::MetalSonic:
				return 25000;
			default: break;
		}
	}
	return tornadoCost;
}

inline s32 CustomBoostCosts(const Player &player, const s32 &boostCost) {
	if(player.extremeGear == ExtremeGear::ChaosEmerald) {
		switch(player.character) {
			case Character::Tails:
				return 20000;
			case Character::Knuckles:
				return 11000;
			case Character::Shadow:
				return 33000;
			case Character::MetalSonic:
				return 27500;
			default: break;
		}
	}
	return boostCost;
}

inline s32 CustomPassiveAirDrain(const Player &player, const s32 &airDrain) {
	if(player.extremeGear == ExtremeGear::ChaosEmerald) {
		switch(player.character) {
			case Character::Tails:
			case Character::MetalSonic:
				return 16;
			case Character::Knuckles:
				return 15;
			case Character::Shadow:
				return 0x15;
			default: break;
		}
	}
	return airDrain;
}

inline s32 CustomDriftCosts(const Player &player, const s32 &driftCost) {
	if(player.extremeGear == ExtremeGear::ChaosEmerald) {
		switch(player.character) {
			case Character::Tails:
				return 0xA6;
			case Character::Knuckles:
				return 0xC;
			case Character::MetalSonic:
				return 200;
			default: break;
		}
	}
	return driftCost;
}

inline s32 CustomDriftDashFrames(const Player &player, const s32 &driftFrames) {
	if(player.extremeGear == ExtremeGear::ChaosEmerald) {
		switch(player.character) {
			case Character::Knuckles:
				return 0;
			default: break;
		}
	}
	return driftFrames;
}

inline Flag<SpecialFlags> CustomSpecialFlags(const Player &player, const Flag<SpecialFlags> &specialFlags) {
	Flag<SpecialFlags> newSpecialFlags = specialFlags;
	if(player.extremeGear == ExtremeGear::ChaosEmerald) {
		switch(player.character) {
			case Character::SuperSonic:
				newSpecialFlags |= SpecialFlags::ringGear;
				break;
			case Character::Tails:
				newSpecialFlags |= (SpecialFlags::iceImmunity | SpecialFlags::noSpeedLossChargingJump);
				newSpecialFlags &= ~(SpecialFlags::ringGear | SpecialFlags::noPits | SpecialFlags::thirtyPercentAir);
				break;
			case Character::Shadow:
				newSpecialFlags |= SpecialFlags::stickyFingers;
				newSpecialFlags &= ~SpecialFlags::thirtyPercentAir;
				newSpecialFlags |= SpecialFlags::fiftyPercentAir;
				break;
			case Character::MetalSonic:
				newSpecialFlags &= ~(SpecialFlags::ringGear | SpecialFlags::thirtyPercentAir | SpecialFlags::noPits);
				break;
			default: break;
		}
	}
	return newSpecialFlags;
}

inline f32 CustomAcceleration(const Player &player, const f32 &acceleration) {
	f32 newAcceleration = acceleration;
	if(player.extremeGear == ExtremeGear::ChaosEmerald) {
		switch(player.character) {
			case Character::Knuckles:
				newAcceleration = 0.00617286f;
				break;
			case Character::MetalSonic:
				if (player.gearExload().exLoadID == EXLoad::StardustSpeeder) newAcceleration = 0.000617286f;
			default: break;
		}
	}
	// if (player->characterArchetype == TopSpeedArchetype && player->extremeGear != CoverP
	// 	&& player->extremeGear != HangOn) acceleration += 0.0010;
	// if (player->characterArchetype == Windcatcher && player->extremeGear != CoverP
	// 	&& player->extremeGear != HangOn) acceleration += 0.0010;
	return newAcceleration;
}

inline void GetCustomEXLoadWeight(const EXLoadInfo &exLoad, f32 &weight) {
	const f32 &newWeight = exLoad.weight();
	if(newWeight == -1.0F) { return; }
	weight = newWeight;
}

inline bool isLevel2Gear(const Player &player) {
	return player.extremeGear == ExtremeGear::CoverF ||
	player.extremeGear == ExtremeGear::CoverP ||
	player.extremeGear == ExtremeGear::CoverS ||
	player.extremeGear == ExtremeGear::GunGear;
}

ASMUsed void InitPlayerData(Player &player) {
	player.reset();
}

ASMUsed void InitGearData(Player &player) {
	if (CurrentStage == BabylonGarden || CurrentStage == SkyRoad) {
        // If the turbulence on these stages try to access this, deny it and return
        if (player.index == InGamePlayerCount) {return;}
    }
	const Character &character = *player.characterptr;
	const Gear &gear = *player.gearptr;

	// const TypeStats &typeStats = StatPresets[(player.characterExload().exLoadID == EXLoad::E10R ? 2 : character.statPreset)];
	u8 newStatPreset = character.statPreset;
	switch (player.characterExload().exLoadID) {
		case EXLoad::E10R: // Speed to Power
			newStatPreset = 2;
			break;
		case EXLoad::E10Y: // Speed to Fly
			newStatPreset = 1;
			break;
		default:
			break;
	}
	const TypeStats &typeStats = StatPresets[newStatPreset];
	const f32 &speedMultiplier = character.speedMultiplier;
	const f32 &speedHandlingMultiplier = gear.speedHandlingMultiplier;
	const f32 handlingSpeed = 1.0f + speedMultiplier + speedHandlingMultiplier;
	const f32 handlingPenaltyFromSpeed = 1.0f - speedMultiplier;
	const bool level2Gear = isLevel2Gear(player);

	for(auto level = 0u; level < 3; level++) {
		const auto gearLevel              = level2Gear ? 1 : level; // Level 2 gears init (almost) all stats as level 2
		const auto &currentLevelTypeStats = typeStats.stats[gearLevel];
		const auto &[
			gearMaxAir,
			gearPassiveAirDrain,
			gearDriftingAirCost,
			gearBoostCost,
			gearTornadoCost,
			gearDriftDashSpeed,
			gearBoostSpeed
			] = gear.levelStats[gearLevel];
		auto &[
			baseTopSpeed,
			baseAccel,
			topSpeed,
			tier1Accel,
			tier2Accel,
			tier3Accel,
			offroadSpeedCap,
			maxAir,
			airDrain,
			driftCost,
			boostCost,
			tornadoCost,
			driftDashSpeed,
			boostSpeed
			] = player.gearStats[level];

		const f32 customTopSpeed = CustomTopSpeeds(player, gear.topSpeed, gearLevel);
		baseTopSpeed = (handlingSpeed * currentLevelTypeStats.unk0) + customTopSpeed;
		baseAccel = (handlingSpeed * currentLevelTypeStats.unk4) + customTopSpeed;
		topSpeed = (handlingSpeed * currentLevelTypeStats.unk8) + customTopSpeed;
		const f32 customAcceleration = CustomAcceleration(player, gear.acceleration);
		tier1Accel = (handlingPenaltyFromSpeed * currentLevelTypeStats.unkC) + customAcceleration;
		tier2Accel = (handlingPenaltyFromSpeed * currentLevelTypeStats.unk10) + customAcceleration;
		tier3Accel = (handlingPenaltyFromSpeed * currentLevelTypeStats.unk14) + customAcceleration;
		offroadSpeedCap = (handlingSpeed * currentLevelTypeStats.unk18) + gear.offRoadSpeed;
		driftDashSpeed = gearDriftDashSpeed;
		boostSpeed = CustomBoostSpeeds(player, gearBoostSpeed, gearLevel);
		maxAir = gear.levelStats[level].maxAir;// UI Air bar length is relative based on lvl 1 max air

		// sometimes gf32_SystemHzAdd1 will not be 1.0 in float for some reason? so it will bug passive air drain and drift costs to insane values
		airDrain = CustomPassiveAirDrain(player, gearPassiveAirDrain);
		driftCost = CustomDriftCosts(player, gearDriftingAirCost);
		boostCost = CustomBoostCosts(player, gearBoostCost);
		tornadoCost = CustomTornadoCosts(player, gearTornadoCost);
		// if(character.type == Character::FlyType) {
		// 	currentPlayerGearStats.airDrain = static_cast<s32>(1.3f * static_cast<f32>(currentPlayerGearStats.airDrain));
		// }
		constexpr f32 maxSpeed = pSpeed(447.12f);
		constexpr f32 minSpeed = pSpeed(120.0f);
		baseTopSpeed = clamp(baseTopSpeed, minSpeed, maxSpeed);
		baseAccel = clamp(baseAccel, minSpeed, maxSpeed);
		topSpeed = clamp(topSpeed, minSpeed, maxSpeed);

		tier1Accel = clamp(tier1Accel, pSpeed(1.0f / 3.0f), pSpeed(2.5f));

		constexpr f32 temp_f4 = pSpeed(1.666667);
		tier2Accel = clamp(tier2Accel, pSpeed(0.05), temp_f4);
		tier3Accel = clamp(tier3Accel, pSpeed(0.0233334), temp_f4);
		offroadSpeedCap = clamp(offroadSpeedCap, 10.0f, 200.0f);
	}
	player.unk998 = gear.turningSpeedLoss + (handlingPenaltyFromSpeed * typeStats.unk54);
	player.unk99C = gear.handling + (handlingPenaltyFromSpeed * typeStats.unk58);
	player.unk9A0 = gear.backAxelHandling + (handlingPenaltyFromSpeed * typeStats.unk5C);
	player.unk9A4 = gear.frontAxelHandling + (handlingPenaltyFromSpeed * typeStats.unk60);
	player.unk9A8 = gear.driftRadius + pSpeed(5.654859);
	player.unk9AC = gear.driftRotation + pSpeed(226.1952);
	player.unk9B0 = gear.backAxelDriftRotation + pSpeed(37.69913);
	player.unk9B4 = gear.frontAxelDriftRotation + pSpeed(5.277874);
	player.unk9B8 = gear.unk44 + pSpeed(130.0);
	player.unk9BC = gear.unk48 + pSpeed(-0.1);
	player.unk9C0 = gear.unk4C + pSpeed(447.12);
	player.requiredDriftDashFrames = CustomDriftDashFrames(player, gear.driftDashFrames) + 0x3C;

	if(player.characterExload().exLoadID == EXLoad::E10R) {
		player.typeAttributes = gear.extraTypeAttributes | Type::Power;
	}else if(player.characterExload().exLoadID == EXLoad::E10Y) {
		player.typeAttributes = gear.extraTypeAttributes | Type::Fly;
	} else if(player.isSuperCharacter(Character::MetalSonic)) {
		player.typeAttributes = Type::Speed | Type::Power;
		if(player.gearExload().exLoadID == EXLoad::StardustSpeeder) player.typeAttributes = Type::Speed;
	} else if(player.isSuperCharacter(Character::Tails)) {
		player.typeAttributes = Type::Fly;
	}
	 else {
		const Flag<Type> newType = toGearType(character.type);
		player.typeAttributes = gear.extraTypeAttributes | newType;
	}
	auto weight = CharacterWeights[player.character];
	if(player.hasCharacterExload()) {
		GetCustomEXLoadWeight(player.characterExload(), weight);
	}
	if(player.gearExload().exLoadID == EXLoad::HyperSonic) {
		GetCustomEXLoadWeight(player.gearExload(), weight);
	}
	player.weight = gear.weight + weight;
	player.currentAir = player.gearStats[0].maxAir;
	player.changeInAir_gain = 0;
	player.changeInAir_loss = 0;
	player.unk990 = 0;
	player.trickAirGainMultiplier = 1.0f + gear.trickAirGainMultiplier;
	player.shortcutAirGainMultiplier = 1.0f + gear.shortcutAirGainMultiplier;
	player.QTEAirGainMultiplier = 1.0f + gear.QTEAirGainMultiplier;
	const Flag<SpecialFlags> &specialFlags = player.specialFlags = CustomSpecialFlags(player, gear.specialFlags);
	if(specialFlags.hasAny(SpecialFlags::noTypeShortcuts)) {
		player.typeAttributes = Type::None;
	}
	if(specialFlags.hasAny(SpecialFlags::thirtyPercentAir)) {
		player.currentAir = (player.gearStats[player.level].maxAir * 0x1E) / 100;
	}
	if(specialFlags.hasAny(SpecialFlags::fiftyPercentAir)) {
		player.currentAir = (player.gearStats[player.level].maxAir * 0x32) / 100;
	}
	if(level2Gear) {
		player.currentAir = player.gearStats[1].maxAir;
	}
	constexpr f32 ONE = 1.0f;
	constexpr f32 ZERO = 0.0f;
	player.unk998 = clamp(player.unk998, ZERO, ONE);
	constexpr f32 temp_f0_4 = pSpeed(37.69913);
	player.unk99C = clamp(player.unk99C, ZERO, temp_f0_4);
	player.unk9A0 = clamp(player.unk9A0, ZERO, temp_f0_4);
	player.unk9A4 = clamp(player.unk9A4, ZERO, temp_f0_4);
	if(player.unk9A4 == temp_f0_4) {
		return;
	}

	for(auto &color: SuperNeoMetalHUDColors) {// Should this be here? this is defined in rodata, and already contains this value
		color = 0x1239B8FF;
	}

    Player_BlastGaugeUpdateStatus(player);
}