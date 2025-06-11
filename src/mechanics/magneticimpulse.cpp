#include "mechanics/magneticimpulse.hpp"
#include "cosmetics/player/exloads.hpp"
#include "gears/blastGaugeGears.hpp"
#include "gears/hangon.hpp"
#include "gears/hypersonic.hpp"
#include "gears/supermetal.hpp"
#include "handlers/menu/debugmenu/debugmenu.hpp"
#include "handlers/player/voicelines.hpp"
#include "lib/lib.hpp"
#include "lib/stdlib.hpp"
#include "mechanics/dash/flyhoopdash.hpp"
#include "mechanics/dash/grindraildash.hpp"
#include "riders/gamemode.hpp"
#include "riders/stage.hpp"
#include <ppu_intrinsics.h>
//#include "gears/hyperhangon.hpp"
#include "lib/animation.hpp"
#include "archetype/afterburner.hpp"
#include "handlers/player/specialflagtweaks.hpp"

struct LevelUpStarsObject1;
struct Particle_LevelUpStars;

namespace MagneticImpulse {
	std::array<MI::ImpulseData, MaxPlayerCount> impulseData;
	std::array<Matrix3x4F, MaxPlayerCount> PlayerPositionRotationMatrices;

	void playMaxMILine(const Player&player);
	void setMaxMIAnimation(Player &player);
	void levelUpStars(const Player&player);

	constexpr auto VoiceLinesPerChar = 3;
	constexpr m2darray<u8, std::to_underlying(Character::Total), VoiceLinesPerChar> VoiceLineIDs = {{
			{0x02}, // Sonic
			{0x02}, // Tails
			{0x02}, // Knuckles
			{0x02}, // Amy
			{0x02}, // Jet
			{0x02}, // Storm
			{0x02}, // Wave
			{0x02}, // Eggman
			{0x02}, // Cream
			{0x02}, // Rouge
			{0x02}, // Shadow
			{0x02}, // SuperSonic
			{0x02, 0x03, 19}, // Nights
			{0x02}, // Aiai
			{0x02}, // Ulala
			{}, // E10G
			{}, // E10R
			{0x08, 0x09, 0x0A}, // Silver
			{0x07, 0x08, 0x09}, // MetalSonic
			{0x02, 0x07}, // Emerl
			{0x08, 0x09}, // Blaze
			{0x07}, // Chaos
			{8, 9, 10} // Tikal
	}};
	constexpr m2darray<u8, EXLoadCount, VoiceLinesPerChar> ExLoadVoiceLineIDs = {{
			{},
			{}, // Perfect Nazo
			{}, // E10R
			{}, // Dark Sonic
			{12}, // Hyper Sonic
			{}, // Stardust Speeder
			{}, // Hyper HangOn
			{}, // The Professional
			{}, // Ollie King Gear
			{}, // Windmaster Jet
			{}, // Neo Metal
			{}, // E99
			{}, // The Beast
			{}, // E10B
			{8}, // Miku
			{}, // Metal Sonic Scarf
			{0x02, 0x03, 19}, // Reala
			{8}, // Gongon
	}};
} // namespace MagneticImpulse

void MagneticImpulse::playMaxMILine(const Player&player){
	const u8&character = player.character;
	const auto ID = player.hasCharacterExload() ? player.characterExload().id() : player.gearExload().id();
	const std::span MIVoiceLines = !player.hasExload() ? MI::VoiceLineIDs[character] : MI::ExLoadVoiceLineIDs[ID];

	u32 usedSlots = 0;
	while (usedSlots < MI::VoiceLinesPerChar) {
		// Get how many voicelines this character actually has
		if (MIVoiceLines[usedSlots] == 0) { break; }
		usedSlots++;
	}
	if (usedSlots == 0) { return; } // Character has no lines to play

	const u32 selectedLine = lbl_RNG_Number(usedSlots);
	Sound::PlaySound(player, MIVoiceLines[selectedLine]);
}

void MagneticImpulse::setMaxMIAnimation(Player &player) {
	constexpr auto TORNADO_CHEER_ANIMATIONID = 0x3B;
	constexpr auto RAIL_GRINDING_ANIMATIONID = 0x1B;

	if (player.state == PlayerState::Cruise ||
			player.state == PlayerState::TurbulenceRide ||
			player.state == PlayerState::Fly) {
		func_ClearPlayerMotionTable(player);
		func__RegistPlayerMotionTable(player, 1, 0, 10.0f, 0.0f);
		func__RegistPlayerMotionTable(player, TORNADO_CHEER_ANIMATIONID, 8, 1.0f, 0.0f);
		func_StartPlayerMotionTable(player);
	} else if (player.state == PlayerState::RailGrind) {
		func_ClearPlayerMotionTable(player);
		func__RegistPlayerMotionTable(player, RAIL_GRINDING_ANIMATIONID, 0, 10.0f, 0.0f);
		func__RegistPlayerMotionTable(player, TORNADO_CHEER_ANIMATIONID, 8, 1.0f, 0.0f);
		func_StartPlayerMotionTable(player);
	}
}

