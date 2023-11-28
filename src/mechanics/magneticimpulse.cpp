#include "mechanics/magneticimpulse.hpp"
#include "cosmetics/player/exloads.hpp"
#include "gears/blastGaugeGears.hpp"
#include "gears/hangon.hpp"
#include "gears/hypersonic.hpp"
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
//#include "gears/olliekinggear.hpp"
// #include "gears/eggsterminator.hpp"

struct LevelUpStarsObject1;
struct Particle_LevelUpStars;

namespace MagneticImpulse {
	std::array<MI::ImpulseData, MaxPlayerCount> impulseData;
	std::array<Matrix3x3F, MaxPlayerCount> PlayerPositionRotationMatrices;

	void playMaxMILine(const Player &player);
	void levelUpStars(const Player &player);

	constexpr auto VoiceLinesPerChar = 3;
	constexpr m2darray<u8, TotalCharacterAmount, VoiceLinesPerChar> VoiceLineIDs = {{
			{0x02},            // Sonic
			{0x02},            // Tails
			{0x02},            // Knuckles
			{0x02},            // Amy
			{0x02},            // Jet
			{0x02},            // Storm
			{0x02},            // Wave
			{0x02},            // Eggman
			{0x02},            // Cream
			{0x02},            // Rouge
			{0x02},            // Shadow
			{0x02},            // SuperSonic
			{0x02, 0x03, 19},  // Nights
			{0x02},            // Aiai
			{0x02},            // Ulala
			{},                // E10G
			{},                // E10R
			{0x08, 0x09, 0x0A},// Silver
			{0x07, 0x08, 0x09},// MetalSonic
			{0x02, 0x07},      // Emerl
			{0x08, 0x09},      // Blaze
			{0x07},            // Chaos
			{8, 9, 10}         // Tikal
	}};
	constexpr m2darray<u8, EXLoadCount, VoiceLinesPerChar> ExLoadVoiceLineIDs = {{
			{},
			{},              // Perfect Nazo
			{},              // E10R
			{},              // Dark Sonic
			{12},            // Hyper Sonic
			{},              // Stardust Speeder
			{},              // Hyper HangOn
			{},              // The Professional
			{},              // Ollie King Gear
			{},              // Windmaster Jet
			{},              // Neo Metal
			{},              // E99
			{},              // The Beast
			{},              // E10B
			{8},             // Miku
			{},              // Metal Sonic Scarf
			{0x02, 0x03, 19},// Reala
			{8},             // Gongon
	}};
}// namespace MagneticImpulse

void MagneticImpulse::playMaxMILine(const Player &player) {
	const u8 &character = player.character;
	const auto exload = FetchEnabledEXLoadIDs(player);
	const std::span MIVoiceLines = !exload.isExloadEnabled() ? MI::VoiceLineIDs[character] : MI::ExLoadVoiceLineIDs[exload.characterExLoadID];

	u32 usedSlots = 0;
	while(usedSlots < MI::VoiceLinesPerChar) {// Get how many voicelines this character actually has
		if(MIVoiceLines[usedSlots] == 0) { break; }
		usedSlots++;
	}
	if(usedSlots == 0) { return; }// Character has no lines to play

	const u32 selectedLine = lbl_RNG_Number(usedSlots);
	Sound::PlaySound(player, MIVoiceLines[selectedLine]);
}

