#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"
#include "lib/stdlib.hpp"
#include "mechanics/characters/gizoidreplication.hpp"
#include "mechanics/dash/flyhoopdash.hpp"
#include "mechanics/magneticimpulse.hpp"
#include "riders/object.hpp"
#include "riders/stage.hpp"

constexpr std::array<f32, TotalStageAmount> StageBaseSpeeds = {
		0.0f,      // test stage
		0.231482f, // metal city
		0.0f,      // splash canyon
		0.925926f, // egg factory
		0.347222f, // green cave
		0.0462963f,// sand ruins
		0.185185f, // babylon garden
		0.324074f, // digital dimension
		0.509306f, // sega carnival
		1.85189f,  // night chase
		0.0925926f,// red canyon
		0.972222f, // ice factory
		0.0462963f,// white cave
		0.0f,      // dark desert
		0.0f,      // sky road
		0.925926f, // babylon guardian
		0.0925927f,// sega illusion
};

constexpr std::array<f32, TotalStageAmount> SpecificFlyHoopSpeeds = {
		0.0f,      // test stage
		0.462963f, // metal city
		0.0f,      // splash canyon
		-0.462963f,// egg factory
		0.0f,      // green cave
		0.0f,      // sand ruins
		0.0f,      // babylon garden
		0.0f,      // digital dimension
		0.0f,      // sega carnival
		0.0f,      // night chase
		0.0f,      // red canyon
		0.0f,      // ice factory
		0.185187f, // white cave
		0.0f,      // dark desert
		0.0f,      // sky road
		0.0f,      // babylon guardian
		0.833334f, // sega illusion
};

constexpr f32 BaseFlyHoopVerticalSpeeds[TotalStageAmount] = {
		0.05f,// test stage
		0.35f,// metal city
		0.1f, // splash canyon
		0.5f, // egg factory
		0.4f, // green cave
		0.1f, // sand ruins
		0.25f,// babylon garden
		0.2f, // digital dimension
		0.05f,// sega carnival
		0.05f,// night chase
		0.05f,// red canyon
		0.4f, // ice factory
		0.05f,// white cave
		0.05f,// dark desert
		0.05f,// sky road
		0.05f,// babylon guardian
		0.45f,// sega illusion
};

ASMUsed f32 CustomFlyHoopVerticalSpeeds(ObjectNode *object, Player *player, f32 flyHoopSpeed, f32 currentSpeed, f32 angle) {
	f32 multiplier = BaseFlyHoopVerticalSpeeds[CurrentStage];
	switch(CurrentStage) {
		case MetalCity:
			switch(object->object_id) {
				case 0x121:
					multiplier = 0.1f;
					break;
				case 0x124:
					multiplier = 0.15f;
					break;
				default:
					break;
			}
			break;

		case GreenCave:
			switch(object->object_id) {
				case 0xEB:
					multiplier = -0.1f;
					break;
				default:
					break;
			}
			break;

		case SandRuins:
			switch(object->object_id) {
				case 0x136:
					multiplier = -0.8f;
					break;
				case 0x138:
					multiplier = 1.5f;
					break;
				default:
					break;
			}
			break;

		case NightChase:
			switch(object->object_id) {
				case 0x26:
					multiplier = 0.4f;
					break;
				case 0x27:
					multiplier = 0.2f;
					break;
				default:
					break;
			}
			break;

		case IceFactory:
			switch(object->object_id) {
				case 0x16E:
				case 0x16F:
					multiplier = 0.2f;
					break;
				default:
					break;
			}
			break;

		case SkyRoad:
			switch(object->object_id) {
				case 0xD4:
					multiplier = -0.5f;
					break;
				case 0xD5:
					multiplier = 0.5f;
					break;
				case 0xD6:
					multiplier = 0.6f;
					break;
				default:
					break;
			}
			break;

		case SegaCarnival:
			switch(object->object_id) {
				case 0x5F:
				case 0x60:
					multiplier = 0.4f;
					break;
				case 0x61:
					multiplier = 0.2f;
					break;
				default:
					break;
			}
			break;
		case SplashCanyon:
			switch(object->object_id) {
				case 0xA0:// first hoop, last path
					multiplier = 0.6f;
					break;
				case 0x9F:
					multiplier = 0.8f;
					break;
				case 0x9E:
					multiplier = 0.8f;
					break;
				default:
					break;
			}
			break;
		case DarkDesert:
			switch(object->object_id) {
				case 0xF6:
					multiplier = 0.5f;
					break;
				case 0x2D0:
					multiplier = 0.3f;
					break;
			}
			break;
		default:
			break;
	}
	return angle
		   * (-flyHoopSpeed + ((-currentSpeed + (-player->gearStats[player->level].topSpeed * 0.1f)) * multiplier));
}

