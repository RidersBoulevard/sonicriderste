#include "supermetal.hpp"

#include <tweaks/player/archetype/character_archetype.hpp>

#include "compare_many.hpp"
#include "containers/rgba.hpp"
#include "cosmetics/player/supersonicaura.hpp"
#include "handlers/player/voicelines.hpp"
#include "lib/sound.hpp"
#include "lib/stdlib.hpp"
#include "supers.hpp"

std::array<NeoMetalInfo, MaxPlayerCount> PlayerNeoMetalInfo;

constexpr GearLevelStats SuperMetal_SuperStats = {
        110000,    // max air
        0x14,      // air drain
        200,       // drift cost
        0x7530,    // boost cost
        0x61A8,    // tornado cost
        pSpeed(100), // drift dash speed
        pSpeed(245)// boost speed
};

constexpr GearLevelStats SuperMetal_NonSuperStats = {
        110000,    // max air
        0x14,      // air drain
        200,       // drift cost
        0x7530,    // boost cost
        0x61A8,    // tornado cost
        pSpeed(100), // drift dash speed
        pSpeed(235)// boost speed
};

inline void SuperMetal_UpdatePlayerStats(Player &player, const GearLevelStats &stats, f32 topSpeed) {
	player.gearStats[0].updateGearLevelStats(stats);
	player.gearStats[0].topSpeed = topSpeed;
	if(player.state == PlayerState::Cruise) { player.speedCap = topSpeed; }
}

inline void SuperMetal_PlayTransformationSound(const Player &player){
	if(!player.aiControl) { PlayAudioFromDAT(Sound::SFX::SuperTransformation); }// super transformation sfx
	bool scheck = false;
	for(const auto &curPlayer: getCurrentPlayerList()) {
		if(curPlayer.character == Character::Sonic) {
			scheck = true;
			break;
		}
	}
	if(scheck && lbl_RNG_Number(10) == 1) {
		Sound::PlaySound(player, 19);
	} else {
		constexpr auto TransformationStart = 13;
		constexpr auto TransformationLen = 6;
		Sound::PlaySound(player, lbl_RNG_Number(TransformationLen) + TransformationStart);
	}
}

void Player_SuperMetalTransformation(Player &player) {
	if(player.playerType) { return; }
	if(!player.isSuperCharacter(Character::MetalSonic)) { return; }
	if(player.gearExload().exLoadID == EXLoad::StardustSpeeder) { return; }
	if(player.flags.hasAny(PlayerFlags::InAPit)) { return; }// if in a pit

	if(player.rings >= 50 && player.superFormState == 0) {
		// transform
		player.superFormState = 1;
		// player.specialFlags ^= SpecialFlags::berserkerEffect;
		player.typeAttributes ^= Type::Fly;
		constexpr RGBA32 SuperNeoMetalHUDColor = 0xFFC400FF;
		SuperNeoMetalHUDColors[player.index] = SuperNeoMetalHUDColor;
		SuperMetal_UpdatePlayerStats(player, SuperMetal_SuperStats, pSpeed(230));
		SuperMetal_PlayTransformationSound(player);
		if(player.movementFlags.hasAny(MovementFlags::boosting)) { lbl_Player_BoostEndFunction(player); }
	}
	if(player.rings == 0 && player.superFormState != 0) {
		// untransform
		player.superFormState = 0;
		player.SuperMetalFrameCounter = 0;
		// player.specialFlags ^= SpecialFlags::berserkerEffect;
		player.typeAttributes ^= Type::Fly;
		constexpr RGBA32 NeoMetalHUDColor = 0x1239B8FF;
		SuperNeoMetalHUDColors[player.index] = NeoMetalHUDColor;
		SuperMetal_UpdatePlayerStats(player, SuperMetal_NonSuperStats, pSpeed(190));
	}
	if((player.state >= PlayerState::Cruise && player.state <= PlayerState::Jump) || player.state == PlayerState::Run) {// todo: check if this is correct
		if(player.superFormState != 0) {
			if(player.SuperMetalFrameCounter % 6 == 0) {
				player.rings -= 1;
			}
			player.SuperMetalFrameCounter++;
		}
	}
}