struct LevelUpStarsObject1{
	/* 0x0 */ u32 unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 unk8;
	/* 0xC */ u32 unkC;
	/* 0x10 */ u32 unk10;
	/* 0x14 */ u32 unk14;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ u32 unk1C;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ u32 unk28;
	/* 0x2C */ u32 unk2C;
	/* 0x30 */ u32 unk30;
	/* 0x34 */ u32 unk34;
	/* 0x38 */ u32 unk38;
	/* 0x3C */ u32 unk3C;
	/* 0x40 */ u32 unk40;
	/* 0x44 */ u32 unk44;
	/* 0x48 */ const void *unk48;
	/* 0x4C */ u32 unk4C;
	/* 0x50 */ u32 unk50;
	/* 0x54 */ u32 unk54;
	/* 0x58 */ u32 unk58;
	/* 0x5C */ u32 unk5C;
	/* 0x60 */ void *unk60;
	/* 0x64 */ u32 unk64;
	/* 0x68 */ void *unk68;
	/* 0x6C */ u32 unk6C;
	/* 0x70 */ u8 unk70;
	/* 0x71 */ u8 unk71;
	/* 0x72 */ u8 unk72;
	/* 0x73 */ u8 unk73;
	/* 0x74 */ u32 unk74;
	/* 0x78 */ u32 unk78;
	/* 0x7C */ u32 unk7C;
};

struct Particle_LevelUpStars {
	/* 0x0 */ u32 unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 unk8;
	/* 0xC */ u32 unkC;
	/* 0x10 */ u32 unk10;
	/* 0x14 */ u32 unk14;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ u32 unk1C;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ u32 unk28;
	/* 0x2C */ u32 unk2C;
	/* 0x30 */ u32 unk30;
	/* 0x34 */ u32 unk34;
	/* 0x38 */ u32 unk38;
	/* 0x3C */ u32 unk3C;
	/* 0x40 */ u32 unk40;
	/* 0x44 */ u32 unk44;
	/* 0x48 */ u32 unk48;
	/* 0x4C */ u32 unk4C;
	/* 0x50 */ u32 unk50;
	/* 0x54 */ u32 unk54;
	/* 0x58 */ u32 unk58;
	/* 0x5C */ u32 unk5C;
	/* 0x60 */ u32 unk60;
	/* 0x64 */ u32 unk64;
	/* 0x68 */ u32 unk68;
	/* 0x6C */ u32 unk6C;
	/* 0x70 */ u32 unk70;
	/* 0x74 */ u32 unk74;
	/* 0x78 */ u32 unk78;
	/* 0x7C */ u32 unk7C;
	/* 0x80 */ u32 unk80;
	/* 0x84 */ u32 unk84;
	/* 0x88 */ u32 unk88;
	/* 0x8C */ u32 unk8C;
	/* 0x90 */ u32 unk90;
	/* 0x94 */ u32 unk94;
	/* 0x98 */ u32 unk98;
	/* 0x9C */ u32 unk9C;
	/* 0xA0 */ u32 unkA0;
};

extern Particle_LevelUpStars Particle_LevelUpStars;

void MagneticImpulse::levelUpStars(const Player &player) {
	auto *particles = reinterpret_cast<LevelUpStarsObject1 *>(SetTask(func_Particle_Task, ObjectGroups::Particle, Object1Sizes::x80)->object);
	particles->unk72 = player.index;
	particles->unk0 = Particle_LevelUpStars.unk74;
	particles->unk4 = Particle_LevelUpStars.unk78;
	particles->unk8 = Particle_LevelUpStars.unk7C;
	particles->unk30 = Particle_LevelUpStars.unk98;
	particles->unk34 = Particle_LevelUpStars.unk9C;
	particles->unk38 = Particle_LevelUpStars.unkA0;
	particles->unk3C = Particle_LevelUpStars.unk8C;
	particles->unk40 = Particle_LevelUpStars.unk90;
	particles->unk44 = Particle_LevelUpStars.unk94;
	particles->unk10 = Particle_LevelUpStars.unk80;
	particles->unk14 = Particle_LevelUpStars.unk84;
	particles->unk18 = Particle_LevelUpStars.unk88;
	particles->unk48 = &player.x;
	particles->unk60 = &Particle_LevelUpStars;
	particles->unk68 = gpsTexList_Particle;
	particles->unk6C = 0;
	particles->unk73 = static_cast<u8>(-1);
	particles->unk74 = 0;
}

f32 MagneticImpulse::calculateMultiplier(const Player &player, f32 value){
	switch(player.getTypeCount()) {
		case 2:
			value *= DualTypeMultiplier;
			break;
		case 3:
			value *= AllTypeMultiplier;
			break;
		default:
			break;
	}

	return value;
}

ASMUsed f32 MagneticImpulse_ParticleSpeed(const Player &player) {
	return 0.0505316F + (player.magneticImpulse_timer * 0.00012632867F);
}

ASMUsed void MagneticImpulse_AttackParticles(const Player &player, u8 battleModeData[], u32 offset){
	MI::ImpulseData* magneto = &MI::impulseData[player.index];
	if (CurrentGameMode == FreeRace || CurrentGameMode == WorldGrandPrix || CurrentGameMode == StoryMode) {
		u8 particleState;
		if(!magneto->magneticImpulse) {
			particleState = 1;
		} else if(player.magneticImpulse_timer < MI::MaximumCap) {
			particleState = 2;
		} else {
			particleState = 3;
		}

		battleModeData[offset] = particleState;
	}
}

