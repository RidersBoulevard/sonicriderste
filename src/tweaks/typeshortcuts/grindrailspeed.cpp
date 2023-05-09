#include "context.hpp"
#include "cosmetics/player/exloads.hpp"
#include "mechanics/characters/gizoidreplication.hpp"
#include "mechanics/magneticimpulse.hpp"

constexpr s16 data_GrindRailSpeed[2][TotalStageAmount] = {
        {
                // dynamic grind rail speed additives (from ?)
                0,  // test stage
                110,// metal city
                160,// splash canyon
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
			if (player->typeAmount < 2) 
			{railSpeed *= GR_TypeShortcutSpeedMultiplier;}
			else if (player->typeAmount < 3)
			{railSpeed *= GR_TypeShortcutSpeedMultiplierDual;}
		}
	}

	switch(player->extremeGear) {
		case CoverS: {
			if(player->characterptr->type != 0x0) {
				railSpeed *= 1.085f;
			}
			break;
		}
		case Grinder: {
			if(player->typeAttributes == SpeedType)// mono grinder
			{
				railSpeed *= 1.085f;
			}
			break;
		}
		case ChaosEmerald: {
			if(player->character == MetalSonic) {
				railSpeed += (player->objectLinkCount * 0.08f * SPEED_DIVISOR);
			}
			break;
		}
			// case SuperHangOn:
			// {
			//     struct HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];
			//     if (exLoads.gearExLoadID == HyperHangOnEXLoad && player->characterptr->type == 0x0 && hhoInfo->extraType == SpeedType && hhoInfo->saturnMegadriveStatus == 2) railSpeed *= 1.085f;
			//     break;
			// }
			// case TurboStar:
			// {
			//     struct OKGInfo *okgInfo = &PlayerOKGInfo[player->index];
			//     if (exLoads.gearExLoadID == OllieKingGearEXLoad) railSpeed += (okgInfo->currentSpeed*0.10f*SPEED_DIVISOR);
			//     break;
			// }

			// case DefaultGear:
			// {
			//     if (exLoads.gearExLoadID == WindmasterJetEXLoad)
			//     {
			//         struct WindmasterInfo *wmInfo = &PlayerWMInfo[player->index];
			//         if (wmInfo->lastShortcutType == (u8)2) railSpeed += ((player->objectLinkCount+(u32)1)*0.16f*SPEED_DIVISOR);
			//         // if (wmInfo->lastShortcutType == (u8)2) railSpeed *= 1.085f;
			//     }
			//     break;
			// }
		default:
			break;
	}

	if(player->typeAmount == 2)// dual type
	{
		if(player->characterptr->type == 0)// speed type
		{
			railSpeed *= 0.975f;
		} else {
			railSpeed *= 0.95f;
		}
	}

	if((player->typeAttributes & 0x7) == 0x7)// omni type
	{
		// if (player->character == MetalSonic) {
		//     railSpeed *= 1+(player->objectLinkCount*0.08f);
		// }
		railSpeed *= 0.925f;
	}

	if(magneticImpulseData[player->index].magneticImpulse) {
		f32 railSpeedMI;
		railSpeedMI = pSpeed(75.0f) / (pSpeed(railSpeed) / pSpeed(300.0f));
		railSpeedMI *= player->magneticImpulse_timer / MI_MaximumCap;// MI percentage
		railSpeedMI = MagneticImpulse_CalculateMultiplier(player, railSpeedMI);
		railSpeed += railSpeedMI * SPEED_DIVISOR;
	}

	return railSpeed;
}