struct LevelUpStarsObject1 {
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
	auto *particles = reinterpret_cast<LevelUpStarsObject1 *>(SetTask(func_Particle_Task, ObjectGroups::Particle, 2)->object);
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

f32 MagneticImpulse::calculateMultiplier(Player *player, f32 value) {
	const auto typeCount = player->getTypeCount();
	if(typeCount == 2) {
		value *= DualTypeMultiplier;
	} else if(typeCount == 3) {
		value *= AllTypeMultiplier;
	}

	return value;
}
f32 MagneticImpulse::scaleUsingCurrentMI(Player &player, f32 value) {
	if(impulseData[player.index].magneticImpulse) {
		const f32 MIPercentage = player.magneticImpulse_timer / MaximumCap;
		return value * MIPercentage;
	}

	return 0.0f;
}

ASMUsed f32 MagneticImpulse_ParticleSpeed(Player *player) {
	return 0.0505316F + (player->magneticImpulse_timer * 0.00012632867F);
}

ASMUsed void MagneticImpulse_AttackParticles(Player *player, u8 battleModeData[], u32 offset) {
	MI::ImpulseData *magneto = &MI::impulseData[player->index];
	if(CurrentGameMode == FreeRace || CurrentGameMode == WorldGrandPrix || CurrentGameMode == StoryMode) {
		u8 particleState;
		if(!magneto->magneticImpulse) {
			particleState = 1;
		} else if(player->magneticImpulse_timer < MI::MaximumCap) {
			particleState = 2;
		} else {
			particleState = 3;
		}

		battleModeData[offset] = particleState;
	}
}

ASMUsed void MagneticImpulse_GuardParticles(Player *player, u8 battleModeData[], u32 offset) {
	MI::ImpulseData *magneto = &MI::impulseData[player->index];
	if(CurrentGameMode == FreeRace || CurrentGameMode == WorldGrandPrix || CurrentGameMode == StoryMode) {
		u8 particleState;
		if(!magneto->magneticImpulse) {
			if(player->magneticImpulse_timer == 0) {
				particleState = 0;
			} else {
				particleState = 1;
			}
		} else {
			if(player->magneticImpulse_timer < MI::MaximumCap) {
				particleState = 0;
			} else {
				particleState = 2;
			}
		}

		battleModeData[offset] = particleState;
	}
}

ASMUsed f32 Player_MISpeedAirGain(Player *player, f32 airGain) {
	if(player->extremeGear == ExtremeGear::CoverS && player->state == RailGrind) {
		if(player->characterptr->type == Character::SpeedType) {
			airGain *= 0.5f;
		} else if ((player->characterptr->type == Character::FlyType 
					|| player->characterptr->type == Character::PowerType
					|| player->characterptr->type == Character::NoType)) {
			airGain *= 0.30f;
		}
	}

	if(player->characterArchetype == Mechanic) airGain *= 1.15f;

	if(MI::impulseData[player->index].magneticImpulse) {
		airGain += player->magneticImpulse_timer / 3.0f;
	}

	const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);// for grind rails
	if(player->grindRailDash == 1) {
		if(player->specialFlags.hasAny(ringGear)) {
			BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];
			if(player->character == SuperSonic
			   && bgInfo->currentGauge > 0
			   && exLoads.gearExLoadID != HyperSonicEXLoad) {
				const s32 newBlastGaugeValue = bgInfo->currentGauge + static_cast<s32>(GrindRailDashAirCosts[1]);
				bgInfo->currentGauge = clamp(newBlastGaugeValue, 0);
			} else {
				s32 newAir;
				if(player->characterArchetype == Mechanic) {
					newAir = static_cast<s32>(player->currentAir - (player->gearStats[player->level].maxAir * 0.02) / 100);
				} else {
					newAir = static_cast<s32>(player->currentAir - (player->gearStats[player->level].maxAir * 0.05) / 100);
				}
				player->currentAir = clamp(newAir, 0);
			}
		} else {
			if(player->characterArchetype == Mechanic) {
				airGain = GrindRailDashAirCostsMechanic[player->level];
			} else {
				airGain = GrindRailDashAirCosts[player->level];
			}
		}
	}
	if(exLoads.gearExLoadID == TheBeastEXLoad) { return 0; }
	return airGain;
}