ASMUsed void MagneticImpulse_GuardParticles(const Player &player, u8 battleModeData[], u32 offset){
	MI::ImpulseData* magneto = &MI::impulseData[player.index];
	if (CurrentGameMode == FreeRace || CurrentGameMode == WorldGrandPrix || CurrentGameMode == StoryMode) {
		u8 particleState;
		if (!magneto->magneticImpulse) {
			if (player.magneticImpulse_timer == 0) {
				particleState = 0;
			} else {
				particleState = 1;
			}
		} else {
			if (player.magneticImpulse_timer < MI::MaximumCap) {
				particleState = 0;
			} else {
				particleState = 2;
			}
		}

		battleModeData[offset] = particleState;
	}
}

ASMUsed f32 Player_MISpeedAirGain(Player &player, f32 airGain){
	if(CurrentStage == SegaCarnival && (player.railID == 3 || player.railID == 4)) airGain *= 0.5f;
	if(CurrentStage == DarkDesert && player.railID == 0) airGain *= 0.75f;
	if (player.extremeGear == ExtremeGear::CoverS && player.state == PlayerState::RailGrind) {
		if (player.characterptr->type == Character::Type::Speed) {
			airGain *= 0.5f;
		}
		else if ((player.characterptr->type == Character::Type::Fly
				  || player.characterptr->type == Character::Type::Power
				  || player.characterptr->type == Character::Type::None)) {
			airGain *= 0.30f;
		}
	}

	if (player.characterArchetype == CharacterArchetype::Mechanic) airGain *= 1.15f;

	if (MI::impulseData[player.index].magneticImpulse) {
		airGain += player.magneticImpulse_timer / 3.0f;
	}

	// for grind rails
	if (player.grindRailDash == 1) {
		if (player.specialFlags.hasAny(SpecialFlags::ringGear)) {
			auto &[currentGauge] = PlayerBlastGaugeInfo[player.index];
			if (player.character == Character::SuperSonic
				&& currentGauge > 0
				&& player.gearExload().exLoadID != EXLoad::HyperSonic
				) {
				const s32 newBlastGaugeValue = currentGauge + static_cast<s32>(GrindRailDashAirCosts[2] * 1.6);
				currentGauge = clamp(newBlastGaugeValue);
			} else {
				s32 newAir;
				if (player.characterArchetype == CharacterArchetype::Mechanic) {
					newAir = static_cast<s32>(player.currentAir - (player.gearStats[player.level].maxAir * 0.02) / 100);
				} else {
					newAir = static_cast<s32>(player.currentAir - (player.gearStats[player.level].maxAir * 0.05) / 100);
				}
				player.currentAir = clamp(newAir);
			}
		}
		else {
			if (player.characterArchetype == CharacterArchetype::Mechanic) {
				airGain = GrindRailDashAirCostsMechanic[player.level];
			} else {
				airGain = GrindRailDashAirCosts[player.level];
			}
		}
	}
	if (player.gearExload().exLoadID == EXLoad::TheBeast) { return 0; }
	return airGain;
}

ASMUsed f32 Player_MIFlyAirGain(Player &player, f32 airGain){
	// for fly hoops
	if (player.extremeGear == ExtremeGear::CoverS && player.state == PlayerState::Fly) {
		if (player.characterptr->type == Character::Type::Fly) {
			airGain *= 0.5f;
		} else if ((player.characterptr->type == Character::Type::Power
				  || player.characterptr->type == Character::Type::Speed
				  || player.characterptr->type == Character::Type::None)) {
			airGain *= 0.30f;
		}
	}

	if (player.characterArchetype == CharacterArchetype::Mechanic) { airGain *= 1.10f; }

	if (MI::impulseData[player.index].magneticImpulse) {
		airGain += player.magneticImpulse_timer / 3.0f;
	}

	// This involves the new hoop dash V2.
	PlayerFlyHoopDashProperties* properties = &playerFlyHoopDashProperties[player.index];
	BlastGaugeInfo* bgInfo = &PlayerBlastGaugeInfo[player.index];
	if (properties->hasUsed) {
		// This first if-statement is for ring gears
		if (player.specialFlags.hasAny(SpecialFlags::ringGear)) {
			if (player.character == Character::SuperSonic
				&& bgInfo->currentGauge > 0
				&& player.gearExload().exLoadID != EXLoad::HyperSonic
				) {
				// See the air section below, same logic
				bgInfo->currentGauge -= FlyHoopDashInitialCosts[player.level];
				if (bgInfo->currentGauge < 0) { bgInfo->currentGauge = 0; }
			} else {
				// This statement removes rings from ring gears. Change the "5" to change the
				// amount of rings taken
				s32 newAir;
				if (player.characterArchetype == CharacterArchetype::Mechanic) {
					newAir = player.currentAir - (player.gearStats[player.level].maxAir * 3) / 100;
				} else {
					newAir = player.currentAir - (player.gearStats[player.level].maxAir * 5) / 100;
				}
				player.currentAir = clamp(newAir);
			}
		} else {
			// This statement removes air from the player. Change the costs in "FlyHoopDashInitialCosts[player.level]"
			// (in flyhoopdash.cpp) to alter how much air is taken
			s32 newAir;
			if (player.characterArchetype == CharacterArchetype::Mechanic) {
				newAir = player.currentAir - FlyHoopDashInitialCostsMechanic[player.level];
			}
			else {
				newAir = player.currentAir - FlyHoopDashInitialCosts[player.level];
			}
			player.currentAir = clamp(newAir);
		}
		// properties->duration = 0;
		properties->hasUsed = false;
	}

	// Handles the delay for new fly hoop dash. Subtracts 1 frame from the timer every frame and pauses air gain.
	if (properties->gainDelay > 0.0f) {
		properties->gainDelay -= 1.0f;
		return 0;
	}

	if (player.flyHoopDash == 1) {
		if (player.specialFlags.hasAny(SpecialFlags::ringGear)) {
			if (player.character == Character::SuperSonic
				&& bgInfo->currentGauge > 0
				&& player.gearExload().exLoadID != EXLoad::HyperSonic
				) {
				f32 newBlastGaugeValue = bgInfo->currentGauge + static_cast<s32>(FlyHoopDashAirLoss[1]);
				// player.shortcutAirGainMultiplier -= FlyHoopDashAirLoss[player.level];
				if (newBlastGaugeValue < 0) { newBlastGaugeValue = 0; }
				bgInfo->currentGauge = newBlastGaugeValue;
			} else {
				const s32 newAir = player.currentAir - (player.gearStats[player.level].maxAir * 0.05) / 100;
				player.currentAir = clamp(newAir);
			}
		} else {
			airGain = FlyHoopDashAirLoss[player.level];
		}
	}
	if (player.gearExload().exLoadID == EXLoad::TheBeast) { return 0; }
	return airGain;
}

