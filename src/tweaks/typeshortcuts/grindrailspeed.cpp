#include "cosmetics/player/exloads.hpp"
#include "mechanics/characters/gizoidreplication.hpp"
#include "mechanics/magneticimpulse.hpp"
#include "riders/stage.hpp"
#include "gears/omnipotence.hpp"
#include "gears/supermetal.hpp"
#include "handlers/player/specialflagtweaks.hpp"
#include "riders/gamemode.hpp"

constexpr m2darray<s16, 2, TotalStageAmount> data_GrindRailSpeed = {{
		{
				// dynamic grind rail speed additives (from ?)
				0, // test stage
				110, // metal city
				110, // splash canyon
				185, // egg factory
				70, // green cave
				65, // sand ruins
				190, // babylon garden
				25, // digital dimension
				80, // sega carnival
				0, // night chase
				25, // red canyon
				60, // ice factory
				20, // white cave
				30, // dark desert
				70, // sky road
				150, // babylon guardian
				85, // sega illusion
		},
		{
				// default grind rail speed additives (from 180)
				0, // test stage
				60, // metal city
				320, // splash canyon
				50, // egg factory
				50, // green cave
				50, // sand ruins
				50, // babylon garden
				110, // digital dimension
				450, // sega carnival
				90, // night chase
				50, // red canyon
				50, // ice factory
				50, // white cave
				160, // dark desert
				50, // sky road
				50, // babylon guardian
				50, // sega illusion
		}
}};

constexpr std::array<f32, TotalStageAmount> data_MIRailBonus = {
			{
				// rail speed granted with MI for a stage
				pSpeed(40.0f),  // test stage
				pSpeed(50.0f),// metal city
				pSpeed(20.0f),// splash canyon
				pSpeed(40.0f),// egg factory
				pSpeed(25.0f), // green cave
				pSpeed(20.0f), // sand ruins
				pSpeed(20.0f),// babylon garden
				pSpeed(20.0f), // digital dimension
				pSpeed(30.0f), // sega carnival
				pSpeed(30.0f),  // night chase
				pSpeed(40.0f), // red canyon
				pSpeed(40.0f), // ice factory
				pSpeed(20.0f), // white cave
				pSpeed(20.0f),  // dark desert
				pSpeed(40.0f), // sky road
				pSpeed(50.0f),// babylon guardian
				pSpeed(20.0f), // sega illusion
		},
};