ASMUsed f32 Player_MIFlyAirGain(Player *player, f32 airGain) {
	// for fly hoops
	const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);
	PlayerFlyHoopDashProperties *properties = &playerFlyHoopDashProperties[player->index];
	BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];

	if(player->extremeGear == ExtremeGear::CoverS && player->state == Fly) {
		if(player->characterptr->type == Character::FlyType) {
			airGain *= 0.5f;
		} else if ((player->characterptr->type == Character::PowerType 
					|| player->characterptr->type == Character::SpeedType
					|| player->characterptr->type == Character::NoType)) {
			airGain *= 0.30f;
		}
	}

	if(player->characterArchetype == Mechanic) { airGain *= 1.10f; }

	if(MI::impulseData[player->index].magneticImpulse) {
		airGain += player->magneticImpulse_timer / 3.0f;
	}

	// This involves the new hoop dash V2.
	if(properties->hasUsed) {
		// This first if-statement is for ring gears
		if(player->specialFlags.hasAny(ringGear)) {
			if(player->character == SuperSonic
			   && bgInfo->currentGauge > 0
			   && exLoads.gearExLoadID != HyperSonicEXLoad) {
				// See the air section below, same logic
				bgInfo->currentGauge -= FlyHoopDashInitialCosts[player->level];
				if(bgInfo->currentGauge < 0) { bgInfo->currentGauge = 0; }
			} else {
				// This statement removes rings from ring gears. Change the "5" to change the
				// amount of rings taken
				s32 newAir;
				if(player->characterArchetype == Mechanic) {
					newAir = player->currentAir - (player->gearStats[player->level].maxAir * 3) / 100;
				} else {
					newAir = player->currentAir - (player->gearStats[player->level].maxAir * 5) / 100;
				}
				player->currentAir = clamp(newAir, 0);
			}
		} else {
			// This statement removes air from the player. Change the costs in "FlyHoopDashInitialCosts[player->level]"
			// (in flyhoopdash.cpp) to alter how much air is taken
			s32 newAir;
			if(player->characterArchetype == Mechanic) {
				newAir = player->currentAir - FlyHoopDashInitialCostsMechanic[player->level];
			} else {
				newAir = player->currentAir - FlyHoopDashInitialCosts[player->level];
			}
			player->currentAir = clamp(newAir, 0);
		}
		// properties->duration = 0;
		properties->hasUsed = FALSE;
	}

	// Handles the delay for new fly hoop dash. Subtracts 1 frame from the timer every frame and pauses air gain.
	if(properties->gainDelay > 0.0f) {
		properties->gainDelay -= 1.0f;
		return 0;
	}

	if(player->flyHoopDash == 1) {
		if(player->specialFlags.hasAny(ringGear)) {
			if(player->character == SuperSonic
			   && bgInfo->currentGauge > 0
			   && exLoads.gearExLoadID != HyperSonicEXLoad) {
				f32 newBlastGaugeValue = bgInfo->currentGauge + static_cast<s32>(FlyHoopDashAirLoss[1]);
				// player->shortcutAirGainMultiplier -= FlyHoopDashAirLoss[player->level];
				if(newBlastGaugeValue < 0) { newBlastGaugeValue = 0; }
				bgInfo->currentGauge = newBlastGaugeValue;
			} else {
				const s32 newAir = player->currentAir - (player->gearStats[player->level].maxAir * 0.05) / 100;
				player->currentAir = clamp(newAir, 0);
			}
		} else {
			airGain = FlyHoopDashAirLoss[player->level];
		}
	}
	if(exLoads.gearExLoadID == TheBeastEXLoad) { return 0; }
	return airGain;
}

ASMUsed f32 Player_MIPowerObjectAirGain(Player *player, f32 airGain) {
	// for power objects
	const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);
	const HyperSonicInfo &hsInfo = PlayerHyperSonicInfo[player->index];
	BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];

	if(player->extremeGear == ExtremeGear::CoverS) {
		if(player->characterptr->type == Character::PowerType) {
			airGain *= 0.5F;
		} else if ((player->characterptr->type == Character::SpeedType 
					|| player->characterptr->type == Character::FlyType
					|| player->characterptr->type == Character::NoType)) {
			airGain *= 0.30F;
		}
	}

	if(player->characterArchetype == Mechanic) airGain *= 1.20f;

	if(player->character == SuperSonic
	   && bgInfo->currentGauge > 0
	   && exLoads.gearExLoadID != HyperSonicEXLoad
	   && player->movementFlags.hasAny(boosting)) {// if blast gauge is available, subtract half a trick
		s32 newBlastGaugeValue = bgInfo->currentGauge - 10000;
		if(newBlastGaugeValue < 0) { newBlastGaugeValue = 0; }
		bgInfo->currentGauge = newBlastGaugeValue;
	}

	if(exLoads.gearExLoadID == HyperSonicEXLoad && hsInfo.hyperdriveEnabled) {
		bgInfo->currentGauge += 1700;
	}

	if(MI::impulseData[player->index].magneticImpulse) {
		airGain += player->magneticImpulse_timer * 5.0f;
	}
	if(exLoads.gearExLoadID == TheBeastEXLoad) { return 0; }
	return airGain;
}