void Player_SuperMetalChaosControl(Player &player) {
	if(!isSuperCharacter(player, Character::MetalSonic)) { return; }
	if(player.gearExload().exLoadID != EXLoad::StardustSpeeder) { return; }
	NeoMetalInfo *neoInfo = &PlayerNeoMetalInfo[player.index];

	if (player.state == PlayerState::StartLine) {
		neoInfo->chaosControlState = 0; // 0 = inactive, 1 = active, 2 = burner state
		neoInfo->chaosDelayTimer = -1;
		neoInfo->chaosControlTimer = -1; // 5 seconds maximum, goes to -1 to inactive
		neoInfo->chaosBurnerTimer = -1; // depends on link amount
		neoInfo->objectLink1 = 0;
		neoInfo->objectLink2 = 0;
		neoInfo->trickCount = 0;
		neoInfo->attackCount = 0;
		neoInfo->totalChaosActionCount = 0;
		neoInfo->chaosBonus = 0;
		SuperMetal_UpdatePlayerStats(player, SuperMetal_StandardArch, pSpeed(205));
		player.currentAir = player.gearStats[player.level].maxAir;

	}

	bool hasChaosRequirement = (player.specialFlags.hasAny(SpecialFlags::ringGear)) ? player.currentAir >= 50000 : player.rings >= 50; // replace w/ meter later
	if (player.input->toggleFaceButtons.hasAny(Buttons::DPadUp) && neoInfo->chaosControlState == 0 && hasChaosRequirement) {
		// if (!player.specialFlags.hasAny(SpecialFlags::ringGear)) {
		// 	player.speed += player.rings > 50 ? pSpeed(static_cast<f32>(player.rings)) : 0;
		// 	// neoInfo->chaosBonus = (player.rings > 50) ? (player.rings / 10) : 0; // if player has more than 50 rings, store that bonus
		// }
		// else if () // add meter equivalent for ring gear
		neoInfo->chaosBonus = clamp(player.rings - 50, 0U);
		player.rings = 50;
		neoInfo->chaosDelayTimer = 60; // delay before full activation
		neoInfo->chaosControlTimer = 300; // give player 5 seconds
		neoInfo->chaosControlState = 1; // Active
		// player.typeAttributes = Type::All;
		player.typeAttributes ^= Type::Fly;
		PlayAudioFromDAT(Sound::SFX::HyperStream);
	}

	// Any of these states will make the timer(s) count down
	// PlayerState::Fall
	bool timerActive = if_any(player.state, std::is_eq, PlayerState::Cruise, PlayerState::Jump, PlayerState::TurbulenceRide,
			PlayerState::TurbulenceTrick, PlayerState::TurbulenceTrick2);

	if (timerActive && neoInfo->chaosControlState == 1 && neoInfo->chaosDelayTimer > 0) {
		// stop init from happening until timer is finished
		SuperSonicAuraCXX(player); // visible aura for this mode
		neoInfo->chaosDelayTimer -= 1;
		// return;
	} else if (timerActive && neoInfo->chaosControlState == 1 && neoInfo->chaosDelayTimer == 0) neoInfo->chaosDelayTimer = -1;

	if(neoInfo->chaosControlTimer > 0 && neoInfo->chaosControlState == 1) {
		// add everything while timer is active
		SuperSonicAuraCXX(player); // visible aura for this mode

		if (timerActive && neoInfo->chaosDelayTimer == -1 && neoInfo->chaosControlTimer % 6 == 0) {
			player.rings -= 1;
			player.rings = clamp(player.rings, 0U);
		} // visually display rings going down
		if (timerActive && neoInfo->chaosDelayTimer == -1) neoInfo->chaosControlTimer -= 1;

		if (player.input->toggleFaceButtons.hasAny(Buttons::DPadUp) && neoInfo->chaosControlTimer < 300 && neoInfo->chaosDelayTimer == -1) {
			PlayAudioFromDAT(Sound::SFX::MagnetShield);
			player.rings = 0;
			neoInfo->chaosControlTimer = 0;
		} // charge-cancel (omg GGXXAC+R Order Sol reference)

		// link counter
		if(neoInfo->objectLink1 < player.objectLinkCount) {
			neoInfo->objectLink1 = player.objectLinkCount;
		} else if(neoInfo->objectLink1 > player.objectLinkCount) //  && (player->objectLinkCount == 0)
		{
			neoInfo->objectLink2 += neoInfo->objectLink1;
			neoInfo->objectLink1 = 0;
		}

		// attack counter
		if (player.state == PlayerState::AttackingPlayer) neoInfo->attackCount += 1; // THIS ONLY WORKS FOR AOE RIGHT NOW. CHANGE FOR ACTUAL TETHERS LATER.

		// actually, try length of time
		if (player.state == PlayerState::RailGrind) neoInfo->totalChaosActionCount += 1; // per frame
		if (player.state == PlayerState::Fly) neoInfo->totalChaosActionCount += 1; // per frame

		// trick count is handled elsewhere
	}

	// Timer finishes in here. Add together everything and start distributing it.
	if(neoInfo->chaosControlTimer == 0) {

		if (neoInfo->chaosControlState == 1) {
			neoInfo->chaosControlState = 2;
			neoInfo->chaosDelayTimer = 60;
		}

		if (timerActive && neoInfo->chaosControlState == 2 && neoInfo->chaosDelayTimer > 0) {
			// stop init from happening until timer is finished
			SuperSonicAuraCXX(player); // visible aura for this mode
			neoInfo->chaosDelayTimer -= 1;
			return;
		} else if (timerActive && neoInfo->chaosControlState == 2 && neoInfo->chaosDelayTimer == 0) neoInfo->chaosDelayTimer = -1;
		PlayAudioFromDAT(Sound::SFX::SuperTransformation);

		neoInfo->chaosControlTimer = -1; // inactive timer
		neoInfo->chaosControlState = 2; // set to burner state after this
		// neoInfo->totalChaosActionCount = neoInfo->objectLink1 + neoInfo->objectLink2 + neoInfo->trickCount + neoInfo->attackCount;

		// take total time in shortcuts, add rings above 0, clamp to 10 seconds
		neoInfo->totalChaosActionCount = clamp((neoInfo->totalChaosActionCount + (neoInfo->chaosBonus * 6)), 0U, 600U); // clamp to 600 frames (10 seconds)
		player.rings = clamp((neoInfo->totalChaosActionCount / 6), 0U, 100U);
		//+ neoInfo->chaosBonus; // links/tricks/attacks

		// neoInfo->chaosBurnerTimer = (static_cast<s32>(neoInfo->totalChaosActionCount)); // "This is where the fun begins."
		// neoInfo->chaosBurnerTimer = static_cast<s32>(player.rings) / 10; // "This is where the fun begins."
		neoInfo->chaosBurnerTimer = static_cast<s32>(neoInfo->totalChaosActionCount); // "This is where the fun begins."
		// neoInfo->chaosBurnerTimer *= 60; // to make sure it's in seconds
		// neoInfo->chaosBurnerTimer += 60; // extra second to support
		neoInfo->objectLink1 = 0;
		neoInfo->objectLink2 = 0;
		neoInfo->trickCount = 0;
		neoInfo->attackCount = 0;
		neoInfo->chaosBonus = 0;

		player.superFormState = 1;
	}

	// if(neoInfo->chaosBurnerTimer > 0 && neoInfo->chaosControlState == 2) {
	// 	// add everything while timer is active
	// 	if(timerActive && neoInfo->chaosBurnerTimer % 6 == 0) player.rings -= 1; // visually display rings going down
	// 	// power link check
	// 	// if(player.state == PlayerState::Cruise && player.unkD90 >= 90) player.rings += 1;
	// 	//
	// 	// if(player.movementFlags.hasAny(MovementFlags::railLink)) player.rings += 1; // irregular check because of frames, replace.
	// 	//
	// 	// // attack counter
	// 	// if (player.state == PlayerState::AttackingPlayer) player.rings += 1; // THIS ONLY WORKS FOR AOE RIGHT NOW. CHANGE FOR ACTUAL TETHERS LATER.
	//
	// 	// trick count is handled elsewhere
	// }

	if (neoInfo->chaosBurnerTimer > 0 && neoInfo->chaosControlState == 2 && timerActive) {
		if(neoInfo->chaosBurnerTimer % 6 == 0) {
			player.rings -= 1;
			player.rings = clamp(player.rings, 0U);
		}
		neoInfo->chaosBurnerTimer -= 1;
	} else if (neoInfo->chaosBurnerTimer == 0) {
		neoInfo->chaosBurnerTimer = -1; // inactive timer
		PlayAudioFromDAT(Sound::SFX::EmerlLoseType);
		neoInfo->chaosControlState = 0;
		player.superFormState = 0;
		player.typeAttributes ^= Type::Fly;
		neoInfo->totalChaosActionCount = 0;
		// player.typeAttributes = Type::Speed | Type::Power;
	} // reset back to neutral


}

