#include "cosmetics/player/exloads.hpp"
#include "gears/blastGaugeGears.hpp"
#include "mechanics/characters/gizoidreplication.hpp"
#include "mechanics/magneticimpulse.hpp"
#include "riders/object.hpp"
#include "riders/stage.hpp"

constexpr std::array<f32, TotalStageAmount> PowerObjectSpeeds = {
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
        0,		   // dark desert pSpeed(25)
        pSpeed(35),// sky road
        0,         // babylon guardian
        0,         // sega illusion
};

constexpr f32 PowerObjectSpeeds_MaxMISpeed = pSpeed(50.0f);

ASMUsed u32 PowerObjectSpeed(Player *player, ObjectNode *object, ObjectNode *digitalObject, u32 airGain) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
	BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];

	if (player->specialFlags.hasAny(ringGear) &&
	   player->extremeGear != ExtremeGear::Accelerator &&
	exLoads.gearExLoadID != HyperSonicEXLoad) {
		f32 startObj = 80.0f;
		const auto typeCount = player->getTypeCount();

		if (typeCount == 3) {
			startObj = 60.0f;
		} else if (typeCount == 2) {
			startObj = 80.0f;
		} else {
			startObj = 100.0f;
		}

		f32 modifier = startObj - (static_cast<f32>(player->objectLinkCount) * 10.0f);
		if (modifier < 0.0f) { modifier = 0.0f; }
		MI::impulseData[player->index].ringPowerObjTimer += modifier;
	}

	f32 speedToAdd = PowerObjectSpeeds[CurrentStage];
	if(player->character == Emerl) {
		GizoidReplicationInfo *grInfo = &PlayerGizoidReplication[player->index];
		if(grInfo->isEnabled && InGamePlayerCount >= 2) {
			const auto typeCount = player->getTypeCount();
			if (typeCount < 2){
				speedToAdd *= GR_TypeShortcutSpeedMultiplier;
				speedToAdd += pSpeed(5.0f);
			}
			else if (typeCount < 3){
				speedToAdd *= GR_TypeShortcutSpeedMultiplierDual;
				speedToAdd += pSpeed(2.0f);
			}
			grInfo->timer += toFrames(0.5);
		}
	}
	f32 newSpeed = player->speed + speedToAdd;

	if(CurrentStage == DigitalDimension && digitalObject != nullptr) {
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

	if(isSuperCharacter(*player, Knuckles)) {// super knuckles
		newSpeed += pSpeed(10);
	} else {
		switch(player->extremeGear) {
			using namespace ExtremeGear;
			case CoverS:
				if(player->characterptr->type != 0x2) {
					newSpeed += pSpeed(10);
				}
				break;

			case Destroyer:
				if(player->typeAttributes == Type::Power) {
					newSpeed += pSpeed(13);
				}
				break;
			case ChaosEmerald:
				switch(player->character) {
					case MetalSonic:
						newSpeed += pSpeed(3) * static_cast<f32>(player->objectLinkCount);
						break;
					case SuperSonic:
						if(player->movementFlags.hasAny(boosting)
						   && bgInfo->currentGauge > 0
						   && exLoads.gearExLoadID != HyperSonicEXLoad) {
							newSpeed += pSpeed(10.0f);
						}
						break;
					default:
						break;
				}
				break;
				// case SuperHangOn:
				//     {
				//         HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];
				//         if (exLoads.gearExLoadID == HyperHangOnEXLoad && player->characterptr->type == 0x2 && hhoInfo->extraType == PowerType && hhoInfo->saturnMegadriveStatus == 2) newSpeed += pSpeed(10.0f);
				//         break;
				//     }
			case TurboStar: {
				if(exLoads.gearExLoadID == OllieKingGearEXLoad) {
					newSpeed += (player->speed * 10) / 100;
				}
				break;
			}
			default:
				break;
		}
		if (player->characterArchetype == Mechanic)
		{newSpeed += pSpeed(1.0f);}

		if(player->getTypeCount() == 3) {
			speedToAdd *= 0.5f;
		}
	}

	if(MI::impulseData[player->index].magneticImpulse) {
		newSpeed += MI::calculateMultiplier(
				player, (player->magneticImpulse_timer / MI::MaximumCap) * PowerObjectSpeeds_MaxMISpeed);
	}

	player->speed = newSpeed;
	return airGain;
}