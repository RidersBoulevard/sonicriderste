#include "specialflagtweaks.hpp"
#include "riders/stage.hpp"
#include "flags.hpp"

std::array<SpecialFlagInfo, MaxPlayerCount> PlayerSpecialFlagInfo;

void Player_storeFlagInfo(Player *player) {
	SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[player->index];
	if(player->state == StartLine) {
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
	}
}

void Player_lightBoardEffect(Player *player) {
	SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[player->index];
	if(player->extremeGear == ExtremeGear::MagicCarpet) {
		// CANNOT be allowed to stack with berserker in ASM
		// Still has light board effect but without the stacked effect issue, continues past the main check
		player->specialFlags &= ~lightBoardEffect;
		if(player->characterArchetype == BoostArchetype) { return; }
	} else if(!player->specialFlags.hasAny(lightBoardEffect)) {
		return;
	}
	// player->specialFlags &= ~lightBoardEffect;

	// To-Do: Figure out why light board doesn't land on the boxes man this is wack.
	if(CurrentStage == DigitalDimension && player->qteState == 0x11 && player->extremeGear == ExtremeGear::LightBoard) {
		player->specialFlags &= ~lowerDecel;
	} else if (player->extremeGear == ExtremeGear::LightBoard) {
		player->specialFlags |= lowerDecel;
	}

	switch(player->state) {
			// case StartLine:
			//     spfInfo->lvlTopSpeed[0] = player->gearStats[0].topSpeed;
			//     spfInfo->lvlTopSpeed[1] = player->gearStats[1].topSpeed;
			//     spfInfo->lvlTopSpeed[2] = player->gearStats[2].topSpeed;
			//     break;

        case Run:
		case Death:
			spfInfo->lightBoardEffect = FALSE;
			break;

		case Jump:
			if(player->last_movementFlags.hasAny(boosting)) {
				// player->lightBoard_flag |= 0x400;
				spfInfo->lightBoardEffect = TRUE;
			}
			break;

		case Cruise:
			if(player->movementFlags.hasAny(boosting)) {
				// player->lightBoard_flag |= 0x400;
				spfInfo->lightBoardEffect = TRUE;
				player->speedCap = player->gearStats[player->level].boostSpeed;
			}
			break;

		case TurbulenceRide:
		case AttackedByPlayer:
			spfInfo->lightBoardEffect = FALSE;
			break;

		default:
			break;
	}

	// if (spfInfo->lightBoardEffect == TRUE && player->flags & 0x400) {
	//     lbl_Player_BoostEndFunction(player);
	// }

	if(spfInfo->lightBoardEffect) {
		if(CurrentStage == DigitalDimension && player->qteState == 0x11) { return; }
		if(player->state == Cruise) {
			player->gearStats[player->level].topSpeed = player->gearStats[player->level].boostSpeed;
			player->speedCap = player->gearStats[player->level].boostSpeed;
		} else if(player->state == Fly) {
			player->gearStats[player->level].topSpeed = spfInfo->lvlTopSpeed[player->level];
			player->speedCap = player->gearStats[player->level].boostSpeed;
		}
	} else {
		if(player->state == Cruise || player->state == Fly) {
			if(CurrentStage == DigitalDimension && player->qteState == 0x11) { return; }
			player->gearStats[player->level].topSpeed = spfInfo->lvlTopSpeed[player->level];
			player->speedCap = spfInfo->lvlTopSpeed[player->level];
		}
	}
}