ASMUsed f32 Player_MITrickAirGain(Player *player, f32 airGain) {
	// HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];
	BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];

	if(player->isBlastGaugeGear) {// for all blast gear gauges, use this when they land their tricks
		const HyperSonicInfo &hsInfo = PlayerHyperSonicInfo[player->index];
		const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);
		if(exLoads.gearExLoadID == HyperSonicEXLoad && hsInfo.hyperdriveEnabled) {
			//player->shortcutAirGainMultiplier += player->trickCount * 0;
			//}else if(exLoads.gearExLoadID == HyperSonicEXLoad && !hsInfo->hyperdriveEnabled){
			//	player->shortcutAirGainMultiplier += player->trickCount * 10000;
			//}else if(exLoads.gearExLoadID != HyperSonicEXLoad && player->character == SuperSonic){
			//	player->shortcutAirGainMultiplier += player->trickCount * 10000;
		} else {
			// player->shortcutAirGainMultiplier += player->trickCount * 10000;
			bgInfo->currentGauge += player->trickCount * 10000;
		}
		if(bgInfo->currentGauge >= 200000) {
			// player->shortcutAirGainMultiplier = 200000;
			bgInfo->currentGauge = 200000;
		}
	}

	// if (exLoads.gearExLoadID == HyperHangOnEXLoad && hhoInfo->saturnMegadriveStatus != 2)
	// {
	//     hhoInfo->trickCount += 1;
	//     PlayAudioFromDAT(Sound::SFX::TornadoSlingshot);
	// }

	// if (exLoads.gearExLoadID == TheBeastEXLoad && !(player->specialFlags & ringGear))
	// {
	//     player->rings += 10;
	//     return 0;
	// }

	if(MI::impulseData[player->index].magneticImpulse) {
		const f32 MIpercentage = player->magneticImpulse_timer / MI::MaximumCap;
		f32 MIair = MIpercentage * 50000.0f;
		if(player->extremeGear == ExtremeGear::TheCrazy) {
			MIair *= 0.2f;
		} else if(player->extremeGear == ExtremeGear::AdvantageF) {
			MIair *= 0.5f;
		}
		if(!player->typeAttributes.hasAny(Type::All)) { MIair *= MI::NoTypeMultiplier; }
		if (MIair > 0.0f && player->extremeGear == ExtremeGear::SuperHangOn) MIair = MIpercentage * 20000.0f;
		airGain += MIair;
	}
	return airGain;
}

ASMUsed void Player_MIPitAirGain(Player *player, ObjectNode *pit) {
	f32 airGain, MIpercentage;
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
	if(exLoads.gearExLoadID == E99EXLoad) {
		player->currentAir = player->gearStats[player->level].maxAir;
	}

	player->changeInAir_gain += 0xB4 + 0x384;

	if(player->extremeGear == ExtremeGear::SuperHangOn) {
		player->changeInAir_gain *= 2.4f;
	} else if(((pit->object_id == 0x1B || pit->object_id == 0x1A) && CurrentStage == SkyRoad && player->level == 0)) {
		player->changeInAir_gain *= 3;
	}

	if(MI::impulseData[player->index].magneticImpulse) {
		MIpercentage = player->magneticImpulse_timer / MI::MaximumCap;
		airGain = MIpercentage * 500.0f;
		if(player->characterArchetype == Mechanic) { airGain *= 2.0f; }
		if(!player->typeAttributes.hasAny(Type::All)) airGain *= MI::NoTypeMultiplier;

		player->changeInAir_gain += static_cast<s32>(airGain);
	}
}