ASMUsed f32 Player_MIPowerObjectAirGain(const Player &player, f32 airGain){
	// for power objects
	if (player.extremeGear == ExtremeGear::CoverS) {
		if (player.characterptr->type == Character::Type::Power) {
			airGain *= 0.5F;
		} else if ((player.characterptr->type == Character::Type::Speed
				  || player.characterptr->type == Character::Type::Fly
				  || player.characterptr->type == Character::Type::None)) {
			airGain *= 0.30F;
		}
	}

	if (player.characterArchetype == CharacterArchetype::Mechanic) airGain *= 1.20f;

	if (MI::impulseData[player.index].magneticImpulse) {
		airGain += player.magneticImpulse_timer * 5.0f;
	}
	if (player.gearExload().exLoadID == EXLoad::TheBeast) { return 0; }
	return airGain;
}

ASMUsed f32 Player_MITrickAirGain(const Player &player, f32 airGain){
	// HHOInfo *hhoInfo = &PlayerHHOInfo[player.index];
	BlastGaugeInfo* bgInfo = &PlayerBlastGaugeInfo[player.index];

	if (player.isBlastGaugeGear) {
		// for all blast gear gauges, use this when they land their tricks
		const HyperSonicInfo &hsInfo = PlayerHyperSonicInfo[player.index];
		if (!(player.isSuperCharacter(Character::Shadow)
			|| player.isSuperCharacter(Character::Tails)
			|| (player.extremeGear == ExtremeGear::Accelerator && player.gearExload().exLoadID == EXLoad::HyperHangOn))) {
			if (player.gearExload().exLoadID == EXLoad::HyperSonic) bgInfo->currentGauge += player.trickCount * 8000; // 30 tricks to fill
			else bgInfo->currentGauge += player.trickCount * 10000; // 20 tricks to fill
		}
		if (bgInfo->currentGauge >= 200000) {
			    bgInfo->currentGauge = 200000;
		    }
	}

	// if (exLoads.gearExLoadID == HyperHangOn && hhoInfo->saturnMegadriveStatus != 2)
	// {
	//     hhoInfo->trickCount += 1;
	//     PlayAudioFromDAT(Sound::SFX::TornadoSlingshot);
	// }

	// if (exLoads.gearExLoadID == TheBeast && !(player.specialFlags & ringGear))
	// {
	//     player.rings += 10;
	//     return 0;
	// }

    // NOTE: Until we extern trick landing speeds & air gain (lbl_001C670C, rodata.s), this is our override.
    if (DebugMenu_CheckOption(DebugMenuOptions::PTRMode)) {
        // Base air gain amounts calc'ed first
    	switch(player.trickRank) {
    	    case SRank:
    	        // S rank starts with 40 air in PTR mode
    	            airGain = 40000.0f * (1.0f+player.gearptr->trickAirGainMultiplier);
    	        break;
    		case XRank:
    		    // X rank starts with 80 air in PTR mode
    			airGain = 85000.0f * (1.0f+player.gearptr->trickAirGainMultiplier);
    			break;
    		default:
    			break;
    	}
        // Then, check for overtricking and give air per trick
        if (player.trickRank > SPlusRank) {
            // S and C rank
            airGain += static_cast<f32>(player.trickCount - static_cast<u8>(4)) * 5000.0f; // 5 air per trick
        }
    }

	if (MI::impulseData[player.index].magneticImpulse) {
		const f32 MIpercentage = player.magneticImpulse_timer / MI::MaximumCap;
		f32 MIair = MIpercentage * 50000.0f;
	    // NOTE: these two can be removed tbh (now handled in Player_TrickPayout)
		if (player.extremeGear == ExtremeGear::TheCrazy) {
			MIair *= 0.2f;
		} else if (player.extremeGear == ExtremeGear::AdvantageF) {
			MIair *= 0.5f;
		}
		if (!player.typeAttributes.hasAny(Type::All)) { MIair *= MI::NoTypeMultiplier; }
		if (MIair > 0.0f && player.extremeGear == ExtremeGear::SuperHangOn) MIair = MIpercentage * 20000.0f;
		airGain += MIair;
	}
	return airGain;
}

