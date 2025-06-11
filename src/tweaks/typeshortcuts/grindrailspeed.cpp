#include "cosmetics/player/exloads.hpp"
#include "mechanics/characters/gizoidreplication.hpp"
#include "mechanics/magneticimpulse.hpp"
#include "riders/stage.hpp"
#include "gears/omnipotence.hpp"
#include "handlers/player/specialflagtweaks.hpp"

constexpr m2darray<s16, 2, TotalStageAmount> data_GrindRailSpeed = {{
		{
				// dynamic grind rail speed additives (from ?)
				0,  // test stage
				110,// metal city
				110,// splash canyon
				185,// egg factory
				70, // green cave
				65, // sand ruins
				190,// babylon garden
				25, // digital dimension
				80, // sega carnival
				0,  // night chase
				25, // red canyon
				60, // ice factory
				20, // white cave
				30,  // dark desert
				70, // sky road
				150,// babylon guardian
				85, // sega illusion
		},
		{
				// default grind rail speed additives (from 180)
				0,  // test stage
				60, // metal city
				320,// splash canyon
				50, // egg factory
				50, // green cave
				50, // sand ruins
				50, // babylon garden
				110,// digital dimension
				450,// sega carnival
				90, // night chase
				50, // red canyon
				50, // ice factory
				50, // white cave
				160,// dark desert
				50, // sky road
				50, // babylon guardian
				50, // sega illusion
		}
}};

ASMUsed s16 lbl_GrindRailSpeed(s16 railSpeed, Player *player) {
	const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);

	u32 railType = 0;
	if(!railSpeed) {
		railSpeed = 180;// default rail speed
		railType = 1;   // offset index
	}
	railSpeed += data_GrindRailSpeed[railType][CurrentStage];

	// specific grind rail changes
	switch(CurrentStage) {
		case SandRuins:
			if(player->railID == 0) {
				// first rail
				railSpeed = 500;
			}
			if(player->railID == 1) {
				// second rail
				railSpeed = 700;
			}
			if(player->railID == 2) {
				// second rail
				railSpeed = 240;
			}
			if(player->railID == 3) {
				// second rail
				railSpeed = 415;
			}
			break;
		case SkyRoad:
			if(player->railID == 0) {
				// last rail before QTE
				railSpeed = 460;
			}
			break;
		case NightChase:
			if (player->railID > 1) {
				// Post tunnel rails
				railSpeed = 300;
			}
			break;
		case SegaCarnival:
			if (player->railID == 3 || player->railID == 4) {
				// Post QTE rails
				railSpeed = 270;
			}
			break;
		case SegaIllusion:
			if (player->railID >= 1) {
				// Post QTE rails
				railSpeed = 320;
			}
			break;
		default:
			break;
	}

	if(player->character == Character::Emerl) {
		GizoidReplicationInfo *grInfo = &PlayerGizoidReplication[player->index];
		if(grInfo->isEnabled && InGamePlayerCount >= 2) {
			const auto typeCount = player->getTypeCount();
			if(typeCount < 2) {
				railSpeed *= GR_TypeShortcutSpeedMultiplier;
			} else if(typeCount < 3) {
				railSpeed *= GR_TypeShortcutSpeedMultiplierDual;
			}
		}
	}

	switch(player->extremeGear) {
		using namespace ExtremeGear;
		case CoverS: {
			if(player->characterptr->type != Character::Type::Speed) {
				railSpeed *= 1.115f;
			}
			break;
		}
		case Grinder: {
			if(player->typeAttributes == Type::Speed)// mono grinder
			{
				railSpeed *= 1.085f;
			}
			break;
		}
		case ChaosEmerald: {
			if(player->character == Character::MetalSonic) {
				railSpeed += static_cast<s16>(static_cast<f32>(player->objectLinkCount) * 0.08f * SPEED_DIVISOR);
			}
			break;
		}
		case Omnipotence: {
			OmnipotenceInfo *OMNInfo = &PlayerOMNInfo[player->index];
			if((OMNInfo->lastShortcutType != 1)) {
				// if (player->character == E10G) {railSpeed *= 1.05f;}
				// else 
				if (OMNInfo->lastShortcutType != 0) {railSpeed *= 1.085f;}
			}
			break;
		}
			// case SuperHangOn:
			// {
			//     HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];
			//     if (exLoads.gearExLoadID == HyperHangOn && player->characterptr->type == 0x0 && hhoInfo->extraType == SpeedType && hhoInfo->saturnMegadriveStatus == 2) railSpeed *= 1.085f;
			//     break;
			// }
		default:
			break;
	}

	const auto typeCount = player->getTypeCount();
	if(typeCount == 2) {            // dual type
		if(player->characterptr->type == Character::Type::Speed) {
			railSpeed *= 0.975f;
		} else {
			railSpeed *= 0.95f;
		}
	} else if(typeCount == 3) {
		// if (player->character == MetalSonic) {
		//     railSpeed *= 1+(player->objectLinkCount*0.08f);
		// }
		SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[player->index];
		if(player->characterptr->type == Character::Type::Speed && player->extremeGear == ExtremeGear::Omnipotence) {// speed type
			railSpeed *= 0.95f;
		} 
		// else if (player->gearExload().exLoadID == EXLoad::DarkSonic && spfInfo->gearChange >= 3 && player->level == 2) {
		// 	// skip
		// }
		else railSpeed *= 0.925f;
	}

	if(MI::impulseData[player->index].magneticImpulse) {
		f32 railSpeedMI = pSpeed(75.0f) / (pSpeed(railSpeed) / pSpeed(300.0f));
		railSpeedMI *= player->magneticImpulse_timer / MI::MaximumCap;// MI percentage
		railSpeedMI = MI::calculateMultiplier(player, railSpeedMI);
		railSpeed += railSpeedMI * SPEED_DIVISOR;
	}

	return railSpeed;
}