ASMUsed f32 Player_MIPitSpeed(Player *player, f32 pitSpeed) {
	//EnabledEXLoads exLoads;
	//FetchEnabledEXLoadIDs(player, exLoads);
	// HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];
	// EggInfo *EggInfo = &PlayerEggsterminator[player->index];

	// if (exLoads.gearExLoadID == HyperHangOnEXLoad && hhoInfo->saturnMegadriveStatus != 2)
	// {
	//     hhoInfo->pitCount += (u8)1;
	//     pitSpeed += pSpeed(70.0f);
	//     PlayAudioFromDAT(Sound::SFX::TornadoSlingshot);
	// } else if (exLoads.gearExLoadID == HyperHangOnEXLoad && hhoInfo->saturnMegadriveStatus == 2)
	// {
	//     pitSpeed += pSpeed(100.0f);
	//     return pitSpeed;
	// }

	// if (exLoads.gearExLoadID == E99EXLoad)
	// {
	//     EggInfo->emeraldsInserted += 1;
	//     PlayAudioFromDAT(Sound::SFX::SuperTransformation);
	// }
	if(CurrentGameMode == FreeRace || CurrentGameMode == WorldGrandPrix || CurrentGameMode == StoryMode) {
		if(MI::impulseData[player->index].magneticImpulse) {
			f32 addSpeed = (player->magneticImpulse_timer / MI::MaximumCap) * pSpeed(100.0f);
			if(!player->typeAttributes.hasAny(Type::All)) {
				addSpeed *= MI::NoTypeMultiplier;
			}
			pitSpeed += addSpeed;
		}
	}
	return pitSpeed;
}

ASMUsed f32 Player_MITrickLandingSpeed(Player *player, f32 currentTrickSpeed) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
	// OKGInfo *okgInfo = &PlayerOKGInfo[player->index];

	if(player->trickRank > 0x5 && player->extremeGear != ExtremeGear::Legend && player->characterArchetype != TricksterArchetype) {// S and C rank
		currentTrickSpeed += ((f32) (player->trickCount - (u8) 4) * pSpeed(2.0f));
	}

	if(player->extremeGear == ExtremeGear::Legend || exLoads.gearExLoadID == TheProfessionalEXLoad) {
		if(player->rampType != 0x3) {                                     // half pipe
			if(player->trickRank != SRank && player->trickRank != CRank) {// S and C rank
				currentTrickSpeed = pSpeed(100.0f) + (static_cast<f32>(player->trickCount) * pSpeed(35.0f));
			}
		}
	}

	if(player->rampType == 0x3) {
		currentTrickSpeed += pSpeed(100.0f);
		if(!player->typeAttributes.hasAny(Type::All)) {
			// no type
			currentTrickSpeed += pSpeed(50.0f);
		}
	}

	if(player->characterArchetype == TricksterArchetype) {
		currentTrickSpeed += pSpeed(50.0f);
		if(player->trickRank >= SPlusRank) {
			MI::impulseData[player->index].afterburnerTimer = (player->trickCount - 1u) * 60u;
		}
	}

	if(MI::impulseData[player->index].magneticImpulse) {
		f32 MIspeed = (player->magneticImpulse_timer / MI::MaximumCap) * pSpeed(50.0f);
		if(!player->typeAttributes.hasAny(Type::All)) { MIspeed *= MI::NoTypeMultiplier; }
		currentTrickSpeed += MIspeed;
	}

	// if (exLoads.gearExLoadID == OllieKingGearEXLoad)
	// {
	//     switch (player->characterArchetype)
	//     {
	//         case TopSpeedArchetype:
	//             currentTrickSpeed += pSpeed(30.0f);
	//             break;
	//         case AllRounder:
	//             currentTrickSpeed += pSpeed(20.0f);
	//             break;
	//         case CombatArchetype:
	//         case LateBooster:
	//             currentTrickSpeed += pSpeed(10.0f);
	//             break;
	//     }
	//     player->gearStats[player->level].boostSpeed = currentTrickSpeed;
	//     okgInfo->OllieBurstStatus = 1;
	// }

	return currentTrickSpeed;
}

ASMUsed s16 Player_MIBabylonGardenMiniSpiralQTE(Player *player, void *qte) {
	s16 qteSpeed = *(reinterpret_cast<s16 *>(qte) + 0x20 / 2);

	if(CurrentStage == BabylonGarden) {
		qteSpeed = 250;

		if(player->unkABC > 250)
			qteSpeed = player->unkABC;

		if(MI::impulseData[player->index].magneticImpulse) {
			const f32 MIpercentage = player->magneticImpulse_timer / MI::MaximumCap;
			f32 additiveSpeed = 50.0f * MIpercentage;
			if(!player->typeAttributes.hasAny(Type::All)) { additiveSpeed *= MI::NoTypeMultiplier; }
			qteSpeed += static_cast<s16>(additiveSpeed);
		}
	}
	return qteSpeed;
}