ASMUsed void Player_TrickPayout(Player &player, s32 payoutAmount) {
	if (!player.specialFlags.hasAny(SpecialFlags::ringGear)) return;

    // We don't have a flag for this yet so... yeah here's the check.
    bool hasTrickPayout = player.extremeGear == ExtremeGear::TheCrazy
    || player.extremeGear == ExtremeGear::AdvantageF;

    if (!hasTrickPayout) return;

    // For each gear with trick payout, these tables determine the ring amount to give per trick rank

    // The Crazy's trick payout table
    if (player.extremeGear == ExtremeGear::TheCrazy) {
        switch (player.trickRank) {
            case CRank: payoutAmount = 2; break;
            case BRank: payoutAmount = 3; break;
            case ARank: payoutAmount = 4; break;
            case APlusRank: payoutAmount = 5; break;
            case SRank: payoutAmount = 3; break;
            case SPlusRank: payoutAmount = 6; break;
            case XRank: payoutAmount = 10; break;
            default: break;
        }
    }

    // Advantage-F's trick payout table
    if (player.extremeGear == ExtremeGear::AdvantageF) {
        switch (player.trickRank) {
            case CRank: payoutAmount = 1; break;
            case BRank: payoutAmount = 2; break;
            case ARank: payoutAmount = 3; break;
            case APlusRank: payoutAmount = 4; break;
            case SRank: payoutAmount = 3; break;
            case SPlusRank: payoutAmount = 5; break;
            case XRank: payoutAmount = 7; break;
            default: break;
        }
    }

    // MI affects trick payout, so calc that too
    if (MI::impulseData[player.index].magneticImpulse) {
        const s32 MIpercentage = static_cast<s32>(player.magneticImpulse_timer / MI::MaximumCap);
        s32 MIRings = MIpercentage;

        if (player.extremeGear == ExtremeGear::TheCrazy) {
            MIRings *= 10; // up to +10 rings
        } else if (player.extremeGear == ExtremeGear::AdvantageF) {
            MIRings *= 5; // up to +5 rings
        }
        if (!player.typeAttributes.hasAny(Type::All)) { MIRings *= 2; } // no-type doubles this
        payoutAmount += MIRings;
    }

    // ensures no overtricking air gain messes anything up by calc'ing the percentage properly here
    s32 const newRings = player.currentAir + (player.gearStats[0].maxAir * payoutAmount / 100);
    player.currentAir = clamp(newRings, 0, player.gearStats[0].maxAir);
}

ASMUsed void Player_MIPitAirGain(Player &player, const ObjectNode* pit){
	player.changeInAir_gain += 0xB4 + 0x384;

	if (player.extremeGear == ExtremeGear::SuperHangOn) {
		player.changeInAir_gain *= 3.0f;
	} else if (((pit->object_id == 0x1B || pit->object_id == 0x1A) && CurrentStage == SkyRoad && player.level == 0)) {
		player.changeInAir_gain *= 3;
	}

	if (MI::impulseData[player.index].magneticImpulse) {
		f32 MIpercentage = player.magneticImpulse_timer / MI::MaximumCap;
		f32 airGain = MIpercentage * 500.0f;
		if (player.characterArchetype == CharacterArchetype::Mechanic) { airGain *= 2.0f; }
		if (!player.typeAttributes.hasAny(Type::All)) {
			airGain *= MI::NoTypeMultiplier;
		}

		player.changeInAir_gain += static_cast<s32>(airGain);
	}
}

ASMUsed f32 Player_MIPitSpeed(Player &player, f32 pitSpeed){
	//EnabledEXLoads exLoads;
	//FetchEnabledEXLoadIDs(player, exLoads);
	// HHOInfo &hhoInfo = PlayerHHOInfo[player.index];

	// if (exLoads.gearExLoadID == HyperHangOn && hhoInfo.saturnMegadriveStatus != 2)
	// {
	//     hhoInfo.pitCount += (u8)1;
	//     pitSpeed += pSpeed(70.0f);
	//     PlayAudioFromDAT(Sound::SFX::TornadoSlingshot);
	// } else if (exLoads.gearExLoadID == HyperHangOn && hhoInfo.saturnMegadriveStatus == 2)
	// {
	//     pitSpeed += pSpeed(100.0f);
	//     return pitSpeed;
	// }
	player.reciproExtendTimer = 0;
	if (player.extremeGear == ExtremeGear::SuperHangOn) {
		pitSpeed += pSpeed(100.0f);
	}
	if (CurrentGameMode == FreeRace || CurrentGameMode == WorldGrandPrix || CurrentGameMode == StoryMode) {
		if (MI::impulseData[player.index].magneticImpulse) {
			f32 addSpeed = (player.magneticImpulse_timer / MI::MaximumCap) * pSpeed(100.0f);
			if (!player.typeAttributes.hasAny(Type::All)) {
				addSpeed *= MI::NoTypeMultiplier;
			}
			pitSpeed += addSpeed;
		}
	}
	return pitSpeed;
}

