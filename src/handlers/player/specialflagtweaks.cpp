#include "riders/player.hpp"
#include "specialflagtweaks.hpp"
#include "riders/stage.hpp"
#include <mechanics/archetype/afterburner.hpp>
#include "mechanics/magneticimpulse.hpp"
#include "gears/gambler.hpp"
#include "gears/supertails.hpp"

std::array<SpecialFlagInfo, MaxPlayerCount> PlayerSpecialFlagInfo;

void Player_storeFlagInfo(Player *player) {
	SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[player->index];
	if(player->state == PlayerState::StartLine) {
		spfInfo->lvlTopSpeed[0] = player->gearStats[0].topSpeed;
		spfInfo->lvlTopSpeed[1] = player->gearStats[1].topSpeed;
		spfInfo->lvlTopSpeed[2] = player->gearStats[2].topSpeed;

		spfInfo->tierAccel[0][0] = player->gearStats[0].tier1Accel;
		spfInfo->tierAccel[0][1] = player->gearStats[0].tier2Accel;
		spfInfo->tierAccel[0][2] = player->gearStats[0].tier3Accel;

		spfInfo->tierAccel[1][0] = player->gearStats[1].tier1Accel;
		spfInfo->tierAccel[1][1] = player->gearStats[1].tier2Accel;
		spfInfo->tierAccel[1][2] = player->gearStats[1].tier3Accel;

		spfInfo->tierAccel[2][0] = player->gearStats[2].tier1Accel;
		spfInfo->tierAccel[2][1] = player->gearStats[2].tier2Accel;
		spfInfo->tierAccel[2][2] = player->gearStats[2].tier3Accel;

		spfInfo->lightBoardEffect = FALSE;
		spfInfo->isBoosting = FALSE;
        spfInfo->totalBoostTime = 0;
		spfInfo->gearChange = 0;
	}
}

void Player_lightBoardEffect(Player *player) {
	SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[player->index];
	if(player->extremeGear == ExtremeGear::MagicCarpet) {
		// CANNOT be allowed to stack with berserker in ASM
		// Still has light board effect but without the stacked effect issue, continues past the main check
		player->specialFlags &= ~SpecialFlags::lightBoardEffect;
		if(player->characterArchetype == CharacterArchetype::Boost) { return; }
	} else if(!player->specialFlags.hasAny(SpecialFlags::lightBoardEffect)) {
		return;
	}
	// player->specialFlags &= ~lightBoardEffect;

	// To-Do: Figure out why light board doesn't land on the boxes man this is wack.
	if(CurrentStage == DigitalDimension && player->qteState == PlayerState::QTE2 && player->extremeGear == ExtremeGear::LightBoard) {
		player->specialFlags &= ~SpecialFlags::lowerDecel;
		player->gearStats[player->level].topSpeed = pSpeed(195.0f); // See SetPlayerState.cpp
		player->speedCap = pSpeed(195.0f);
	} else if (player->extremeGear == ExtremeGear::LightBoard) {
		player->specialFlags |= SpecialFlags::lowerDecel;
	}

	switch(player->state) {
			// case StartLine:
			//     spfInfo->lvlTopSpeed[0] = player->gearStats[0].topSpeed;
			//     spfInfo->lvlTopSpeed[1] = player->gearStats[1].topSpeed;
			//     spfInfo->lvlTopSpeed[2] = player->gearStats[2].topSpeed;
			//     break;

        case PlayerState::Run:
		case PlayerState::Death:
			spfInfo->lightBoardEffect = FALSE;
			break;

		// case PlayerState::Jump:
		// 	if(player->last_movementFlags.hasAny(MovementFlags::boosting)) {
		// 		// player->lightBoard_flag |= 0x400;
		// 		spfInfo->lightBoardEffect = TRUE;
		// 	}
		// 	break;

		case PlayerState::Cruise:
			if(player->movementFlags.hasAny(MovementFlags::boosting)) {
				// player->lightBoard_flag |= 0x400;
				spfInfo->lightBoardEffect = TRUE;
				player->speedCap = player->gearStats[player->level].boostSpeed;
			}
			break;

		// case PlayerState::TurbulenceRide:
		case PlayerState::AttackedByPlayer:
			spfInfo->lightBoardEffect = FALSE;
			break;

		default:
			break;
	}

	// if (spfInfo->lightBoardEffect == TRUE && player->flags & 0x400) {
	//     lbl_Player_BoostEndFunction(player);
	// }

	if(spfInfo->lightBoardEffect) {
		if(CurrentStage == DigitalDimension && player->qteState == PlayerState::QTE2) { return; }
		if(player->state == PlayerState::Cruise) {
			player->gearStats[player->level].topSpeed = player->gearStats[player->level].boostSpeed;
			player->speedCap = player->gearStats[player->level].boostSpeed;
		} else if(player->state == PlayerState::Fly || player->state == PlayerState::TurbulenceRide) {
			player->gearStats[player->level].topSpeed = spfInfo->lvlTopSpeed[player->level];
			if (player->state != PlayerState::TurbulenceRide) player->speedCap = player->gearStats[player->level].boostSpeed;
		}
	} else {
		if(player->state == PlayerState::Cruise || player->state == PlayerState::Fly || player->state == PlayerState::TurbulenceRide) {
			if(CurrentStage == DigitalDimension && player->qteState == PlayerState::QTE2) { return; }
			player->gearStats[player->level].topSpeed = spfInfo->lvlTopSpeed[player->level];
			if (player->state != PlayerState::TurbulenceRide) player->speedCap = spfInfo->lvlTopSpeed[player->level];
		}
	}
}

