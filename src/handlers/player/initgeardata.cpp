// 100% match

#include "initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "gears/berserker.hpp"
#include "gears/supers.hpp"
#include "handlers/files/separatemodelloading.hpp"
#include "lib/stdlib.hpp"
#include "mechanics/characters/gizoidreplication.hpp"
#include "mechanics/dash/flyhoopdash.hpp"
#include "mechanics/magneticimpulse.hpp"
#include "riders/gamemode.hpp"

inline f32 CustomBoostSpeeds(const Player &player, const f32 &boostSpeed, const u32 &level) {
	f32 newBoostSpeed = boostSpeed;
	if(player.extremeGear == ExtremeGear::LightBoard) {
		switch(player.characterArchetype) {
			case CombatArchetype:
				newBoostSpeed += pSpeed(2);
				break;
			case LateBooster:
			case NoTypeArchetype:
				newBoostSpeed += pSpeed(5);
				break;
			default: break;
		}
	} else {
		if(player.extremeGear == ExtremeGear::ChaosEmerald) {
			switch(player.character) {
				case Tails:
					newBoostSpeed = pSpeed(200);
					break;
				case Knuckles:
					newBoostSpeed = pSpeed(250);
					break;
				case Shadow:
					newBoostSpeed = pSpeed(300);
					break;
				case MetalSonic:
					newBoostSpeed = pSpeed(235);
					break;
				default: break;
			}
		}
	}

	if(player.characterArchetype == BoostArchetype) {
		newBoostSpeed += BoostArchetypeBoostSpeeds[level];
	}
	return newBoostSpeed;
}

inline f32 CustomTopSpeeds(const Player &player, const f32 &topSpeed, const u32 &level) {
	// this top speed is additive, usually adds on top of 162 speed
	if(player.character == Emerl) {
		return topSpeed + EmerlTopSpeedBonus[level];
	}
	if(player.extremeGear == ExtremeGear::ChaosEmerald) {
		switch(player.character) {
			case Tails: return pSpeed(45);
			case Knuckles: return pSpeed(37);
			case Shadow: return pSpeed(58.5);
			case MetalSonic: return pSpeed(38);
			default: break;
		}
		return topSpeed;// Retains previous behavior. todo: Should this return actually be here?
	}
	return player.gearptr->topSpeed + Archetype_TopSpeedBonus[player.characterArchetype];
}

inline s32 CustomTornadoCosts(const Player &player, const s32 &tornadoCost) {
	if(player.extremeGear == ExtremeGear::ChaosEmerald) {
		switch(player.character) {
			case Tails:
			case Knuckles:
			case Shadow:
				return 5500;
			case MetalSonic:
				return 25000;
			default: break;
		}
	}
	return tornadoCost;
}

inline s32 CustomBoostCosts(const Player &player, const s32 &boostCost) {
	if(player.extremeGear == ExtremeGear::ChaosEmerald) {
		switch(player.character) {
			case Tails:
				return 27500;
			case Knuckles:
				return 11000;
			case Shadow:
				return 33000;
			case MetalSonic:
				return 25000;
			default: break;
		}
	}
	return boostCost;
}

inline s32 CustomPassiveAirDrain(const Player &player, const s32 &airDrain) {
	if(player.extremeGear == ExtremeGear::ChaosEmerald) {
		switch(player.character) {
			case Tails:
			case MetalSonic:
				return 16;
			case Knuckles:
				return 9;
			case Shadow:
				return 0x15;
			default: break;
		}
	}
	return airDrain;
}

inline s32 CustomDriftCosts(const Player &player, const s32 &driftCost) {
	if(player.extremeGear == ExtremeGear::ChaosEmerald) {
		switch(player.character) {
			case Tails:
				return 0xA6;
			case Knuckles:
				return 0xC;
			case MetalSonic:
				return 200;
			default: break;
		}
	}
	return driftCost;
}

