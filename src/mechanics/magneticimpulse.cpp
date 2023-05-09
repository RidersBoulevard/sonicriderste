#include <ppu_intrinsics.h>
#include "mechanics/magneticimpulse.hpp"
#include "mechanics/dash/grindraildash.hpp"
#include "mechanics/dash/flyhoopdash.hpp"
#include "lib/sound.hpp"
#include "lib/lib.hpp"
#include "cosmetics/player/exloads.hpp"
#include "gears/hypersonic.hpp"
//#include "gears/hyperhangon.hpp"
//#include "gears/olliekinggear.hpp"
//#include "gears/eggsterminator.hpp"
#include "handlers/menu/debugmenu/debugmenu.hpp"

MagneticImpulseData magneticImpulseData[8];
Matrix3x3 PlayerPositionRotationMatrices[8];

void MagneticImpulse_PlayMaxMILine(const Player &player){
	u32 usedSlots = 0;
	const u8 &character = player.character;

	while(usedSlots < MIVoiceLinesPerChar){ // Get how many voicelines this character actually has
		if(MI_VoiceLineIDs[character][usedSlots] == 0) break;
		usedSlots++;
	}
	if(usedSlots == 0) return; // Character has no lines to play

	u32 selectedLine = lbl_RNG_Number(usedSlots);
	if(isSuperCharacter(player, MetalSonic)) selectedLine += 0x03; // Select Neo Metal's lines
	Sound::PlaySound(player, MI_VoiceLineIDs[character][selectedLine]);
}

void MagneticImpulse_LevelUpStars(const Player &player){
	auto *particles = reinterpret_cast<LevelUpStarsObject1 *>(SetTask(func_Particle_Task, 0xB3B0, 2)->object);
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

USED f32 MagneticImpulse_ParticleSpeed(Player *player){
	return 0.0505316f + (player->magneticImpulse_timer * 0.00012632867f);
}

USED void MagneticImpulse_AttackParticles(Player *player, u8 battleModeData[], u32 offset){
	MagneticImpulseData *magneto = &magneticImpulseData[player->index];
	u8 particleState;
	if(CurrentGameMode == FreeRace || CurrentGameMode == WorldGrandPrix || CurrentGameMode == StoryMode){
		if(!magneto->magneticImpulse){
			particleState = 1;
		}else if(player->magneticImpulse_timer < MI_MaximumCap){
			particleState = 2;
		}else{
			particleState = 3;
		}

		battleModeData[offset] = particleState;
	}
}

USED void MagneticImpulse_GuardParticles(Player *player, u8 battleModeData[], u32 offset){
	MagneticImpulseData *magneto = &magneticImpulseData[player->index];
	u8 particleState;
	if(CurrentGameMode == FreeRace || CurrentGameMode == WorldGrandPrix || CurrentGameMode == StoryMode){
		if(!magneto->magneticImpulse){
			if(player->magneticImpulse_timer == 0){
				particleState = 0;
			}else{
				particleState = 1;
			}
		}else{
			if(player->magneticImpulse_timer < MI_MaximumCap){
				particleState = 0;
			}else{
				particleState = 2;
			}
		}

		battleModeData[offset] = particleState;
	}
}

USED f32 Player_MISpeedAirGain(Player *player, f32 airGain){
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);// for grind rails

	if (player->extremeGear == CoverS && player->state == RailGrind)
    {
        if (player->characterptr->type == 0x0) 
        {
            airGain *= 0.5f;
        }
        else if ((player->characterptr->type == 0x1 || player->characterptr->type == 0x2))
        {
            airGain *= 0.30f;
        }
    }

	if((&magneticImpulseData[player->index])->magneticImpulse){
		airGain += player->magneticImpulse_timer / 3.0f;
	}

	if(player->grindRailDash == 1){
		if(player->specialFlags & ringGear){
			if(player->character == SuperSonic
			   && player->shortcutAirGainMultiplier > 0
			   && exLoads.gearExLoadID != HyperSonicEXLoad){
				s32 newBlastGaugeValue = player->shortcutAirGainMultiplier + static_cast<s32>(GrindRailDashAirCosts[1]);
				player->shortcutAirGainMultiplier = clamp(newBlastGaugeValue, 0);
			}else{
				s32 newAir = s32(player->currentAir - (player->gearStats[player->level].maxAir * 0.05) / 100);
				player->currentAir = clamp(newAir, 0);
			}
		}else{
			airGain = GrindRailDashAirCosts[player->level];
		}

	}
	if(exLoads.gearExLoadID == TheBeastEXLoad) return 0;
	return airGain;
}