ASMUsed s16 lbl_GrindRailSpeed(s16 railSpeed, Player &player) {
	if (CurrentGameMode == MissionMode) {
		railSpeed = 250;
		return railSpeed;
	}

	//const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(player);

	u32 railType = 0;
	if(!railSpeed) {
		railSpeed = 180; // default rail speed
		railType  = 1; // offset index
	}
	railSpeed += data_GrindRailSpeed[railType][CurrentStage];

	// specific grind rail changes
	switch(CurrentStage) {
		case MetalCity:
			if(player.railID == 2) {
				// final rail
				railSpeed = 400; // 440
			}
			break;
		case IceFactory:
			if(player.railID == 0) {
				railSpeed = 250;
			}
			if(player.railID == 1) {
				railSpeed = 550;
			}
			if(player.railID >= 2 && player.railID <= 4) {
				railSpeed = 330;
			}
			break;
		case RedCanyon:
			if(player.railID == 0) {
				railSpeed = 400;
			}
			break;
		case GreenCave:
			if(player.railID == 0) {
				railSpeed = 355;
			}
			if(player.railID == 1) {
				railSpeed = 270;
			}
			if(player.railID == 2) {
				railSpeed = 330;
			}
			if(player.railID == 3) {
				railSpeed = 280;
			}
			break;
		case WhiteCave:
			if(player.railID == 0) {
				railSpeed = 250;
			}
			if(player.railID == 2) {
				railSpeed = 290;
			}
			break;
		case BabylonGarden:
			if(player.railID == 1) {
				railSpeed = 380;
			}
			break;
		case SandRuins: if(player.railID == 0) {
				// first rail
				railSpeed = 500;
			}
			if(player.railID == 1) {
				// second rail
				railSpeed = 700;
			}
			if(player.railID == 2) {
				// second rail
				railSpeed = 240;
			}
			if(player.railID == 3) {
				// second rail
				railSpeed = 415;
			}
			break;
		case SkyRoad: if(player.railID == 0) {
				// last rail before QTE
				railSpeed = 460;
			}
			break;
		case NightChase: if(player.railID > 1) {
				// Post tunnel rails
				railSpeed = 300;
			}
			break;
		case SegaCarnival: if(player.railID == 1) {
				railSpeed = 270;
			}
			if (player.railID == 3 || player.railID == 4) {
				// Post QTE rails
				railSpeed = 250;
			}
			break;
		case SegaIllusion: if(player.railID == 1) {
				// Post QTE rails
				railSpeed = 320;
			}
			if (player.railID == 2) {
				// Post QTE rails
				railSpeed = 320;
			}
			break;
		default: break;
	}

	if(player.character == Character::Emerl) {
		const GizoidReplicationInfo &grInfo = PlayerGizoidReplication[player.index];
		if(grInfo.isEnabled && InGamePlayerCount >= 2) {
			switch(player.getTypeCount()) {
				case 0:
				case 1: railSpeed *= GR_TypeShortcutSpeedMultiplier;
					break;
				case 2: railSpeed *= GR_TypeShortcutSpeedMultiplierDual;
					break;
				default: break;
			}
		}
	}

	switch(player.extremeGear) {
			using namespace ExtremeGear;
		case CoverS: {
			if(player.characterptr->type != Character::Type::Speed) {
				railSpeed *= 1.115f;
			}
			break;
		}
		case Grinder: {
			if(player.typeAttributes == Type::Speed) // mono grinder
			{
				railSpeed *= 1.085f;
			}
			break;
		}
		case ChaosEmerald: {
			if(player.character == Character::MetalSonic) {
				if(player.gearExload().exLoadID == EXLoad::StardustSpeeder) {
					NeoMetalInfo *neoInfo = &PlayerNeoMetalInfo[player.index];
					if(neoInfo->formState == 2 && neoInfo->stolenType == Type::Speed) {
						railSpeed += static_cast<s16>(static_cast<f32>(player.objectLinkCount) * 0.10f * SPEED_DIVISOR); // NEO III
					} else if (neoInfo->formState == 2) {
						railSpeed += static_cast<s16>(static_cast<f32>(player.objectLinkCount) * 0.08f * SPEED_DIVISOR); // NEO III
					}
				} else railSpeed += static_cast<s16>(static_cast<f32>(player.objectLinkCount) * 0.08f * SPEED_DIVISOR);
			}
			break;
		}
		case Omnipotence: {
			const OmnipotenceInfo &OMNInfo = PlayerOMNInfo[player.index];
			if(OMNInfo.lastShortcutType != 1) {
				// if (player.character == E10G) {railSpeed *= 1.05f;}
				// else
				if(OMNInfo.lastShortcutType != 0) {
					railSpeed *= 1.05f;
				}
			}
			break;
		}
			// case SuperHangOn:
			// {
			//     HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];
			//     if (exLoads.gearExLoadID == HyperHangOn && player->characterptr->type == 0x0 && hhoInfo->extraType == SpeedType && hhoInfo->saturnMegadriveStatus == 2) railSpeed *= 1.085f;
			//     break;
			// }

		default: break;
	}

	switch(player.getTypeCount()) {
		case 2:
			if(player.characterptr->type == Character::Type::Speed) {
				railSpeed *= 0.975f;
			} else {
				railSpeed *= 0.95f;
			}
			break;
		case 3: {
			// if (player.character == MetalSonic) {
			//     railSpeed *= 1+(player.objectLinkCount*0.08f);
			// }
			SpecialFlagInfo &spfInfo = PlayerSpecialFlagInfo[player.index];
			if(player.characterptr->type == Character::Type::Speed && player.extremeGear == ExtremeGear::Omnipotence) {
				// speed type
				railSpeed *= 0.95f;
			}
            else {
				railSpeed *= 0.925f;
			}
			break;
		}
		default: break;
	}

	if(MI::impulseData[player.index].magneticImpulse) {
		// f32 railSpeedMI = pSpeed(75.0f) / (pSpeed(railSpeed) / pSpeed(300.0f));
		f32 railSpeedMI = data_MIRailBonus[CurrentStage];
		railSpeedMI *= player.magneticImpulse_timer / MI::MaximumCap; // MI percentage
		railSpeedMI = MI::calculateMultiplier(player, railSpeedMI);
		railSpeed += railSpeedMI * SPEED_DIVISOR;
	}

	return railSpeed;
}