ASMUsed f32 Player_MITrickLandingSpeed(Player &player, f32 currentTrickSpeed){
	//OKGInfo &okgInfo = PlayerOKGInfo[player.index];
	// NeoMetalInfo *neoInfo = &PlayerNeoMetalInfo[player.index];

    // NOTE: Until we extern trick landing speeds & air gain (lbl_001C670C, rodata.s), this is our override.
    if (DebugMenu_CheckOption(DebugMenuOptions::PTRMode)) {
        switch (player.trickRank) {
            case SRank:
                currentTrickSpeed = pSpeed(270.0f);
                break;
            case SPlusRank:
                currentTrickSpeed = pSpeed(230.0f);
                break;
            case XRank:
                currentTrickSpeed = pSpeed(240.0f);
                break;
            default: break;
        }
    }

	if (player.trickRank > SPlusRank && player.extremeGear != ExtremeGear::Legend) {
		// S and C rank
		// player->specialFlags.hasAny(SpecialFlags::ringGear) ? pSpeed(2.0f) :
		f32 overtrickBonus = (DebugMenu_CheckOption(DebugMenuOptions::PTRMode)) ? pSpeed(2.5f) : pSpeed(2.0f);
		currentTrickSpeed += static_cast<f32>(player.trickCount - static_cast<u8>(4)) * overtrickBonus;
	}

	if (player.extremeGear == ExtremeGear::Legend) {
		if (player.rampType != 0x3) {
			// half pipe
			if (player.trickRank != SRank && player.trickRank != CRank) {
				// S and C rank
				currentTrickSpeed = pSpeed(100.0f) + (static_cast<f32>(player.trickCount) * pSpeed(35.0f));
			}
		}
	}

	if (player.rampType == 0x3) {
		currentTrickSpeed += pSpeed(100.0f);
		if (!player.typeAttributes.hasAny(Type::All)) {
			// no type
			currentTrickSpeed += pSpeed(50.0f);
		}
	}

	if (player.characterArchetype == CharacterArchetype::Trickster) {
		// currentTrickSpeed += pSpeed(50.0f); // Removed for new trick system
		if (player.trickRank >= SPlusRank) {
			MI::impulseData[player.index].afterburnerTimer = calculateAfterburnerTimer(player);
		}
	}

	if (player.extremeGear == ExtremeGear::Gambler && player.trickRank != SRank && player.trickRank != CRank)
	{player.rings += static_cast<u32>(player.trickCount);}

    if (player.extremeGear == ExtremeGear::Darkness && player.gearExload().exLoadID == EXLoad::TheProfessional && player.level == 2 && player.rings >= 60) {
        currentTrickSpeed += pSpeed(30.0f) * (static_cast<f32>(player.rings - 60) / 40.0f);
    }

	// if (isSuperCharacter(*player, Character::MetalSonic) && player->gearExload().exLoadID == EXLoad::StardustSpeeder && player->trickRank != CRank) {
	// 	if (neoInfo->chaosControlTimer > 0) neoInfo->trickCount += static_cast<u32>(player->trickCount); // adds trick count during phase 1
	// 	if (neoInfo->chaosControlTimer > 0 && neoInfo->chaosControlState == 1) neoInfo->totalChaosActionCount += static_cast<u32>(player->trickRank * 9); // adds trick count during phase 1
	// 	if (neoInfo->chaosControlState == 2) {
	// 		// player->rings += static_cast<u32>(player->trickCount); // adds rings during phase 2
	// 		currentTrickSpeed += static_cast<f32>(player->trickRank) * pSpeed(2.5f); // doubles overtricking
	// 	}
	// }

	if (MI::impulseData[player.index].magneticImpulse) {
		f32 MIspeed = (player.magneticImpulse_timer / MI::MaximumCap) * pSpeed(50.0f);
		if (!player.typeAttributes.hasAny(Type::All)) { MIspeed *= MI::NoTypeMultiplier; }
		currentTrickSpeed += MIspeed;
	}

	return currentTrickSpeed;
}

ASMUsed s16 Player_MIBabylonGardenMiniSpiralQTE(const Player &player, void* qte){
	s16 qteSpeed = *(static_cast<s16 *>(qte) + 0x20 / 2);

	if (CurrentStage == BabylonGarden) {
		qteSpeed = 250;

		if (player.unkABC > 250)
			qteSpeed = (player.unkABC > 300) ? 300 : player.unkABC;

		if (MI::impulseData[player.index].magneticImpulse) {
			const f32 MIpercentage = player.magneticImpulse_timer / MI::MaximumCap;
			f32 additiveSpeed = 50.0f * MIpercentage;
			if (!player.typeAttributes.hasAny(Type::All)) { additiveSpeed *= MI::NoTypeMultiplier; }
			qteSpeed += static_cast<s16>(additiveSpeed);
		}
	}
	return qteSpeed;
}