inline f32 SpecificFlyHoops(ObjectNode *object, Player *player, f32 currentHoopSpeed) {
	f32 flyHoopSpeed = SpecificFlyHoopSpeeds[CurrentStage];
	bool toAddSpeed = FALSE;
	u32 object_id = object->object_id;

	switch(CurrentStage) {
		case SegaIllusion:
			if(object_id >= 0xAA && object_id <= 0xAD) {
				toAddSpeed = TRUE;
			}

			break;
		case EggFactory:
			if(object_id >= 0x125 && object_id <= 0x126) {
				toAddSpeed = TRUE;
				break;
			}
			if(object_id >= 0x2C5 && object_id <= 0x2C7) {
				toAddSpeed = TRUE;
			}
			break;
		case WhiteCave:
			if(object_id == 0x27) {
				player->changeInAir_gain += 30000;// 30 air
			} else {
				toAddSpeed = TRUE;
			}
			break;
		case MetalCity:
			if(object_id >= 0x122 && object_id <= 0x124) {
				toAddSpeed = TRUE;
				currentHoopSpeed *= 1.2f;
			}
			break;
		case SplashCanyon:
			if(object_id >= 0x9E && object_id <= 0xA0) {
				// last fly route
				currentHoopSpeed *= 3.6f;
			} 
			// else if(object_id == 0xB6 || object_id == 0xB7 || object_id == 0x23B) {
			// 	// first fly route
			// 	currentHoopSpeed *= 1.7f;
			// }
			break;
		case SkyRoad:
			if(object_id >= 0xD4 && object_id <= 0xD6) {
				currentHoopSpeed *= 1.4f;
			}
			break;
		case DarkDesert:
			if((object_id >= 0xF6 && object_id <= 0xF8)
				|| object_id == 0x2D0){
				currentHoopSpeed *= 2.0f;
			}
			// BIGASS NOTE: THE OBJECT IDS CHANGE IF YOU CHANGE THE STAGE.
			if(object_id >= 0x2D1 && object_id <= 0x2D3) {
				currentHoopSpeed *= 0.875f;
			}
			break;
		default:
			break;
	}

	if(toAddSpeed) {
		currentHoopSpeed += flyHoopSpeed;
	}

	return currentHoopSpeed;
}

inline f32 FlyCharacter_DualType(Player *player, f32 currentFlyHoopSpeed) {
	const auto &characterType = player->characterptr->type;
	if(characterType == Character::FlyType) {
		currentFlyHoopSpeed *= 0.975f;
	} else {
		currentFlyHoopSpeed *= 0.95f;
	}
	return currentFlyHoopSpeed;
}

inline f32 FlyCharacter_AllType(/*Player *player, */ f32 currentFlyHoopSpeed) {
	//u8 characterType = player->characterptr->type;
	// if (player->character == MetalSonic && player->extremeGear == ChaosEmerald) {
	// 	currentFlyHoopSpeed *= 0.925f + (player->objectLinkCount * 0.08f);
	// }
	// else {
	currentFlyHoopSpeed *= 0.925f;
	// }
	return currentFlyHoopSpeed;
}