inline s32 CustomDriftDashFrames(const Player &player, const s32 &driftFrames) {
	if(player.extremeGear == ExtremeGear::ChaosEmerald) {
		switch(player.character) {
			case Knuckles:
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
			case Tails:
				newSpecialFlags |= (iceImmunity | noSpeedLossChargingJump);
				newSpecialFlags &= ~(ringGear | noPits | thirtyPercentAir);
				break;
			case Shadow:
				newSpecialFlags |= stickyFingers;
				newSpecialFlags &= ~thirtyPercentAir;
				newSpecialFlags |= fiftyPercentAir;
				break;
			case MetalSonic:
				newSpecialFlags &= ~(ringGear | thirtyPercentAir | noPits);
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
			case Knuckles:
				newAcceleration = 0.00617286f;
				break;
			default: break;
		}
	}
	// if (player->characterArchetype == TopSpeedArchetype && player->extremeGear != CoverP
	// 	&& player->extremeGear != HangOn) acceleration += 0.0010;
	// if (player->characterArchetype == Windcatcher && player->extremeGear != CoverP
	// 	&& player->extremeGear != HangOn) acceleration += 0.0010;
	return newAcceleration;
}

inline void GetCustomEXLoadWeight(const u16 &exLoadID, f32 &weight) {
	if(exLoadID == 0) { return; }

	const f32 &newWeight = EXLoadWeights[exLoadID];
	if(newWeight == -1.0F) { return; }
	weight = newWeight;
}

inline bool isLevel2Gear(const Player &player, std::optional<const EnabledEXLoads> exLoads = std::nullopt) {
	const auto &enabledExLoad = exLoads.value_or(FetchEnabledEXLoadIDs(player));
	return player.extremeGear == ExtremeGear::CoverF || player.extremeGear == ExtremeGear::CoverP || player.extremeGear == ExtremeGear::CoverS || (player.extremeGear == ExtremeGear::HighBooster && enabledExLoad.gearExLoadID == StardustSpeederEXLoad);
}

ASMUsed void InitPlayerData(Player &player) {
	// Preserve fields we want to keep
	const auto voiceID = player.characterVoiceID;

	// Copy the index since it gets cleared in the memset
	const auto index = player.index;

	// Clears the player struct for everything not below unkC4
	TRK_memset(&player.unkC4, 0, sizeof(Player) - offsetof(Player, unkC4));
	// Re-Init our extra player data using the default constructors
	MI::impulseData[index] = {};
	PlayerGizoidReplication[index] = {};
	playerFlyHoopDashProperties[index] = {};
	Player_BerserkerStatIncreaseMultipliers[index] = {};
	if(RaceExitMethod != 2) {// on retry, don't clear data
		IsSeparateBoardModelActive[index] = false;
	}

	// Copy our preserved fields back
	player.characterVoiceID = voiceID;
}