ASMUsed s32 Player_MIRingBoxGain(const Player &player){
	s32 extraRings = 0;
	if (MI::impulseData[player.index].magneticImpulse) {
		extraRings = static_cast<s16>((player.magneticImpulse_timer / MI::MaximumCap) * 2.0f);
	}

	return extraRings;
}

ASMUsed f32 Player_MIRingDrain(const Player &player, f32 ringDrain){
	if (player.extremeGear == ExtremeGear::SuperHangOn && player.gearSpecificFlags[SuperHangOn::Level4]) {
		ringDrain *= 1.365f;
	}
	else if (MI::impulseData[player.index].magneticImpulse) {
		if (player.specialFlags.hasAny(SpecialFlags::ringGear)) {
			const f32 ringDrainInvert = ringDrain < 0
											? 1.0f
											: -1.0f;

			const f32 multiplier = __fabsf(ringDrainInvert + (player.magneticImpulse_timer / MI::MaximumCap));
			// NOLINT(cppcoreguidelines-narrowing-conversions)
			ringDrain *= multiplier;
		}
	}
	else if (MI::impulseData[player.index].ringPowerObjTimer > 0.0f) {
		ringDrain = -1.0f;
		MI::impulseData[player.index].ringPowerObjTimer -= 1.0f;
	}

	return ringDrain;
}

ASMUsed void Player_MISpeedShoes(Player &player){
	f32 extraSpeed = 0.0f;

	if (MI::impulseData[player.index].magneticImpulse) {
		extraSpeed = (player.magneticImpulse_timer / MI::MaximumCap) * pSpeed(50);
	}
	if (!player.typeAttributes.hasAny(Type::All)) { extraSpeed *= MI::NoTypeMultiplier; }

	player.speed += pSpeed(100) + extraSpeed;
	if (player.characterArchetype == CharacterArchetype::Mechanic) player.speed += pSpeed(50);
}

ASMUsed void Player_MIAirBoxGain(Player &player, const s32 airToAdd) {
	f32 multiplier = (!player.typeAttributes.hasAny(Type::All)) ? 1.3f : 1.0f;

	if (MI::impulseData[player.index].magneticImpulse) {
		if (player.typeAttributes.hasAny(Type::All)) {
			multiplier += (player.magneticImpulse_timer / MI::MaximumCap) * 0.3f;
		} else { // No Type
			multiplier += (player.magneticImpulse_timer / MI::MaximumCap) * 0.3f;
		}
	}

	const auto scaledAir = static_cast<f32>(airToAdd) * multiplier;
	player.changeInAir_gain += static_cast<s32>(scaledAir);
}

ASMDefined void* lbl_001F9C30;

void MagneticImpulse::createGuaranteedMaxMIParticles(const Player &player){
	if (gu32GameCnt % 3 != 0) { return; }

	auto *particles = static_cast<ParticleTaskObject1 *>(SetTask(func_Particle_Task, ObjectGroups::Particle, Object1Sizes::x80)->object);
	particles->unk72 = 4;
	particles->x = 0;
	particles->y = 0;
	particles->z = 0;
	particles->unk30 = 0;
	particles->unk34 = 0;
	particles->unk38 = 0;
	particles->unk3C = 0.5F;
	particles->unk40 = 0.1F;
	particles->unk44 = 0.1F;
	particles->unk10 = 0;
	particles->unk14 = 0;
	particles->unk18 = 0;
	particles->unk48 = &gcosNnSystemVecZeroFast;
	particles->particleParams = &lbl_001F9C30;
	particles->texList = gpsTexList_Particle;
	particles->baseModelMatrix = &MI::PlayerPositionRotationMatrices[player.index];
	particles->unk73 = 0xFF;
	particles->unk74 = 0;
}

/**
 * \brief Gets the extra amount of rings a player should get from Sega Carnival's ring granting ring hoops at the start of the stage.
 * \param player The player to get the extra ring count from.
 * \return The amount of rings that should be added on top of the player's rings
 */
ASMUsed u32 MI_GetRingGrantingHoopRingCount(Player &player) {
	constexpr auto additiveRings = 15;
	constexpr u32 extraMIrings = 5U;

	// vanilla code
	u32 rings = static_cast<u32>(!player.flags.hasAny(PlayerFlags::Unknown3)) * additiveRings;

	// mi scale
	if (player.specialFlags.hasAny(SpecialFlags::ringGear)) return rings;
	rings += MagneticImpulse::scaleUsingCurrentMI(player, extraMIrings);

	return rings;
}

/**
 * \brief Gets the extra amount of rings a player should get from Sega Carnival's Super Monkey Ball ring gate.
 * \param player The player to get the extra ring count from.
 * \param currentRingCount The current ring count used by the ring gate.
 * \return The new ring count that should be added on top of the player's rings.
 */
ASMUsed u32 MI_GetSuperMonkeyBallGateRingCount(Player &player, const u32 currentRingCount) {
	constexpr u32 extraMIrings = 10U;
	// if (player.specialFlags.hasAny(ringGear))
	// {
	// 	s32 ringGearBonus= (player.gearStats[player.level].maxAir *
	// 						static_cast<s32>(MagneticImpulse::scaleUsingCurrentMI(player, extraMIrings))) / 100;
	// 	player.currentAir += ringGearBonus;
	// }
	return currentRingCount + MagneticImpulse::scaleUsingCurrentMI(player, extraMIrings);
}