void Player_moneyCrisis(Player *player) {
	if(!player->specialFlags.hasAny(SpecialFlags::moneyCrisis)) { return; }
	SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[player->index];

	if(player->extremeGear == ExtremeGear::CoverP) {
		const f32 rings = static_cast<f32>(player->rings);
		const f32 acceleration = rings / 150000.0f;
		if(acceleration > 0.0f) {
			player->gearStats[player->level].tier1Accel = spfInfo->tierAccel[player->level][0] + acceleration;
			player->gearStats[player->level].tier2Accel = spfInfo->tierAccel[player->level][1] + acceleration;
			player->gearStats[player->level].tier3Accel = spfInfo->tierAccel[player->level][2] + acceleration;
		}

		f32 boostSpeed = pSpeed(200.0f) + pSpeed(0.201f * rings);
		if(player->characterArchetype == CharacterArchetype::Boost) {
			boostSpeed += BoostArchetypeBoostSpeeds[player->level];
		}
		player->gearStats[player->level].boostSpeed = boostSpeed;

		if(player->state == PlayerState::Cruise || player->state == PlayerState::Fly) {
			f32 topSpeed = player->gearStats[0].topSpeed - pSpeed(13.0f) + pSpeed(0.15f * rings); // remove level up top speed bonus with -13.0f
			if(player->character == Character::Emerl) {
				topSpeed -= pSpeed(4.0f);
			}
			player->gearStats[player->level].topSpeed = topSpeed;
			if(!player->movementFlags.hasAny(MovementFlags::boosting)) {
				player->speedCap = topSpeed;
			}
		}
	} else {
		if(player->specialFlags.hasAny(SpecialFlags::ringGear)) {
			f32 &boostSpeed = player->gearptr->levelStats[player->level].boostSpeed;
			boostSpeed += pSpeed(0.4f * static_cast<f32>(player->currentAir) / 1000.0);
			if(player->characterArchetype == CharacterArchetype::Boost) {
				boostSpeed += BoostArchetypeBoostSpeeds[player->level];
			}

			if(!player->movementFlags.hasAny(MovementFlags::boosting)) {
				if(player->state == PlayerState::Cruise || player->state == PlayerState::Fly) {
					const f32 topSpeed = spfInfo->lvlTopSpeed[0] + pSpeed(0.3f * static_cast<f32>(player->currentAir) / 1000.0);
					player->gearStats[player->level].topSpeed = topSpeed;
					player->speedCap = topSpeed;
				}
			} else {
				// player->speedCap = boostSpeed;
			}
		} else {
			f32 &boostSpeed = player->gearptr->levelStats[player->level].boostSpeed;
			const f32 rings = static_cast<f32>(player->rings);
			if(!player->movementFlags.hasAny(MovementFlags::boosting)) {
				boostSpeed += pSpeed(0.5f * rings);
			}
			if(player->characterArchetype == CharacterArchetype::Boost) {
				boostSpeed += BoostArchetypeBoostSpeeds[player->level];
			}
			//player->gearStats[player->level].boostSpeed = boostSpeed;

			if(!player->movementFlags.hasAny(MovementFlags::boosting)) {
				if(player->state == PlayerState::Cruise || player->state == PlayerState::Fly) {
					const f32 topSpeed = spfInfo->lvlTopSpeed[player->level] + pSpeed(0.2f * rings);
					player->gearStats[player->level].topSpeed = topSpeed;
					player->speedCap = topSpeed;
				}
			} else {
				// player->speedCap = boostSpeed;
			}
		}
	}
}