f32 Player_MIFlyAirGain(Player *player, f32 airGain){
	// for fly hoops
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
	// PlayerFlyHoopDashProperties playerFlyHoopDashProperties[8];
	PlayerFlyHoopDashProperties *properties = &playerFlyHoopDashProperties[player->index];
	s32 newBlastGaugeValue;

	if(player->extremeGear == CoverS && player->state == Fly) {
		if(player->characterptr->type == 0x1) {
			airGain *= 0.5f;
		}
		else if((player->characterptr->type == 0x2 || player->characterptr->type == 0x0)) {
			airGain *= 0.30f;
		}
	}

	if((&magneticImpulseData[player->index])->magneticImpulse){
		airGain += player->magneticImpulse_timer / 3.0f;
	}

	// This involves the new hoop dash V2.
	if (properties->hasUsed == TRUE) {
		s32 newAir;
		// This first if-statement is for ring gears
		if (player->specialFlags & ringGear){
				if(player->character == SuperSonic
					   && player->shortcutAirGainMultiplier > 0
					   && exLoads.gearExLoadID != HyperSonicEXLoad){
						// See the air section below, same logic
						player->shortcutAirGainMultiplier -= FlyHoopDashInitialCosts[player->level];
						if(player->shortcutAirGainMultiplier < 0) player->shortcutAirGainMultiplier = 0;
				}else{
					// This statement removes rings from ring gears. Change the "5" to change the
					// amount of rings taken
					newAir = player->currentAir - (player->gearStats[player->level].maxAir * 5) / 100;
					player->currentAir = clamp(newAir, 0);
				}
		} else {
			// This statement removes air from the player. Change the costs in "FlyHoopDashInitialCosts[player->level]"
			// (in flyhoopdash.cpp) to alter how much air is taken
			newAir = player->currentAir - FlyHoopDashInitialCosts[player->level];
			player->currentAir = clamp(newAir, 0);
		}
			// properties->duration = 0;
			properties->hasUsed = FALSE;
	}

	// Handles the delay for new fly hoop dash. Subtracts 1 frame from the timer every frame and pauses air gain.
	if (properties->gainDelay > 0.0f) {
		properties->gainDelay -= 1.0f;
		return 0;
	}

	if(player->flyHoopDash == 1){
		if(player->specialFlags & ringGear){
			if(player->character == SuperSonic
			   && player->shortcutAirGainMultiplier > 0
			   && exLoads.gearExLoadID != HyperSonicEXLoad){
				newBlastGaugeValue = player->shortcutAirGainMultiplier + static_cast<s32>(FlyHoopDashAirLoss[1]);
				// player->shortcutAirGainMultiplier -= FlyHoopDashAirLoss[player->level];
				if(newBlastGaugeValue < 0) newBlastGaugeValue = 0;
				player->shortcutAirGainMultiplier = newBlastGaugeValue;
			}else{
				s32 newAir = player->currentAir - (player->gearStats[player->level].maxAir * 0.05) / 100;
				player->currentAir = clamp(newAir, 0);
			}
		}else{
			airGain = FlyHoopDashAirLoss[player->level];
		}

	}
	if(exLoads.gearExLoadID == TheBeastEXLoad) return 0;
	return airGain;
}

USED f32 Player_MIPowerObjectAirGain(Player *player, f32 airGain){
	// for power objects
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
	s32 newBlastGaugeValue;
	struct HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player->index];

	if(player->extremeGear == CoverS){
		if(player->characterptr->type == 0x2){
			airGain *= 0.5f;
		}else if(player->characterptr->type == 0x0 || player->characterptr->type == 0x1){
			airGain *= 0.30f;
		}
	}

	if(player->character == SuperSonic
	   && player->shortcutAirGainMultiplier > 0
	   && exLoads.gearExLoadID != HyperSonicEXLoad
	   && player->movementFlags & boosting) // if blast gauge is available, subtract half a trick
	{
		newBlastGaugeValue = player->shortcutAirGainMultiplier - 10000;
		if(newBlastGaugeValue < 0) newBlastGaugeValue = 0;
		player->shortcutAirGainMultiplier = newBlastGaugeValue;
	}

	if(exLoads.gearExLoadID == HyperSonicEXLoad && hsInfo->hyperdriveEnabled == TRUE){
		player->shortcutAirGainMultiplier += 1700;
	}

	if((&magneticImpulseData[player->index])->magneticImpulse){
		airGain += player->magneticImpulse_timer * 5.0f;
	}
	if(exLoads.gearExLoadID == TheBeastEXLoad) return 0;
	return airGain;
}