ASMUsed s32 Player_MIRingBoxGain(Player *player) {
	s32 extraRings = 0;
	if(MI::impulseData[player->index].magneticImpulse) {
		extraRings = static_cast<s16>((player->magneticImpulse_timer / MI::MaximumCap) * 2.0f);
	}

	return extraRings;
}

ASMUsed f32 Player_MIRingDrain(Player *player, f32 ringDrain) {
	if(player->extremeGear == ExtremeGear::SuperHangOn && player->gearSpecificFlags[SuperHangOn::Level4]) {
		ringDrain *= 1.3f;
	} else if(MI::impulseData[player->index].magneticImpulse) {
		if(player->specialFlags.hasAny(ringGear)) {
			const f32 ringDrainInvert = ringDrain < 0
												? 1.0f
												: -1.0f;

			const f32 multiplier = __fabsf(ringDrainInvert + (player->magneticImpulse_timer / MI::MaximumCap));// NOLINT(cppcoreguidelines-narrowing-conversions)
			ringDrain *= multiplier;
		}
	} else if(MI::impulseData[player->index].ringPowerObjTimer > 0.0f) {
		ringDrain = -1.0f;
		MI::impulseData[player->index].ringPowerObjTimer -= 1.0f;
	}

	return ringDrain;
}

ASMUsed void Player_MISpeedShoes(Player *player) {
	f32 extraSpeed = 0.0f;

	if(MI::impulseData[player->index].magneticImpulse) {
		extraSpeed = (player->magneticImpulse_timer / MI::MaximumCap) * pSpeed(100);
	}
	if(!player->typeAttributes.hasAny(Type::All)) { extraSpeed *= MI::NoTypeMultiplier; }

	player->speed += pSpeed(100) + extraSpeed;
	if(player->characterArchetype == Mechanic) player->speed += pSpeed(50);
}

ASMUsed void Player_MIAirBoxGain(Player *player, const s32 airToAdd) {
	f32 multiplier = 1.0f;

	if(MI::impulseData[player->index].magneticImpulse) {
		if(player->typeAttributes.hasAny(Type::All)) {
			multiplier += (player->magneticImpulse_timer / MI::MaximumCap) * 0.3f;
		} else {// No Type
			multiplier += (player->magneticImpulse_timer / MI::MaximumCap) * 0.6f;
		}
	}

	const auto scaledAir = static_cast<f32>(airToAdd) * multiplier;
	player->changeInAir_gain += static_cast<s32>(airToAdd);
}

ASMDefined void *lbl_001F9C30;

void MagneticImpulse::createGuaranteedMaxMIParticles(Player &player) {
	if(gu32GameCnt % 3 != 0) { return; }

	auto *particles = reinterpret_cast<ParticleTaskObject1 *>(SetTask(func_Particle_Task, ObjectGroups::Particle, 2)->object);
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
	particles->unk60 = &lbl_001F9C30;
	particles->unk68 = gpsTexList_Particle;
	particles->unk6C = &MI::PlayerPositionRotationMatrices[player.index];
	particles->unk73 = 0xFF;
	particles->unk74 = 0;
}

inline void UpdatePlayerMatrices(Player &player) {
	GetPlayerPositionRotationMatrix(player, MI::PlayerPositionRotationMatrices[player.index]);
}