ASMUsed f32 Player_DecelModifier(Player *player) {
	const auto &BaseDecel = data_C24748F0;// 0.92592591047287
	f32 FinalDecel = BaseDecel;
	f32 DecelModifier = 0.0f;

	if (player->state == PlayerState::RailGrind || player->state == PlayerState::Fly)
	{return FinalDecel;}

	// if(player->specialFlags.hasAny(SpecialFlags::lowerDecel)) {
	// 	if (player->extremeGear == ExtremeGear::LightBoard) {
    //         DecelModifier += 0.05f;
    //     } else {
	// 		DecelModifier += 0.1f;
	// 	}
	// }

    switch (player->characterArchetype) {
        case CharacterArchetype::Windcatcher:
			DecelModifier += 0.02f;
            break;
        case CharacterArchetype::TopSpeed:
            DecelModifier += 0.01f;
            break;
		case CharacterArchetype::LateBooster:
			if (player->movementFlags.hasAny(MovementFlags::boosting)) {DecelModifier += 0.025f;}
			break;
		case CharacterArchetype::Trickster:
			if (MI::impulseData[player->index].afterburnerTimer != 0 &&
				player->state == PlayerState::Cruise && player->trickCount > 1) {
				const f32 afterburnerTimer = static_cast<f32>(MI::impulseData[player->index].afterburnerTimer);
				const f32 maxAfterburnerTimer = static_cast<f32>(calculateAfterburnerTimer(player));
				const f32 scale = afterburnerTimer / maxAfterburnerTimer;
				DecelModifier += (player->trickCount * 0.005f) * scale;
			}
			break;
		default: break;
	}

	switch (player->extremeGear) {
		using namespace ExtremeGear;
		case Legend:
		case AirBroom:
			DecelModifier += 0.025f;
			break;
		case ExtremeGear::LightBoard:
			DecelModifier += 0.05f;
			break;
		// case Accelerator:
		// 	if(player->input->holdFaceButtons.hasAny(Buttons::A) && player->jumpCharge > 0
		// 		&& player->state == PlayerState::Cruise && !player->movementFlags.hasAny(MovementFlags::boosting)) {DecelModifier += 0.2f;}
		// 		break;
		case PowerGear:
			if (player->movementFlags.hasAny(MovementFlags::drifting)) {DecelModifier = 0.0f;}
			break;
		default: 
			if(player->specialFlags.hasAny(SpecialFlags::lowerDecel)) {DecelModifier += 0.1f;}
			break;
	}

	if (CurrentStage == DigitalDimension && player->qteState == PlayerState::QTE2) DecelModifier = 0.0f;
	FinalDecel = BaseDecel - (BaseDecel * DecelModifier);

	return FinalDecel;
}

ASMUsed u32 Player_MagnetTimer(Player *player, u32 currTime) {
	if(isSuperCharacter(*player, Character::Tails)) {
		SuperTailsInfo *stInfo = &PlayerSuperTailsInfo[player->index];
		if(player->superFormState == 1 && stInfo->playerMagnet == true) {
			stInfo->playerMagnet = false;
			currTime = 900;
			player->superFormState = 2;
		} else {
			return currTime;
		}
		// if(player->superFormState == 0) {
		// 	currTime = 1;
		// } else {// cmpwi r4, 0 : bne- lbl_C24ECA30_disable_magnet // if not in super state
		// 	if(player->superFormState == 1) {
		// 		currTime = 900;
		// 		player->superFormState = 2;
		// 	} else {
		// 		return currTime;
		// 	}
		// }
	}

	switch(player->extremeGear) {
		using namespace ExtremeGear;
		case ChaosEmerald:// player->character == Tails
			break;
		default:
			break;
	}

	return currTime;
}

