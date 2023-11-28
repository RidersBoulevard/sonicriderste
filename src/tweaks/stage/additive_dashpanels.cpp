#include "additive_dashpanels.hpp"
#include "mechanics/magneticimpulse.hpp"
#include "riders/stage.hpp"

ASMUsed void lbl_DashPanelSpeed(f32 dashPanelSpeed, Player *player, ObjectNode *object) {
	const u32 stage = CurrentStage;
	const f32 &stage_additiveSpeed = data_DashPanelSpeed[stage];
	const f32 &setSpeed = pSpeed(200.0f);

	if(player->speed < (setSpeed - stage_additiveSpeed)) {
		dashPanelSpeed = setSpeed;
	} else {
		dashPanelSpeed = player->speed + stage_additiveSpeed;
	}

	const u32 &object_id = object->object_id;
	switch(stage) {
		case NightChase: {
			if(object_id >= 0x5A && object_id <= 0x5E) {
				dashPanelSpeed = pSpeed(250.0f);
			}
			break;
		}
		case DigitalDimension: {
			if(object_id >= 0x108 && object_id <= 0x10B) {
				dashPanelSpeed += pSpeed(50.0f);
			} else if(object_id == 0x28A) {
				dashPanelSpeed = pSpeed(255.0f);
			}
			break;
		}
		case EggFactory: {
			if(object_id == 0x115 || object_id == 0xF9) {
				dashPanelSpeed = pSpeed(999.0f);
			} else if((object_id >= 0xEE && object_id <= 0xF8) || (object_id >= 0xFA && object_id <= 0xFD) || (object_id >= 0xDC && object_id <= 0xE0)) {
				dashPanelSpeed += pSpeed(100.0f);
			}
			break;
		}
		case GreenCave: {
			if(object_id >= 0x22 && object_id <= 0x27) {
				dashPanelSpeed += pSpeed(60.0f);
			}
			break;
		}
		case SplashCanyon:
			if(object_id >= 0x1F6 && object_id <= 0x1F7) {
				dashPanelSpeed += pSpeed(300.0f);
			} else if(object_id >= 0x1F8 && object_id <= 0x1F9) {
				dashPanelSpeed += pSpeed(150.0f);
			}
			break;
		case RedCanyon:
			if(object_id >= 0x22A && object_id <= 0x22B) {
				dashPanelSpeed += pSpeed(100.0f);
			}
			break;
		default:
			break;
	}

	switch(player->extremeGear) {
		using namespace ExtremeGear;
		case CoverS:
		case Omnipotence: {
			dashPanelSpeed += pSpeed(10.0f);
			break;
		}
		// case SuperHangOn:
		// {
		//     if (exLoads.gearExLoadID == HyperHangOnEXLoad)
		//     {
		//         if (hhoInfo->saturnMegadriveStatus == 2) {player->currentAir += (player->gearStats[player->level].maxAir * 20) / 100;}
		//     }
		//     break;
		// }
		default:
			break;
	}

    if (player->characterArchetype == Windcatcher){
        if (player->specialFlags.hasAny(ringGear)){
			player->currentAir += (player->gearStats[player->level].maxAir * 3) / 100;
		} else {
			player->currentAir += (player->gearStats[player->level].maxAir * 10) / 100;
		}
    }

    if (player->characterArchetype == Mechanic) {
		dashPanelSpeed *= 1.1f;
	}

    if (MI::impulseData[player->index].magneticImpulse) {
		f32 isNoType;

		if(player->typeAttributes.hasAny(Type::All)) {
			isNoType = 1.0f;
		} else {
			isNoType = MI::NoTypeMultiplier;
		}

		const f32 MIpercentage = player->magneticImpulse_timer / MI::MaximumCap;
		dashPanelSpeed += (pSpeed(30.0f) * MIpercentage) * isNoType;
	}

	player->speed = dashPanelSpeed;
}