#include "cosmetics/player/exloads.hpp"
#include "mechanics/characters/gizoidreplication.hpp"
#include "mechanics/magneticimpulse.hpp"
#include "riders/stage.hpp"

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
				5,  // dark desert
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
				100,// dark desert
				50, // sky road
				50, // babylon guardian
				50, // sega illusion
		}
}};

ASMUsed s16 lbl_GrindRailSpeed(s16 railSpeed, Player *player) {
	//EnabledEXLoads exLoads;
	//FetchEnabledEXLoadIDs(player, exLoads);

	u32 railType = 0;
	if(!railSpeed) {
		railSpeed = 180;// default rail speed
		railType = 1;   // offset index
	}
	railSpeed += data_GrindRailSpeed[railType][CurrentStage];

	// specific grind rail changes
	switch(CurrentStage) {
		case SkyRoad:
			if(player->railID == 0) {
				// last rail before QTE
				railSpeed = 405;
			}
			break;
		default:
			break;
	}

	if(player->character == Emerl) {
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
			if(player->characterptr->type != 0x0) {
				railSpeed *= 1.085f;
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
			if(player->character == MetalSonic) {
				railSpeed += static_cast<s16>(static_cast<f32>(player->objectLinkCount) * 0.08f * SPEED_DIVISOR);
			}
			break;
		}
			// case SuperHangOn:
			// {
			//     HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];
			//     if (exLoads.gearExLoadID == HyperHangOnEXLoad && player->characterptr->type == 0x0 && hhoInfo->extraType == SpeedType && hhoInfo->saturnMegadriveStatus == 2) railSpeed *= 1.085f;
			//     break;
			// }
			// case TurboStar:
			// {
			//     OKGInfo *okgInfo = &PlayerOKGInfo[player->index];
			//     if (exLoads.gearExLoadID == OllieKingGearEXLoad) railSpeed += (okgInfo->currentSpeed*0.10f*SPEED_DIVISOR);
			//     break;
			// }

			// case DefaultGear:
			// {
			//     if (exLoads.gearExLoadID == WindmasterJetEXLoad)
			//     {
			//         WindmasterInfo *wmInfo = &PlayerWMInfo[player->index];
			//         if (wmInfo->lastShortcutType == (u8)2) railSpeed += ((player->objectLinkCount+(u32)1)*0.16f*SPEED_DIVISOR);
			//         // if (wmInfo->lastShortcutType == (u8)2) railSpeed *= 1.085f;
			//     }
			//     break;
			// }
		default:
			break;
	}

	const auto typeCount = player->getTypeCount();
	if(typeCount == 2) {            // dual type
		if(player->characterptr->type == 0) {// speed type
			railSpeed *= 0.975f;
		} else {
			railSpeed *= 0.95f;
		}
	} else if(typeCount == 3) {
		// if (player->character == MetalSonic) {
		//     railSpeed *= 1+(player->objectLinkCount*0.08f);
		// }
		railSpeed *= 0.925f;
	}

	if(MI::impulseData[player->index].magneticImpulse) {
		f32 railSpeedMI = pSpeed(75.0f) / (pSpeed(railSpeed) / pSpeed(300.0f));
		railSpeedMI *= player->magneticImpulse_timer / MI::MaximumCap;// MI percentage
		railSpeedMI = MI::calculateMultiplier(player, railSpeedMI);
		railSpeed += railSpeedMI * SPEED_DIVISOR;
	}

	return railSpeed;
}