ASMUsed void CustomFlyHoopSpeeds(Player *player, ObjectNode *object, f32 currentFlyHoopSpeed, f32 angle) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	const f32 flyHoopSpeed = currentFlyHoopSpeed * angle;
	f32 newFlyHoopSpeed = flyHoopSpeed + StageBaseSpeeds[CurrentStage];
	newFlyHoopSpeed = SpecificFlyHoops(object, player, newFlyHoopSpeed);
	if(player->character == Emerl) {
		GizoidReplicationInfo *grInfo = &PlayerGizoidReplication[player->index];
		if(grInfo->isEnabled && InGamePlayerCount >= 2) {
			const auto typeCount = player->getTypeCount();
			if(typeCount < 2) {
				newFlyHoopSpeed *= GR_TypeShortcutSpeedMultiplier;
			} else if(typeCount < 3) {
				newFlyHoopSpeed *= GR_TypeShortcutSpeedMultiplierDual;
			}
		}
	}

	switch(player->extremeGear) {
		using namespace ExtremeGear;
		case CoverS:
			if(player->characterptr->type != 0x1) {
				newFlyHoopSpeed *= 1.085f;
			}
			break;
		case Access:
			if(player->typeAttributes == Type::Fly) {
				// mono access
				newFlyHoopSpeed *= 1.085f;
			}
			break;
		case ChaosEmerald:
			if(player->character == MetalSonic) {
				newFlyHoopSpeed += (player->objectLinkCount * 0.08f);
			} else if(player->character == Tails && player->superFormState != 0) {
				newFlyHoopSpeed *= 1.1f;
			}
			break;
		// case SuperHangOn:
		// {
		//     HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];
		//     if (exLoads.gearExLoadID == HyperHangOnEXLoad && player->characterptr->type == 0x1 && hhoInfo->extraType == FlyType && hhoInfo->saturnMegadriveStatus == 2) newFlyHoopSpeed *= 1.085f;
		//     break;
		// }
		// case TurboStar:
		// {
		// 	OKGInfo *okgInfo = &PlayerOKGInfo[player->index];
		//     if (exLoads.gearExLoadID == OllieKingGearEXLoad) flyHoopSpeed += (okgInfo->currentSpeed*0.10f);
		// 	break;
		// }
		// case DefaultGear:
		// {
		// 	if (exLoads.gearExLoadID == WindmasterJetEXLoad)
		//     {
		// 		WindmasterInfo *wmInfo = &PlayerWMInfo[player->index];
		//         if (wmInfo->lastShortcutType == (u8)1) newFlyHoopSpeed += ((player->objectLinkCount+(u32)1)*0.08f);
		// 		// if (wmInfo->lastShortcutType == (u8)1) newFlyHoopSpeed *= 1.1f;
		//     }
		// 	break;
		// }
		default:
			break;
	}

	const auto typeCount = player->getTypeCount();
	if(typeCount == 2) {
		newFlyHoopSpeed = FlyCharacter_DualType(player, newFlyHoopSpeed);
	} else if(typeCount == 3) {
		newFlyHoopSpeed = FlyCharacter_AllType(/*player,*/ newFlyHoopSpeed);
	}

	// Experimental new hoop dash V2
	PlayerFlyHoopDashProperties *properties = &playerFlyHoopDashProperties[player->index];
	if(player->input->holdFaceButtons.hasAny(BButton, XButton, LButton)) {
		newFlyHoopSpeed *= 1.2f;
		PlayAudioFromDAT(Sound::SFX::RailHoopDash);// dash sfx
		Player_InitBoostParticles(player);
		// properties->gainDelay are the amount of frames it delays the air gain.
		// change this value here to change the delay time
		if(!player->specialFlags.hasAny(ringGear)) {
			properties->gainDelay = 30.0f;
		}

		// don't worry about this duration here
		// properties->duration += 1;

		// This handles the air subtraction in magneticimpulse.cpp, tells it to subtract air if true.
		properties->hasUsed = TRUE;

		// "player->magneticImpulse_timer - 60.0f" is the time (in frames) it takes from the MI timer
		// change this value here to change the amount of time it takes away
		f32 newMI = player->magneticImpulse_timer - 60.0f;
		player->magneticImpulse_timer = clamp(newMI, 0.f);
	} else if(properties->gainDelay > 0.0f) {
		properties->gainDelay = 0.0f;
	}

	if(newFlyHoopSpeed < flyHoopSpeed && CurrentStage != DarkDesert){
		newFlyHoopSpeed = flyHoopSpeed;
	}

	if(MI::impulseData[player->index].magneticImpulse) {
		newFlyHoopSpeed += MI::calculateMultiplier(
				player, (newFlyHoopSpeed * 0.2f) * (player->magneticImpulse_timer / MI::MaximumCap));
	}

	player->speed = newFlyHoopSpeed;
}