USED f32 Player_MITrickAirGain(Player *player, f32 airGain){
	// struct HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];

	if(player->isBlastGaugeGear == TRUE){ // for all blast gear gauges, use this when they land their tricks
		HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[player->index];
		EnabledEXLoads exLoads;
		FetchEnabledEXLoadIDs(player, exLoads);
		if(exLoads.gearExLoadID == HyperSonicEXLoad && hsInfo->hyperdriveEnabled){
			//player->shortcutAirGainMultiplier += player->trickCount * 0;
		//}else if(exLoads.gearExLoadID == HyperSonicEXLoad && !hsInfo->hyperdriveEnabled){
		//	player->shortcutAirGainMultiplier += player->trickCount * 10000;
		//}else if(exLoads.gearExLoadID != HyperSonicEXLoad && player->character == SuperSonic){
		//	player->shortcutAirGainMultiplier += player->trickCount * 10000;
		}else{
			player->shortcutAirGainMultiplier += player->trickCount * 10000;
		}
		if(player->shortcutAirGainMultiplier >= 200000) player->shortcutAirGainMultiplier = 200000;
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

	if((&magneticImpulseData[player->index])->magneticImpulse){
		f32 MIpercentage = player->magneticImpulse_timer / MI_MaximumCap;
		f32 MIair = MIpercentage * 50000.0f;
		if(player->extremeGear == TheCrazy) MIair *= 0.2f;
		if(player->extremeGear == AdvantageF) MIair *= 0.4f;
		if(!(player->typeAttributes & AllType)) MIair *= MI_NoTypeMultiplier;

		airGain += MIair;
	}
	return airGain;
}

USED void Player_MIPitAirGain(Player *player){
	f32 airGain, MIpercentage;

	if((&magneticImpulseData[player->index])->magneticImpulse){
		MIpercentage = player->magneticImpulse_timer / MI_MaximumCap;
		airGain = MIpercentage * 500.0f;
		if(!(player->typeAttributes & AllType)) airGain *= MI_NoTypeMultiplier;

		player->changeInAir_gain += static_cast<s32>(airGain);
	}
}

USED f32 Player_MIPitSpeed(Player *player, f32 pitSpeed){
	//EnabledEXLoads exLoads;
	//FetchEnabledEXLoadIDs(player, exLoads);
	// struct HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];
	// struct EggInfo *EggInfo = &PlayerEggsterminator[player->index];

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
	if(CurrentGameMode == FreeRace || CurrentGameMode == WorldGrandPrix || CurrentGameMode == StoryMode){
		if((&magneticImpulseData[player->index])->magneticImpulse){
			f32 addSpeed = (player->magneticImpulse_timer / MI_MaximumCap) * pSpeed(100.0f);
			if(!(player->typeAttributes & AllType)) addSpeed *= MI_NoTypeMultiplier;
			pitSpeed += addSpeed;
		}
	}
	return pitSpeed;
}

USED f32 Player_MITrickLandingSpeed(Player *player, f32 currentTrickSpeed){
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
	// struct OKGInfo *okgInfo = &PlayerOKGInfo[player->index];

	if (player->trickRank > 0x5 && player->extremeGear != Legend && player->characterArchetype != TricksterArchetype) { // S and C rank
		currentTrickSpeed += ((f32)(player->trickCount - (u8)4) * pSpeed(2.0f));
	}

	if(player->extremeGear == Legend || exLoads.gearExLoadID == TheProfessionalEXLoad){
		if(player->rampType != 0x3){ // half pipe
			if(player->trickRank != SRank && player->trickRank != CRank){ // S and C rank
				currentTrickSpeed = pSpeed(100.0f) + (static_cast<f32>(player->trickCount) * pSpeed(35.0f));
			}
		}
	}

	if(player->rampType == 0x3){
		currentTrickSpeed += pSpeed(100.0f);
		if((player->typeAttributes & AllType) == 0){
			// no type
			currentTrickSpeed += pSpeed(50.0f);
		}
	}

	if(player->characterArchetype == TricksterArchetype){
		currentTrickSpeed += pSpeed(50.0f);
		if (player->trickRank >= SPlusRank) 
		{
			magneticImpulseData[player->index].afterburnerTimer = (u32)((player->trickCount - (u8)1) * 60);
		}
	}

	if((&magneticImpulseData[player->index])->magneticImpulse){
		f32 MIspeed = (player->magneticImpulse_timer / MI_MaximumCap) * pSpeed(50.0f);
		if(!(player->typeAttributes & AllType)) MIspeed *= MI_NoTypeMultiplier;
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

USED s16 Player_MIBabylonGardenMiniSpiralQTE(Player *player, void *qte){
	s16 qteSpeed = *(reinterpret_cast<s16 *>(qte) + 0x20 / 2);
	f32 additiveSpeed, MIpercentage;

	if(CurrentStage == BabylonGarden){
		qteSpeed = 250;

		if((&magneticImpulseData[player->index])->magneticImpulse){
			MIpercentage = player->magneticImpulse_timer / MI_MaximumCap;
			additiveSpeed = 50.0f * MIpercentage;
			if(!(player->typeAttributes & AllType)) additiveSpeed *= MI_NoTypeMultiplier;
			qteSpeed += static_cast<s16>(additiveSpeed);
		}
	}

	return qteSpeed;
}

USED s32 Player_MIRingBoxGain(Player *player){
	s32 extraRings = 0;
	if((&magneticImpulseData[player->index])->magneticImpulse){
		extraRings = static_cast<s16>((player->magneticImpulse_timer / MI_MaximumCap) * 2.0f);
	}

	return extraRings;
}

USED f32 Player_MIRingDrain(Player *player, f32 ringDrain){
	f32 ringDrainInvert, multiplier;

	if(player->extremeGear == SuperHangOn && player->level4){
		ringDrain *= 1.3f;
	}else if(magneticImpulseData[player->index].magneticImpulse){
		if(player->specialFlags & ringGear){
			if(ringDrain < 0){
				ringDrainInvert = 1.0f;
			}else{
				ringDrainInvert = -1.0f;
			}

			multiplier = __fabsf(ringDrainInvert + (player->magneticImpulse_timer /
			                                        MI_MaximumCap
			)); // NOLINT(cppcoreguidelines-narrowing-conversions)
			ringDrain *= multiplier;
		}
	} else if (magneticImpulseData[player->index].ringPowerObjTimer > 0.0f) 
	{
		ringDrain = -1.0f;
		magneticImpulseData[player->index].ringPowerObjTimer -= 1.0f;
	}
	
	return ringDrain;
}

USED f32 MagneticImpulse_CalculateMultiplier(Player *player, f32 value){

	if(player->typeAmount == 2){
		value *= MI_DualTypeMultiplier;
	}else if(player->typeAmount == 3){
		value *= MI_AllTypeMultiplier;
	}

	return value;
}

f32 ScaleUsingCurrentMI(Player &player, f32 value) {
    if((&magneticImpulseData[player.index])->magneticImpulse) {
        f32 MIpercentage = player.magneticImpulse_timer / MI_MaximumCap;
        return value * MIpercentage;
    }

    return 0.0f;
}

USED void Player_MISpeedShoes(Player *player){
	f32 extraSpeed = 0.0f;

	if((&magneticImpulseData[player->index])->magneticImpulse){
		extraSpeed = (player->magneticImpulse_timer / MI_MaximumCap) * pSpeed(100);
	}
	if(!(player->typeAttributes & AllType)) extraSpeed *= MI_NoTypeMultiplier;

	player->speed += pSpeed(100) + extraSpeed;
}

USED void Player_MIAirBoxGain(Player *player, s32 airToAdd){
	f32 multiplier = 1.0f;

	if((&magneticImpulseData[player->index])->magneticImpulse){
		if(!(player->typeAttributes & AllType)){
			multiplier += (player->magneticImpulse_timer / MI_MaximumCap) * 0.6f;
		}else{ multiplier += (player->magneticImpulse_timer / MI_MaximumCap) * 0.3f; }
	}

	airToAdd *= multiplier;
	player->changeInAir_gain += airToAdd;
}

USED void MagneticImpulse_ClearData(){
	TRK_memset(&magneticImpulseData, 0, sizeof(magneticImpulseData));
}

void MagneticImpulse_CreateGuaranteedMaxMIParticles(Player &player) {
    extern void* lbl_001F9C30;
    if (gu32GameCnt % 3 != 0) return;

    auto *particles = reinterpret_cast<ParticleTaskObject1 *>(SetTask(func_Particle_Task, 0xB3B0, 2)->object);
    particles->unk72 = 4;
    particles->unk0 = 0;
    particles->unk4 = 0;
    particles->unk8 = 0;
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
    particles->unk6C = &PlayerPositionRotationMatrices[player.index];
    particles->unk73 = -1;
    particles->unk74 = 0;
}

inline void UpdatePlayerMatrices(Player &player) {
    GetPlayerPositionRotationMatrix(player, PlayerPositionRotationMatrices[player.index]);
}

void Player_MagneticImpulseTimer(Player &player){
	MagneticImpulseData *midata = &magneticImpulseData[player.index];
	Player *firstplayer = players.data(); // todo: change into index
	u32 turbulenceBool, slipstreamBool, playerCount;
	u32 i;
	f32 distance;
	playerCount = InGamePlayerCount;

	if(!DebugMenu_CheckOption(MagneticImpulse)) return;

    UpdatePlayerMatrices(player);

	for(i = 0; i < playerCount; i++){
		if(firstplayer->placement == 0){
			break;
		}
		firstplayer++;
	}

	distance = player.otherPlayerDistance[firstplayer->index];

	if(CurrentGameMode != FreeRace && CurrentGameMode != WorldGrandPrix && CurrentGameMode != StoryMode){ return; }
	if(playerCount < 2){ return; }
	if(player.state == StartLine){ return; }
	if(distance < 850 && firstplayer->placement != player.placement){ return; }

	turbulenceBool = (player.state == TurbulenceRide) ? TRUE : FALSE;
	slipstreamBool = (player.slipstream) ? TRUE : FALSE;

	if(!turbulenceBool && !slipstreamBool && player.placement != 0){
		MI_ON:
		if(player.state == QTE){ return; }
		midata->interrupt = 0;
		if(midata->buffer < MI_BufferTime){
			midata->buffer += 1;
			return;
		}

		midata->magneticImpulse = TRUE;
		player.magneticImpulse_soundStatus &= 0x3; // clear out mi dormant and reaches zero status
		if(!(player.magneticImpulse_soundStatus & 0x2)) // check for mi dormant sfx status
		{
			player.magneticImpulse_soundStatus |= 0x2; // set mi dormant status
			if(!player.playerType) PlayAudioFromDAT(Sound::SFX::MIActive);
		}

		if(player.magneticImpulse_timer < MI_MaximumCap){
			player.magneticImpulse_timer += 1;
			return;
		}

		player.magneticImpulse_soundStatus &= 0x3;
		if(!(player.magneticImpulse_soundStatus & 0x1)){
			// should be at max MI here
			player.magneticImpulse_soundStatus |= 0x1;
			if(!player.playerType){
				PlayAudioFromDAT(Sound::SFX::MaximumMI);
				MagneticImpulse_PlayMaxMILine(player);
			}
			MagneticImpulse_LevelUpStars(player);
		}

		if(midata->maxMItimer < MI_MaxMITime){
			midata->maxMItimer += 1;
            MagneticImpulse_CreateGuaranteedMaxMIParticles(player);
		}
	}else{
		// MI off
		if(CurrentStage == BabylonGarden){
			if(turbulenceBool){
				// compare turbulence index to player count to figure out if player is riding on big turb
				if(playerCount == player.closestTurbulenceIndex) return;
			}
		}

		if(player.magneticImpulse_timer >= MI_MaximumCap){
			if(player.placement != 0){
				if(midata->maxMItimer < MI_MaxMITime){
					goto MI_ON;
				}
			}
		}

		if(midata->interrupt < MI_BufferTime){
			midata->interrupt += 1;
			return;
		}

		midata->buffer = 0;
		midata->maxMItimer = 0;
		midata->magneticImpulse = FALSE;

		player.magneticImpulse_soundStatus &= 0xC;
		if(!(player.magneticImpulse_soundStatus & 0x4)){
			player.magneticImpulse_soundStatus |= 0x4;
			if(!player.playerType) PlayAudioFromDAT(Sound::SFX::MIDormant);
		}

		if(player.magneticImpulse_timer > MI_LossTimeCap){
			player.magneticImpulse_timer = MI_LossTimeCap;
		}

		if(player.magneticImpulse_timer > 0){
			player.magneticImpulse_timer -= 1;
			return;
		}

		player.magneticImpulse_soundStatus &= 0xC;
		if(!(player.magneticImpulse_soundStatus & 0x8)){
			player.magneticImpulse_soundStatus |= 0x8;
			if(!player.playerType) PlayAudioFromDAT(Sound::SFX::MIZero);
		}
	}
}
