#include "context.hpp"
#include "cosmetics/player/exloads.hpp"
#include "mechanics/characters/gizoidreplication.hpp"
#include "mechanics/magneticimpulse.hpp"

constexpr f32 PowerObjectSpeeds[17] = {
        0,         // test stage
        0,         // metal city
        pSpeed(50),// splash canyon
        0,         // egg factory
        0,         // green cave
        0,         // sand ruins
        0,         // babylon garden
        0,         // digital dimension
        0,         // sega carnival
        0,         // night chase
        pSpeed(50),// red canyon
        0,         // ice factory
        0,         // white cave
        pSpeed(25),// dark desert
        pSpeed(35),// sky road
        0,         // babylon guardian
        0,         // sega illusion
};

constexpr f32 PowerObjectSpeeds_MaxMISpeed = pSpeed(50.0f);

ASMUsed u32 PowerObjectSpeed(Player *player, Object *object, Object *digitalObject, u32 airGain) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if (player->specialFlags & ringGear && player->extremeGear != Accelerator && player->extremeGear != AdvantageF &&
	exLoads.gearExLoadID != HyperSonicEXLoad) {
		f32 startObj = 80.0f;

		if (player->typeAttributes & AllType) startObj = 60.0f;
		else if (player->typeAmount == 2) startObj = 80.0f;
		else startObj = 100.0f;

		f32 modifier = startObj - ((f32)player->objectLinkCount * 10.0f);
		if ((modifier < 0.0f)) modifier = 0.0f; 
		magneticImpulseData[player->index].ringPowerObjTimer += modifier;
	}

	f32 speedToAdd = PowerObjectSpeeds[CurrentStage];
	if(player->character == Emerl) {
		GizoidReplicationInfo *grInfo = &PlayerGizoidReplication[player->index];
		if(grInfo->isEnabled && InGamePlayerCount >= 2) {
			if (player->typeAmount < 2) 
			{speedToAdd *= GR_TypeShortcutSpeedMultiplier;}
			else if (player->typeAmount < 3)
			{speedToAdd *= GR_TypeShortcutSpeedMultiplierDual;}
			grInfo->timer += toFrames(0.5f);
		}
	}
	f32 newSpeed = player->speed + speedToAdd;

	if(CurrentStage == DigitalDimension && digitalObject) {
		if(digitalObject->object_group == 0x811A) {
			airGain *= 2;
		}
	}

	switch(CurrentStage) {
		case SkyRoad:
			airGain *= 3;
			if(!object) { break; }
			if(object->object_id == 0xEB) {
				airGain = 30000;
				newSpeed -= pSpeed(30);
			}
			break;

		case SandRuins:
			airGain *= 2;
			break;

		case IceFactory:
			if(object && object->object_id == 0xA) {
				// icicle on last ramp
				airGain = 30000;
			}
			break;
		default:
			break;
	}

	if(*(u16 *) &player->character == 0x212) {// super knuckles
		newSpeed += pSpeed(10);
	} else {
		switch(player->extremeGear) {
			case CoverS:
				if(player->characterptr->type != 0x2) {
					newSpeed += pSpeed(10);
				}
				break;

			case Destroyer:
				if(player->typeAttributes == PowerType) {
					newSpeed += pSpeed(13);
				}
				break;
			case ChaosEmerald:
				switch(player->character) {
					case MetalSonic:
						newSpeed += pSpeed(3) * static_cast<f32>(player->objectLinkCount);
						break;
					case SuperSonic:
						if(player->movementFlags & boosting
						   && player->shortcutAirGainMultiplier > 0
						   && exLoads.gearExLoadID != HyperSonicEXLoad) {
							newSpeed += pSpeed(10.0f);
						}
						break;
					default:
						break;
				}
				[[fallthrough]]; // todo: Is this supposed to fall through?
				// case SuperHangOn:
				//     {
				//         struct HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];
				//         if (exLoads.gearExLoadID == HyperHangOnEXLoad && player->characterptr->type == 0x2 && hhoInfo->extraType == PowerType && hhoInfo->saturnMegadriveStatus == 2) newSpeed += pSpeed(10.0f);
				//         break;
				//     }
			case TurboStar: {
				if(exLoads.gearExLoadID == OllieKingGearEXLoad) newSpeed += (player->speed * 10) / 100;
				break;
			}
			default:
				break;
		}
		if((player->typeAttributes & 0x7) == 0x7) {
			speedToAdd *= 0.5f;
		}
	}

	if(magneticImpulseData[player->index].magneticImpulse) {
		newSpeed += MagneticImpulse_CalculateMultiplier(
		        player, (player->magneticImpulse_timer / MI_MaximumCap) * PowerObjectSpeeds_MaxMISpeed);
	}

	player->speed = newSpeed;
	return airGain;
}