ASMUsed void InitGearData(Player &player) {
	const Character &character = *player.characterptr;
	const Gear &gear = *player.gearptr;

	const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(player);
	const TypeStats &typeStats = StatPresets[(exLoads.characterExLoadID == E10REXLoad ? 2 : character.statPreset)];
	const f32 &speedMultiplier = character.speedMultiplier;
	const f32 &speedHandlingMultiplier = gear.speedHandlingMultiplier;
	const f32 handlingSpeed = 1.0f + speedMultiplier + speedHandlingMultiplier;
	const f32 handlingPenaltyFromSpeed = 1.0f - speedMultiplier;
	const bool level2Gear = isLevel2Gear(player, exLoads);

	for(auto level = 0u; level < 3; level++) {
		const auto gearLevel = level2Gear ? 1 : level;// Level 2 gears init (almost) all stats as level 2
		const auto &currentLevelTypeStats = typeStats.stats[gearLevel];
		const auto &currentGearLevelStats = gear.levelStats[gearLevel];
		auto &currentPlayerGearStats = player.gearStats[level];

		const f32 topSpeed = CustomTopSpeeds(player, gear.topSpeed, gearLevel);
		currentPlayerGearStats.baseTopSpeed = (handlingSpeed * currentLevelTypeStats.unk0) + topSpeed;
		currentPlayerGearStats.baseAccel = (handlingSpeed * currentLevelTypeStats.unk4) + topSpeed;
		currentPlayerGearStats.topSpeed = (handlingSpeed * currentLevelTypeStats.unk8) + topSpeed;
		const f32 acceleration = CustomAcceleration(player, gear.acceleration);
		currentPlayerGearStats.tier1Accel = (handlingPenaltyFromSpeed * currentLevelTypeStats.unkC) + acceleration;
		currentPlayerGearStats.tier2Accel = (handlingPenaltyFromSpeed * currentLevelTypeStats.unk10) + acceleration;
		currentPlayerGearStats.tier3Accel = (handlingPenaltyFromSpeed * currentLevelTypeStats.unk14) + acceleration;
		currentPlayerGearStats.offroadSpeedCap = (handlingSpeed * currentLevelTypeStats.unk18) + gear.offRoadSpeed;
		currentPlayerGearStats.driftDashSpeed = currentGearLevelStats.driftDashSpeed;
		currentPlayerGearStats.boostSpeed = CustomBoostSpeeds(player, currentGearLevelStats.boostSpeed, gearLevel);
		currentPlayerGearStats.maxAir = gear.levelStats[level].maxAir;// UI Air bar length is relative based on lvl 1 max air

		// sometimes gf32_SystemHzAdd1 will not be 1.0 in float for some reason? so it will bug passive air drain and drift costs to insane values
		currentPlayerGearStats.airDrain = CustomPassiveAirDrain(player, currentGearLevelStats.passiveAirDrain);
		currentPlayerGearStats.driftCost = CustomDriftCosts(player, currentGearLevelStats.driftingAirCost);
		currentPlayerGearStats.boostCost = CustomBoostCosts(player, currentGearLevelStats.boostCost);
		currentPlayerGearStats.tornadoCost = CustomTornadoCosts(player, currentGearLevelStats.tornadoCost);
		// if(character.type == Character::FlyType) {
		// 	currentPlayerGearStats.airDrain = static_cast<s32>(1.3f * static_cast<f32>(currentPlayerGearStats.airDrain));
		// }
		constexpr f32 maxSpeed = pSpeed(447.12f);
		constexpr f32 minSpeed = pSpeed(120.0f);
		currentPlayerGearStats.baseTopSpeed = clamp(currentPlayerGearStats.baseTopSpeed, minSpeed, maxSpeed);
		currentPlayerGearStats.baseAccel = clamp(currentPlayerGearStats.baseAccel, minSpeed, maxSpeed);
		currentPlayerGearStats.topSpeed = clamp(currentPlayerGearStats.topSpeed, minSpeed, maxSpeed);

		currentPlayerGearStats.tier1Accel = clamp(currentPlayerGearStats.tier1Accel, pSpeed(1.0f / 3.0f), pSpeed(2.5f));

		constexpr f32 temp_f4 = pSpeed(1.666667);
		currentPlayerGearStats.tier2Accel = clamp(currentPlayerGearStats.tier2Accel, pSpeed(0.05), temp_f4);
		currentPlayerGearStats.tier3Accel = clamp(currentPlayerGearStats.tier3Accel, pSpeed(0.0233334), temp_f4);
		currentPlayerGearStats.offroadSpeedCap = clamp(currentPlayerGearStats.offroadSpeedCap, 10.0f, 200.0f);
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

	if(exLoads.characterExLoadID == E10REXLoad) {
		player.typeAttributes = gear.extraTypeAttributes | Type::Power;
	} else if(isSuperCharacter(player, MetalSonic)) {
		player.typeAttributes = Type::Speed | Type::Power;
	} else {
		const Flag<Type> newType = static_cast<Type>(1 << character.type);
		player.typeAttributes = gear.extraTypeAttributes | newType;
	}
	auto weight = CharacterWeights[player.character];
	if(exLoads.isExloadEnabled()) {
		GetCustomEXLoadWeight(exLoads.characterExLoadID, weight);
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
	if(specialFlags.hasAny(noTypeShortcuts)) {
		player.typeAttributes = Type::None;
	}
	if(specialFlags.hasAny(thirtyPercentAir)) {
		player.currentAir = (player.gearStats[player.level].maxAir * 0x1E) / 100;
	}
	if(specialFlags.hasAny(fiftyPercentAir)) {
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
}