ASMUsed void Player_CustomRingPickup(Player *player) {
	u8 pickedUpRings = static_cast<u8>(!player->flags.hasAny(static_cast<PlayerFlags>(0x80000)));

	switch(player->extremeGear) {
		case ExtremeGear::CoverP:
			pickedUpRings *= 3;
			break;
		case ExtremeGear::Gambler:
			// pickedUpRings *= 2;
			// switch (lbl_RNG_Number(4)) {
			// 	case 0:
			// 	case 1:
			// 		break;
			// 	case 2:
			// 	case 3:
			// 		pickedUpRings *= 2;
			// }
			break;
		default:
			break;
	}

	player->rings += pickedUpRings;
}

ASMUsed void Player_ArchetypeTierAccelBuffs(Player *player) {
    //SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[player->index];
    //f32 boostArchAccelBuff = 4.5f;
    //u32 calcBoostDuration = 0;

	f32 accelBuff = (player->characterArchetype == CharacterArchetype::Boost && player->movementFlags.hasAny(MovementFlags::boosting)) ? 4.5f : 1.0f;
    if (player->characterArchetype == CharacterArchetype::TopSpeed) {
        switch (player->level) {
            case 0:
                accelBuff = 1.15f;
                break;
            case 1:
                accelBuff = 1.25f;
				if (player->extremeGear == ExtremeGear::CoverP && player->specialFlags.hasAny(SpecialFlags::moneyCrisis))
				{accelBuff = 1.0f;}
                break;
            case 2:
                accelBuff = 1.35f;
                break;
            default:
                break;
        }
    }

    if (player->characterArchetype == CharacterArchetype::Windcatcher){
		accelBuff = 1.10f;
	}

	if(player->extremeGear == ExtremeGear::HangOn && player->characterArchetype != CharacterArchetype::Boost) {
		accelBuff = player->movementFlags.hasAny(MovementFlags::boosting) ? 4.5f : accelBuff;
	}

	// in code, it does "if bge, skip to next tier accel, so do less than comparison to apply it
	if(player->speed < player->gearStats[player->level].baseTopSpeed) {
		player->generalSpeedLoss = (player->gearStats[player->level].tier1Accel * accelBuff);
	} else if(player->speed < player->gearStats[player->level].baseAccel) {
		player->generalSpeedLoss = (player->gearStats[player->level].tier2Accel * accelBuff);
	} else if(player->speed < player->speedCap) {
		player->generalSpeedLoss = (player->gearStats[player->level].tier3Accel * accelBuff);
	} else {
		player->generalSpeedLoss = 0.0f;
	}
}

ASMUsed void Player_ApplyBoostSpeed(Player* player) {
    SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[player->index];

    if (player->characterArchetype == CharacterArchetype::Boost
	   && player->input->holdFaceButtons.hasAny(Buttons::A)
	   && player->jumpCharge > 0
	   && !player->movementFlags.hasAny(MovementFlags::drifting)
	   && player->state == PlayerState::Cruise) {
            player->speedCap = player->gearStats[player->level].topSpeed;
    } else {
			player->speedCap = player->gearStats[player->level].boostSpeed;
	}
}

ASMUsed void Player_TornadoBoostApplications(Player* player) {
	if (!player->specialFlags.hasAny(SpecialFlags::tornadoBoost)) return;
	if (player->extremeGear == ExtremeGear::GunGear) return; // No speed loss on nado
	player->speed = (player->speed < player->gearStats[player->level].boostSpeed) ? player->gearStats[player->level].boostSpeed : player->speed;
}

ASMUsed void Player_AirGainOnAttack(Player *player) {
	// Note: Ring gears should not use this at all
	if (player->specialFlags.hasAny(SpecialFlags::ringGear)) return;
	switch (player->extremeGear) {
		using namespace ExtremeGear;
		case HighBooster: 
		case GunGear:
			// Both gears get half their air tank back when an attack lands
			player->currentAir += player->gearStats[player->level].maxAir / 2;
			break;
		default:
			// The default value for all air gears to get when an attack lands (30000 = 0x7530)
			player->currentAir += 30000;
	}
}