void Player_SuperMetalIII(Player &player) {
	if(!player.isSuperCharacter(Character::MetalSonic)) { return; }
	if(player.gearExload().exLoadID != EXLoad::StardustSpeeder) { return; }
	NeoMetalInfo *neoInfo = &PlayerNeoMetalInfo[player.index];

	// "Steal" natural type of opponent
	if (player.state == PlayerState::StartLine) {
		neoInfo->chaosDelayTimer = -1;
		if (InGamePlayerCount >= 2) {
		    // Create a vector of valid players we can steal type from. This way, we only pick from the players that aren't the current one.
		    // MAXIMUM OF 2 (3P) / 3 (4P) ITEMS IN THIS LIST
		    std::vector<u8> randIndexList = {};
		    for (vu32 idx = 0; idx < InGamePlayerCount; idx++) {
		        if (idx != player.index) {
		            randIndexList.push_back(idx);
		        }
		    }
		    // Get a random index to steal a type if more than 2 players (randomize from list generated earlier). If not, just get alternate player's type.
		    // List rand indexing max 0-1 (3P) / 0-2 (4P)
			const u8 randIndex = (InGamePlayerCount > 2) ? randIndexList[lbl_RNG_Number(InGamePlayerCount - 1)] : 1;
		    randIndexList.clear(); // clear to save mem
		    randIndexList.shrink_to_fit(); // shrink to save mem

		    // Do index calc here and get the natural type. If greater than 2 players, get random one. If just 2, get alternate type here.
			Type stolenType = (InGamePlayerCount > 2)
			? toGearType(players[randIndex].characterptr->type)
			: toGearType(players[player.index ^ randIndex].characterptr->type);

			if (stolenType == static_cast<Type>(0x8)) stolenType = Type::Power; // Gets power from no-types
			neoInfo->stolenType = stolenType; // Store alternate type
		} else if(player.input->toggleFaceButtons.hasAny(Buttons::Z)) {
			// Press Z button before starting the race to rotate types in single player
			if (neoInfo->stolenType == Type::None) neoInfo->stolenType = Type::Power; // Spoof to make this work because typing be dumb
			Flag newType = neoInfo->stolenType << static_cast<Type>(1);// rotate to next type
			if(newType > Type::Power) { newType = Type::Speed; }                // reset to speed type
			if(newType == Type::Speed) PlayAudioFromDAT(Sound::SFX::AdvPTOP1);
			if(newType == Type::Fly) PlayAudioFromDAT(Sound::SFX::AdvPTOP2);
			if(newType == Type::Power) PlayAudioFromDAT(Sound::SFX::AdvPTOP3);
			neoInfo->stolenType = newType; // Store alternate type
		}

		SuperMetal_UpdatePlayerStats(player, SuperMetal_StandardArch, pSpeed(190)); // Standard stats
		player.typeAttributes = toGearType(player.characterptr->type); // Force speed type in base state
		if (neoInfo->stolenType != Type::Speed) player.typeAttributes &= ~neoInfo->stolenType;
		// player.currentAir = player.gearStats[player.level].maxAir;
		return; // Don't run threshold tasks until off the startline
	}

	// Handler for "Threshold" (mechanic)
	const bool thresholdCheck = player.currentAir >= (player.gearStats[player.level].maxAir * static_cast<s32>(100 - player.rings)) / 100
	&& player.rings > 0;

	const bool stateCheck = if_any(player.state, std::is_eq, PlayerState::Cruise, PlayerState::Jump, PlayerState::Fall);

	SuperSonicAuraCXX(player); // visible aura for this mode

	if (player.input->toggleFaceButtons.hasAny(Buttons::X)) {
		if (neoInfo->formState == 2) {
			// Should only happen in super form to swap back to Burn state
			neoInfo->formState = 1; // Burn state
		} else neoInfo->formState ^= 1; // 0 or 1
		if ((player.state == PlayerState::Fly || (player.unkD90 > 60 && player.state == PlayerState::Cruise)) && neoInfo->formState == 0) {
			if (thresholdCheck) neoInfo->formState = 2;
			else neoInfo->formState = 1;
		} else PlayAudioFromDAT(Sound::SFX::AdvPTOP3);
	}

	// Delay for losing super form
	if (neoInfo->chaosDelayTimer > 0) {
		if (player.state == PlayerState::Cruise && player.unkD90 >= 60) neoInfo->chaosDelayTimer = 60; // Refreshes timer for power objects
		neoInfo->chaosDelayTimer -= 1;
		if (thresholdCheck) {
			PlayAudioFromDAT(Sound::SFX::AdvPTOP1);
			neoInfo->chaosDelayTimer = -1;
		}
	} else if (neoInfo->chaosDelayTimer == 0 && stateCheck) {
		neoInfo->chaosDelayTimer = -1;
		neoInfo->formState = 0;
		player.superFormState = 0; // Turn super off
		PlayAudioFromDAT(Sound::SFX::EmerlLoseType);
	}

	// Handles swapping states and stats
	s32 newRings = static_cast<s32>(player.rings);
	switch (neoInfo->formState) {
		case 0:
		// Standard
			if (thresholdCheck) {
				neoInfo->formState = 2;
				break;
			}
			if(player.SuperMetalFrameCounter % 60 == 0 && stateCheck) {
				newRings += 1; // Gain rings
			}
			neoInfo->formState = 0;
			player.superFormState = 0; // Turn super off
			player.characterArchetype = CharacterArchetype::Boost;
			player.typeAttributes = toGearType(player.characterptr->type); // Force speed type in base state
			if (neoInfo->stolenType != Type::Speed) player.typeAttributes &= ~neoInfo->stolenType;
			if (if_all(player.state, std::is_neq, PlayerState::RailGrind, PlayerState::Fly)) player.shortcutAirGainMultiplier = 0.75f;
			player.trickAirGainMultiplier = 0.85f;
			if (player.movementFlags.hasAny(MovementFlags::boosting)) break;
			SuperMetal_UpdatePlayerStats(player, SuperMetal_StandardArch, pSpeed(190)); // Standard stats
			break;
		case 1:
		// Burn state
			if (neoInfo->chaosDelayTimer > 0) neoInfo->chaosDelayTimer = -1;
			if(player.SuperMetalFrameCounter % 12 == 0 && stateCheck) {
				newRings -= 1; // Drain rings
			}
			neoInfo->formState = 1;
			player.superFormState = 0; // Turn super off
			player.characterArchetype = CharacterArchetype::AllRounder;
			player.typeAttributes = neoInfo->stolenType; // Swap to alternate type
			if (neoInfo->stolenType != Type::Speed) player.typeAttributes &= ~toGearType(player.characterptr->type);
			if (if_all(player.state, std::is_neq, PlayerState::RailGrind, PlayerState::Fly)) player.shortcutAirGainMultiplier = 1.0f;
			player.trickAirGainMultiplier = 0.85f;
			// if (player.movementFlags.hasAny(MovementFlags::boosting)) break;
			SuperMetal_UpdatePlayerStats(player, SuperMetal_BurnState, pSpeed(180)); // Burn state stats
			break;
		case 2:
		// Super state
			if (!thresholdCheck && neoInfo->chaosDelayTimer == -1) {
				neoInfo->chaosDelayTimer = 60; // delay before full activation
				PlayAudioFromDAT(Sound::SFX::AdvPTOP2);
			}
			if(player.SuperMetalFrameCounter % 90 == 0 && stateCheck) {
				newRings += 1; // Gain rings
			}
			if (player.superFormState != 1) SuperMetal_PlayTransformationSound(player);
			player.superFormState = 1; // Turn super on
			neoInfo->formState = 2;
			player.characterArchetype = CharacterArchetype::None;
			player.typeAttributes = (Type::Speed | neoInfo->stolenType);
			// Give stats (Higher air drain, higher stats though)
			player.shortcutAirGainMultiplier = 0.5f;
			player.trickAirGainMultiplier = 0.85f;
			if (player.movementFlags.hasAny(MovementFlags::boosting)) break;
			SuperMetal_UpdatePlayerStats(player, SuperMetal_SuperStatsII, pSpeed(210)); // Super stats
			break;
		default:
			break;
		}
		newRings = clamp(newRings, 0, 100);
		player.rings = static_cast<u32>(newRings);
		player.SuperMetalFrameCounter++; // Handles frame counter for all states
}