inline void UpdatePlayerMatrices(Player &player){
	GetPlayerPositionRotationMatrix(player, MI::PlayerPositionRotationMatrices[player.index]);
}

void MagneticImpulseEnabled(Player &player){
	MI::ImpulseData&midata = MI::impulseData[player.index];

	if (player.state == PlayerState::QTE) { return; }
	midata.interrupt = 0;
	if (midata.buffer < MI::BufferTime) {
		midata.buffer += 1;
		return;
	}

	midata.magneticImpulse = true;
	player.magneticImpulse_soundStatus &= 0x3; // clear out mi dormant and reaches zero status
	if ((player.magneticImpulse_soundStatus & 0x2) == 0) {
		// check for mi dormant sfx status
		player.magneticImpulse_soundStatus |= 0x2; // set mi dormant status
		if (!player.playerType) { PlayAudioFromDAT(Sound::SFX::MIActive); }
	}

	if (player.magneticImpulse_timer < MI::MaximumCap) {
		player.magneticImpulse_timer += 1;
		return;
	}

	player.magneticImpulse_soundStatus &= 0x3;
	if ((player.magneticImpulse_soundStatus & 0x1) == 0) {
		// should be at max MI here
		player.magneticImpulse_soundStatus |= 0x1;
		if (!player.playerType) {
			PlayAudioFromDAT(Sound::SFX::MaximumMI);
			MI::playMaxMILine(player);
			MI::setMaxMIAnimation(player);
		}
		MI::levelUpStars(player);
	}

	if (midata.maxMItimer < MI::MaxMITime) {
		midata.maxMItimer += 1;
		// MI::createGuaranteedMaxMIParticles(player);
	}
}

void MagneticImpulseDisabled(Player &player){
	MI::ImpulseData &midata = MI::impulseData[player.index];

	if (CurrentStage == BabylonGarden) {
		if (player.state == PlayerState::TurbulenceRide) {
			// compare turbulence index to player count to figure out if player is riding on big turb
			if (InGamePlayerCount == player.closestTurbulenceIndex) { return; }
		}
	}

	if (player.magneticImpulse_timer >= MI::MaximumCap) {
		if (player.placement != 0) {
			if (midata.maxMItimer < MI::MaxMITime) {
				MagneticImpulseEnabled(player);
				return;
			}
		}
	}

	if (midata.interrupt < MI::BufferTime) {
		midata.interrupt += 1;
		return;
	}

	midata.buffer = 0;
	midata.maxMItimer = 0;
	midata.magneticImpulse = false;

	player.magneticImpulse_soundStatus &= 0xC;
	if ((player.magneticImpulse_soundStatus & 0x4) == 0) {
		player.magneticImpulse_soundStatus |= 0x4;
		if (!player.playerType) { PlayAudioFromDAT(Sound::SFX::MIDormant); }
	}

	if (player.magneticImpulse_timer > MI::LossTimeCap) {
		player.magneticImpulse_timer = MI::LossTimeCap;
		if (player.placement == 0) player.magneticImpulse_timer = 0;
	}

	if (player.magneticImpulse_timer > 0) {
		player.magneticImpulse_timer -= 1;
		return;
	}

	player.magneticImpulse_soundStatus &= 0xC;
	if ((player.magneticImpulse_soundStatus & 0x8) == 0) {
		player.magneticImpulse_soundStatus |= 0x8;
		if (!player.playerType) { PlayAudioFromDAT(Sound::SFX::MIZero); }
	}
}

void Player_MagneticImpulseTimer(Player &player){
	if (PlayerHyperSonicInfo[player.index].hyperdriveEnabled && player.gearExload().exLoadID == EXLoad::HyperSonic && player.movementFlags.hasAny(MovementFlags::boosting)) {
		MI::createGuaranteedMaxMIParticles(player);
	}
	if (!DebugMenu_CheckOption(DebugMenuOptions::MagneticImpulse)) { return; }
	UpdatePlayerMatrices(player); // Kept in this spot to avoid changing existing behavior, Todo: check if this should stay
	if (CurrentGameMode != FreeRace && CurrentGameMode != WorldGrandPrix && CurrentGameMode != StoryMode) { return; }
	if (InGamePlayerCount < 2 && !DebugMenu_CheckOption(DebugMenuOptions::AlwaysMaxMI)) { return; }
	if (player.characterArchetype == CharacterArchetype::Windcatcher) { return; }
	if (player.state == PlayerState::StartLine) { return; }
	if (DebugMenu_CheckOption(DebugMenuOptions::AlwaysMaxMI)) {
		player.magneticImpulse_timer = MI::MaximumCap;
		MagneticImpulseEnabled(player);
		return;
	}

	const Player &firstPlayer = getPlayerInFirst();

	if(player.otherPlayerDistance[firstPlayer.index] < 1700 && firstPlayer.placement != player.placement) { return; }

	const bool turbulenceBool = player.state == PlayerState::TurbulenceRide;
	const bool &slipstreamBool = player.slipstream;

	if (!turbulenceBool && !slipstreamBool && player.placement != 0) {
		MagneticImpulseEnabled(player);
	}
	else {
		// MI off
		MagneticImpulseDisabled(player);
	}
}
