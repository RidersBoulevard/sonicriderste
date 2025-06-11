#include "cosmetics/player/exloads.hpp"
#include "gears/blastGaugeGears.hpp"
#include "mechanics/characters/gizoidreplication.hpp"
#include "mechanics/magneticimpulse.hpp"
#include "riders/object.hpp"
#include "riders/stage.hpp"
#include "gears/omnipotence.hpp"
#include "handlers/player/specialflagtweaks.hpp"
#include "gears/supermetal.hpp"
#include "gears/hypersonic.hpp"

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
        pSpeed(15),		   // dark desert pSpeed(25)
        pSpeed(35),// sky road
        0,         // babylon guardian
        0,         // sega illusion
};

constexpr f32 PowerObjectSpeeds_MaxMISpeed = pSpeed(35.0f);

ASMUsed u32 PowerObjectSpeed(Player &player, const ObjectNode *object, const ObjectNode *digitalObject, u32 airGain) {
	//BlastGaugeInfo &bgInfo = PlayerBlastGaugeInfo[player.index];
	const auto &exloadID   = player.gearExload().exLoadID;

    // && player.extremeGear != ExtremeGear::Accelerator
	if(player.specialFlags.hasAny(SpecialFlags::ringGear) &&
	   exloadID != EXLoad::HyperSonic) {
		f32 startObj;
		switch(player.getTypeCount()) {
			case 3:
				startObj = 60.0f;
				break;
			case 2:
				startObj = 80.0f;
				break;
			case 1:
			case 0:
				startObj = 100.0f;
				break;
			default:
				break;
		}

		f32 modifier = startObj - (static_cast<f32>(player.objectLinkCount) * 10.0f);
		if(modifier < 0.0f) {
			modifier = 0.0f;
		}
		MI::impulseData[player.index].ringPowerObjTimer += modifier;
	}

	f32 speedToAdd = PowerObjectSpeeds[CurrentStage];
	if(player.character == Character::Emerl) {
		GizoidReplicationInfo &grInfo = PlayerGizoidReplication[player.index];
		if(grInfo.isEnabled && InGamePlayerCount >= 2) {
			switch(player.getTypeCount()) {
				case 0:
				case 1:
					speedToAdd *= GR_TypeShortcutSpeedMultiplier;
					speedToAdd += pSpeed(5.0f);
					break;
				case 2:
					speedToAdd *= GR_TypeShortcutSpeedMultiplierDual;
					speedToAdd += pSpeed(2.0f);
					break;
				default:
					break;
			}
			grInfo.timer += toFrames(0.5);
		}
	}

	SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[player.index];
	if(player.getTypeCount() == 3) {
		if(player.characterptr->type == Character::Type::Power && player.extremeGear == ExtremeGear::Omnipotence) {
			speedToAdd *= 0.75f;
		}
		else speedToAdd *= 0.5f;
	}

	f32 newSpeed = player.speed + speedToAdd;

	if(CurrentStage == DigitalDimension && digitalObject != nullptr) {
		if(digitalObject->object_group == 0x811A) {
			airGain *= 2;
		}
	}

	switch(CurrentStage) {
		case SkyRoad: airGain *= 3;
			if(object == nullptr) {
				break;
			}
			if(object->object_id == 0xEB) {
				airGain = 30000;
				newSpeed -= pSpeed(30);
			}
			break;

		case SandRuins: airGain *= 2;
			break;

		case IceFactory: if(object && object->object_id == 0xA) {
				// icicle on last ramp
				airGain = 30000;
			}
			break;

		case NightChase:
			if(!object) { break; }
			if(object->object_id >= 0x131 && object->object_id <= 0x137) {break;} // 1st power path
			if((object->object_id >= 0xBA && object->object_id <= 0xBC) || object->object_id == 0x211) {
				// 2nd power path stationary cars
				airGain += (airGain / 4); // +25% air gain
				break;
			}
			airGain *= 2; // only the stray cars and power path entrance should be doubled
			break;

		default:
			break;
	}

	if(player.isSuperCharacter(Character::Knuckles)) {
		// super knuckles
		// newSpeed += pSpeed(10);
	} else {
		switch(player.extremeGear) {
				using namespace ExtremeGear;
			case CoverS: if(player.characterptr->type != Character::Type::Power) {
					newSpeed += pSpeed(11.5);
				}
				break;

			case Destroyer: if(player.typeAttributes == Type::Power) {
					newSpeed += pSpeed(13);
				}
				break;
			case ChaosEmerald: 
			switch(player.character) {
					case Character::MetalSonic: {
						if (player.gearExload().exLoadID == EXLoad::StardustSpeeder) {
							NeoMetalInfo *neoInfo = &PlayerNeoMetalInfo[player.index];
							if(neoInfo->formState == 2) {
								newSpeed += pSpeed(4) * static_cast<f32>(player.objectLinkCount); // NEO III
							}
						} else newSpeed += pSpeed(3) * static_cast<f32>(player.objectLinkCount);
					}
						break;
					case Character::SuperSonic: {
						BlastGaugeInfo &bgInfo = PlayerBlastGaugeInfo[player.index];
						HyperSonicInfo &hsInfo = PlayerHyperSonicInfo[player.index];
						if (player.gearExload().exLoadID == EXLoad::HyperSonic) {
							if (hsInfo.hyperdriveEnabled) {bgInfo.currentGauge += 1700;}
							break;
						}
						if (player.movementFlags.hasAny(MovementFlags::boosting)) {
							if (bgInfo.currentGauge > 0) {newSpeed += pSpeed(10.0f);}
							s32 newBlastGaugeValue = bgInfo.currentGauge - 10000;
							bgInfo.currentGauge = clamp(newBlastGaugeValue);
						}
					    break;
					}
					default: break;
				}
				break;
			case Omnipotence: {
				const OmnipotenceInfo &OMNInfo = PlayerOMNInfo[player.index];
				if(OMNInfo.lastShortcutType != 3) {
					// if (player.character == E10G) {newSpeed += pSpeed(5.0f);}
					// else 
					if (OMNInfo.lastShortcutType != 0) {newSpeed += pSpeed(5.0f);}
				}
				break;
			}
				// case SuperHangOn:
				//     {
				//         HHOInfo *hhoInfo = &PlayerHHOInfo[player.index];
				//         if (exLoads.gearExLoadID == HyperHangOnEXLoad && player.characterptr.type == 0x2 && hhoInfo.extraType == PowerType && hhoInfo.saturnMegadriveStatus == 2) newSpeed += pSpeed(10.0f);
				//         break;
				//     }
			case Darkness: {
			    if (player.gearExload().exLoadID == EXLoad::TheProfessional) player.rings += 2;
			}
			default: break;
		}
		if(player.characterArchetype == CharacterArchetype::Mechanic) {
			newSpeed += pSpeed(1.0f);
		}

	}

	if(MI::impulseData[player.index].magneticImpulse) {
		newSpeed += MI::calculateMultiplier(
			player,
			(player.magneticImpulse_timer / MI::MaximumCap) * PowerObjectSpeeds_MaxMISpeed
		);
	}

	player.speed = newSpeed;
	return airGain;
}