void Player_moneyCrisis(Player *player) {
	if(!player->specialFlags.hasAny(moneyCrisis)) { return; }
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
		if(player->characterArchetype == BoostArchetype) {
			boostSpeed += BoostArchetypeBoostSpeeds[player->level];
		}
		player->gearStats[player->level].boostSpeed = boostSpeed;

		if(player->state == Cruise || player->state == Fly) {
			f32 topSpeed = player->gearStats[0].topSpeed - pSpeed(13.0f) + pSpeed(0.15f * rings); // remove level up top speed bonus with -13.0f
			if(player->character == Emerl) {
				topSpeed -= pSpeed(4.0f);
			}
			player->gearStats[player->level].topSpeed = topSpeed;
			if(!player->movementFlags.hasAny(boosting)) {
				player->speedCap = topSpeed;
			}
		}
	} else {
		if(player->specialFlags.hasAny(ringGear)) {
			f32 &boostSpeed = player->gearptr->levelStats[player->level].boostSpeed;
			boostSpeed += pSpeed(0.4f * static_cast<f32>(player->currentAir) / 1000.0);
			if(player->characterArchetype == BoostArchetype) {
				boostSpeed += BoostArchetypeBoostSpeeds[player->level];
			}

			if(!player->movementFlags.hasAny(boosting)) {
				if(player->state == Cruise || player->state == Fly) {
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
			if(!player->movementFlags.hasAny(boosting)) {
				boostSpeed += pSpeed(0.5f * rings);
			}
			if(player->characterArchetype == BoostArchetype) {
				boostSpeed += BoostArchetypeBoostSpeeds[player->level];
			}
			//player->gearStats[player->level].boostSpeed = boostSpeed;

			if(!player->movementFlags.hasAny(boosting)) {
				if(player->state == Cruise || player->state == Fly) {
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
	const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);

	const f32 &BaseDecel = data_C24748F0;// 0.92592591047287
	f32 FinalDecel = BaseDecel;
	f32 DecelModifier = 0.0f;

	if(player->specialFlags.hasAny(lowerDecel)) {
		if (player->extremeGear == ExtremeGear::LightBoard) {
            DecelModifier += 0.05f;
        } else {
			DecelModifier += 0.1f;
		}
	}

    switch (player->characterArchetype) {
        case Windcatcher:
			DecelModifier += 0.02f;
            break;
        case TopSpeedArchetype:
            DecelModifier += 0.01f;
            break;
		case LateBooster:
			if (player->movementFlags.hasAny(boosting)) {DecelModifier += 0.025f;}
			break;
		default: break;
	}

	switch (player->extremeGear) {
		using namespace ExtremeGear;
		case AirBroom:
			DecelModifier += 0.02f;
			break;
		default: break;
	}

	FinalDecel = BaseDecel - (BaseDecel * DecelModifier);

	return FinalDecel;
}

ASMUsed u32 Player_MagnetTimer(Player *player, u32 currTime) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if(isSuperCharacter(*player, Tails)) {
		if(player->superFormState == 0) {
			currTime = 1;
		} else {// cmpwi r4, 0 : bne- lbl_C24ECA30_disable_magnet // if not in super state
			if(player->superFormState == 1) {
				currTime = 900;
				player->superFormState = 2;
			} else {
				return currTime;
			}
		}
	}

	switch(player->extremeGear) {
		using namespace ExtremeGear;
		case ChaosEmerald:// player->character == Tails
			break;
		case SuperHangOn:
			if(player->empty[0] == std::byte{1} && exLoads.gearExLoadID == E99EXLoad) {
				currTime = 1;
			}
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
		default:
			break;
	}

	player->rings += pickedUpRings;
}

ASMUsed void Player_ArchetypeTierAccelBuffs(Player *player) {
    //SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[player->index];
    //f32 boostArchAccelBuff = 4.5f;
    //u32 calcBoostDuration = 0;

    // if (player->extremeGear == HangOn && exLoads.gearExLoadID != HangOnATEXLoad
    //     && player->characterArchetype != BoostArchetype) accelBuff = (player->movementFlags & boosting) ? 4.5f : 1.0f;

	f32 accelBuff = (player->characterArchetype == BoostArchetype && player->movementFlags.hasAny(boosting)) ? 4.5f : 1.0f;
    if (player->characterArchetype == TopSpeedArchetype) {
        switch (player->level) {
            case 0:
                accelBuff = 1.15f;
                break;
            case 1:
                accelBuff = 1.25f;
				if (player->extremeGear == ExtremeGear::CoverP && player->specialFlags.hasAny(moneyCrisis))
				{accelBuff = 1.0f;}
                break;
            case 2:
                accelBuff = 1.35f;
                break;
            default:
                break;
        }
    }

    if (player->characterArchetype == Windcatcher){
		accelBuff = 1.10f;
	}

	if(player->extremeGear == ExtremeGear::HangOn && FetchEnabledEXLoadIDs(*player).gearExLoadID != HangOnATEXLoad && player->characterArchetype != BoostArchetype) {
		accelBuff = player->movementFlags.hasAny(boosting) ? 4.5f : accelBuff;
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
    EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
    SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[player->index];

    if (player->characterArchetype == BoostArchetype
	   && player->input->holdFaceButtons.hasAny(AButton)
	   && player->jumpCharge > 0
	   && !player->movementFlags.hasAny(drifting)
	   && player->state == Cruise) {
            player->speedCap = player->gearStats[player->level].topSpeed;
    } else {
			player->speedCap = player->gearStats[player->level].boostSpeed;
	}
}

void Player_TornadoBoostApplications(Player* player) {
	if (!player->specialFlags.hasAny(tornadoBoost) || isSuperCharacter(*player, Knuckles) || isSuperCharacter(*player, Tails)) return;
	if (player->extremeGear != ExtremeGear::TrapGear) return;

	if (player->speed > player->gearStats[player->level].boostSpeed
		&& player->input->toggleFaceButtons.hasAny(LButton | RButton) && player->state == Cruise) {
		player->gearStats[player->level].boostSpeed = player->speed;
	} else player->gearStats[player->level].boostSpeed = player->gearptr->levelStats[player->level].boostSpeed;
}