void MagneticImpulseEnabled(Player &player) {
	MI::ImpulseData &midata = MI::impulseData[player.index];

	if(player.state == QTE) { return; }
	midata.interrupt = 0;
	if(midata.buffer < MI::BufferTime) {
		midata.buffer += 1;
		return;
	}

	midata.magneticImpulse = true;
	player.magneticImpulse_soundStatus &= 0x3;           // clear out mi dormant and reaches zero status
	if((player.magneticImpulse_soundStatus & 0x2) == 0) {// check for mi dormant sfx status
		player.magneticImpulse_soundStatus |= 0x2;       // set mi dormant status
		if(!player.playerType) { PlayAudioFromDAT(Sound::SFX::MIActive); }
	}

	if(player.magneticImpulse_timer < MI::MaximumCap) {
		player.magneticImpulse_timer += 1;
		return;
	}

	player.magneticImpulse_soundStatus &= 0x3;
	if((player.magneticImpulse_soundStatus & 0x1) == 0) {
		// should be at max MI here
		player.magneticImpulse_soundStatus |= 0x1;
		if(!player.playerType) {
			PlayAudioFromDAT(Sound::SFX::MaximumMI);
			MI::playMaxMILine(player);
		}
		MI::levelUpStars(player);
	}

	if(midata.maxMItimer < MI::MaxMITime) {
		midata.maxMItimer += 1;
		// MI::createGuaranteedMaxMIParticles(player);
	}
}

void MagneticImpulseDisabled(Player &player) {
	MI::ImpulseData &midata = MI::impulseData[player.index];

	if(CurrentStage == BabylonGarden) {
		if(player.state == TurbulenceRide) {
			// compare turbulence index to player count to figure out if player is riding on big turb
			if(InGamePlayerCount == player.closestTurbulenceIndex) { return; }
		}
	}

	if(player.magneticImpulse_timer >= MI::MaximumCap) {
		if(player.placement != 0) {
			if(midata.maxMItimer < MI::MaxMITime) {
				MagneticImpulseEnabled(player);
				return;
			}
		}
	}

	if(midata.interrupt < MI::BufferTime) {
		midata.interrupt += 1;
		return;
	}

	midata.buffer = 0;
	midata.maxMItimer = 0;
	midata.magneticImpulse = false;

	player.magneticImpulse_soundStatus &= 0xC;
	if((player.magneticImpulse_soundStatus & 0x4) == 0) {
		player.magneticImpulse_soundStatus |= 0x4;
		if(!player.playerType) { PlayAudioFromDAT(Sound::SFX::MIDormant); }
	}

	if(player.magneticImpulse_timer > MI::LossTimeCap) {
		player.magneticImpulse_timer = MI::LossTimeCap;
	}

	if(player.magneticImpulse_timer > 0) {
		player.magneticImpulse_timer -= 1;
		return;
	}

	player.magneticImpulse_soundStatus &= 0xC;
	if((player.magneticImpulse_soundStatus & 0x8) == 0) {
		player.magneticImpulse_soundStatus |= 0x8;
		if(!player.playerType) { PlayAudioFromDAT(Sound::SFX::MIZero); }
	}
}

void Player_MagneticImpulseTimer(Player &player) {
	const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(player);
	HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player.index];
	if(hsInfo->hyperdriveEnabled && exLoads.gearExLoadID == HyperSonicEXLoad) {
		MI::createGuaranteedMaxMIParticles(player);
	}
	if(!DebugMenu_CheckOption(DebugMenuOptions::MagneticImpulse)) { return; }
	UpdatePlayerMatrices(player);// Kept in this spot to avoid changing existing behavior, Todo: check if this should stay
	if(CurrentGameMode != FreeRace && CurrentGameMode != WorldGrandPrix && CurrentGameMode != StoryMode) { return; }
	if(InGamePlayerCount < 2 && !DebugMenu_CheckOption(DebugMenuOptions::AlwaysMaxMI)) { return; }
	if(player.state == StartLine) { return; }
	if(DebugMenu_CheckOption(DebugMenuOptions::AlwaysMaxMI)) {
		player.magneticImpulse_timer = MI::MaximumCap;
		MagneticImpulseEnabled(player);
		return;
	}

	const Player &firstPlayer = getPlayerInFirst();

	if(player.otherPlayerDistance[firstPlayer.index] < 850 && firstPlayer.placement != player.placement) { return; }

	const bool turbulenceBool = player.state == TurbulenceRide;
	const bool &slipstreamBool = player.slipstream;

	if(!turbulenceBool && !slipstreamBool && player.placement != 0) {
		MagneticImpulseEnabled(player);
	} else {// MI off
		MagneticImpulseDisabled(player);
	}
}
