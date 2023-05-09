// 100% match

#include "initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "gears/supers.hpp"

inline f32 CustomBoostSpeeds(Player *player, f32 boostSpeed, s32 level) {
	if (player->extremeGear == LightBoard) {
		switch (player->characterArchetype) {
			case CombatArchetype:
				boostSpeed += pSpeed(2);
				break;
			case LateBooster:
			case NoTypeArchetype:
				boostSpeed += pSpeed(5);
				break;
		}
	} else {
		if (player->extremeGear == ChaosEmerald) {
			switch (player->character) {
				case Tails:
					boostSpeed = pSpeed(200);
					break;
				case Knuckles:
					boostSpeed = pSpeed(250);
					break;
				case Shadow:
					boostSpeed = pSpeed(300);
					break;
				case MetalSonic:
					boostSpeed = pSpeed(235);
					break;
			}
		}
	}

	if (player->characterArchetype == BoostArchetype) {
		boostSpeed += BoostArchetypeBoostSpeeds[level];
	}
	return boostSpeed;
}

inline f32 CustomTopSpeeds(Player *player, f32 topSpeed, s32 level) {
	// this top speed is additive, usually adds on top of 162 speed
	if (player->character == Emerl) {
		topSpeed += EmerlTopSpeedBonus[level];
	} else {
		if (player->extremeGear == ChaosEmerald) {
			switch (player->character) {
				case Tails:
					topSpeed = pSpeed(45);
					break;
				case Knuckles:
					topSpeed = pSpeed(37);
					break;
				case Shadow:
					topSpeed = pSpeed(55);
					break;
				case MetalSonic:
					topSpeed = pSpeed(38);
					break;
			}
		}
	}

	return topSpeed;
}

inline s32 CustomTornadoCosts(Player *player, s32 tornadoCost) {
	if (player->extremeGear == ChaosEmerald) {
		switch (player->character) {
			case Tails:
			case Knuckles:
			case Shadow:
				tornadoCost = 5500;
				break;
			case MetalSonic:
				tornadoCost = 25000;
				break;
		}
	}
	return tornadoCost;
}

inline s32 CustomBoostCosts(Player *player, s32 boostCost) {
	if (player->extremeGear == ChaosEmerald) {
		switch (player->character) {
			case Tails:
				boostCost = 27500;
				break;
			case Knuckles:
				boostCost = 11000;
				break;
			case Shadow:
				boostCost = 33000;
				break;
			case MetalSonic:
				boostCost = 25000;
				break;
		}
	}
	return boostCost;
}

inline s32 CustomPassiveAirDrain(Player *player, s32 airDrain) {
	if (player->extremeGear == ChaosEmerald) {
		switch (player->character) {
			case Tails:
			case MetalSonic:
				airDrain = 16;
				break;
			case Knuckles:
				airDrain = 9;
				break;
			case Shadow:
				airDrain = 0x15;
				break;
		}
	}
	return airDrain;
}

inline s32 CustomDriftCosts(Player *player, s32 driftCost) {
	if (player->extremeGear == ChaosEmerald) {
		switch (player->character) {
			case Tails:
				driftCost = 0xA6;
				break;
			case Knuckles:
				driftCost = 0xC;
				break;
			case MetalSonic:
				driftCost = 200;
				break;
		}
	}
	return driftCost;
}

inline s32 CustomDriftDashFrames(Player *player, s32 driftFrames) {
	if (player->extremeGear == ChaosEmerald) {
		switch (player->character) {
			case Knuckles:
				driftFrames = 0;
				break;
		}
	}
	return driftFrames;
}

inline u32 CustomSpecialFlags(Player *player, u32 specialFlags) {
	if (player->extremeGear == ChaosEmerald) {
		switch (player->character) {
			case Tails:
				specialFlags |= (iceImmunity | noSpeedLossChargingJump);
				specialFlags &= ~(ringGear | noPits | thirtyPercentAir);
				break;
			case Shadow:
				specialFlags |= stickyFingers;
				specialFlags &= ~thirtyPercentAir;
				specialFlags |= fiftyPercentAir;
				break;
			case MetalSonic:
				specialFlags &= ~(ringGear | thirtyPercentAir | noPits);
				break;

		}
	}
	return specialFlags;
}

inline f32 CustomAcceleration(Player *player, f32 acceleration) {
	if (player->extremeGear == ChaosEmerald) {
		switch (player->character) {
			case Knuckles:
				acceleration = 0.00617286f;
				break;
		}
	}
	if (player->characterArchetype == TopSpeedArchetype && player->extremeGear != CoverP) acceleration += 0.0010;
	return acceleration;
}

inline void GetCustomEXLoadWeight(u16 exLoadID, f32 &weight) {
    if (exLoadID == 0) {
        return;
    }

    f32 const newWeight = EXLoadWeights[exLoadID];
    if (newWeight != -1.0F) {
        weight = newWeight;
    }
}

ASMUsed void InitGearData(Player *arg0) {
	GearLevelStats *phi_r31;
	const LevelTypeStats *phi_r30;
	f32 topSpeed, acceleration;
	u32 specialFlags;
	f32 temp_f0;
	f32 temp_f0_2;
	f32 temp_f0_3;
	f32 temp_f0_4;
	f32 temp_f0_5;
	f32 temp_f0_6;
	f32 temp_f0_7;
	f32 temp_f0_8;
	f32 temp_f0_9;
	//f32 temp_f1;
	f32 temp_f1_2;
	f32 temp_f1_3;
	f32 temp_f1_4;
	f32 temp_f1_5;
	f32 temp_f1_6;
	f32 temp_f2;
	f32 temp_f2_2;
	f32 temp_f2_3;
	f32 temp_f2_4;
	f32 temp_f2_5;
	f32 temp_f2_6;
	f32 temp_f2_7;
	f32 temp_f2_8;
	//f32 temp_f2_9;
	f32 temp_f3;
	f32 temp_f3_2;
	f32 temp_f4;
	f32 temp_f5;
	f32 temp_f6;
	f32 temp_f7;
	f32 temp_f7_2;
	f32 temp_f7_3;
	f32 temp_f7_4;
	f32 temp_f7_5;
	f32 temp_f7_6;
	f32 temp_f7_7;
	//s32 temp_r29;
	Gear *temp_r5;
	const TypeStats *temp_r6;
	Character *temp_r7;
	PlayerGearStats *temp_r8;
	PlayerGearStats *phi_r9;
	EnabledEXLoads exLoads{};
	EnabledEXLoads *exLoadsptr = &exLoads;

	FakePlayerStruct *fakeplayer;
	s32 phi_r29;
	u32 i;
    f32 weight;

	if (arg0->input) {
		FetchEnabledEXLoadIDs(arg0, exLoads);
	} else {
		exLoadsptr = nullptr;
	}

	temp_r8 = &arg0->gearStats[0];
	//temp_f1 = 1.0f;
	temp_f6 = 2.07f;
	temp_r7 = arg0->characterptr;
	temp_r5 = arg0->gearptr;

	temp_f3 = temp_r7->speedMultiplier;
	temp_f2 = temp_r5->speedHandlingMultiplier;
	temp_f5 = 0.0115741f;

	temp_f0 = 1.0f + temp_f3 + temp_f2;
	if (exLoadsptr && exLoadsptr->characterExLoadID == E10REXLoad) {
		temp_r6 = &StatPresets[2];
	} else {
		temp_r6 = &StatPresets[temp_r7->statPreset];
	}
	temp_f1_2 = 1.0f - temp_f3;

	temp_f4 = 0.00771605f;
	temp_f3_2 = 200.0f;
	phi_r9 = temp_r8;
	phi_r30 = &temp_r6->stats[0];
	phi_r31 = &temp_r5->levelStats[0];
	phi_r29 = 0;
	do {
		/*
		if ((u8) arg0->aiControl == 1) {
			phi_r9->baseTopSpeed = (f32) 0.462963f;
			phi_r9->baseAccel = (f32) 0.717593f;
			phi_r9->topSpeed = (f32) 0.75f;
			phi_r9->tier1Accel = (f32) 0.00385802f;
			phi_r9->tier2Accel = (f32) 0.000848765f;
			phi_r9->tier3Accel = (f32) 0.000308642f;
			phi_r9->offroadSpeedCap = (f32) 50.0f;
			phi_r9->driftDashSpeed = (f32) 0.37037f;
			phi_r9->boostSpeed = (f32) 1.06481f;
		} else {
		*/
		topSpeed = CustomTopSpeeds(arg0, temp_r5->topSpeed, phi_r29);
		phi_r9->baseTopSpeed = (f32) ((temp_f0 * phi_r30->unk0) + topSpeed);
		phi_r9->baseAccel = (f32) ((temp_f0 * phi_r30->unk4) + topSpeed);
		phi_r9->topSpeed = (f32) ((temp_f0 * phi_r30->unk8) + topSpeed);
		acceleration = CustomAcceleration(arg0, temp_r5->acceleration);
		phi_r9->tier1Accel = (f32) ((temp_f1_2 * phi_r30->unkC) + acceleration);
		phi_r9->tier2Accel = (f32) ((temp_f1_2 * phi_r30->unk10) + acceleration);
		phi_r9->tier3Accel = (f32) ((temp_f1_2 * phi_r30->unk14) + acceleration);
		phi_r9->offroadSpeedCap = (f32) ((temp_f0 * phi_r30->unk18) + temp_r5->offRoadSpeed);
		phi_r9->driftDashSpeed = (f32) phi_r31->driftDashSpeed;
		phi_r9->boostSpeed = CustomBoostSpeeds(arg0, phi_r31->boostSpeed, phi_r29);
		//}
		phi_r9->maxAir = (s32) phi_r31->maxAir;
		//phi_r9->airDrain = (s32) ((f32) CustomPassiveAirDrain(arg0, phi_r31->passiveAirDrain) * gf32_SystemHzAdd1);
		//phi_r9->driftCost = (s32) ((f32) CustomDriftCosts(arg0, phi_r31->driftingAirCost) * gf32_SystemHzAdd1);

		// sometimes gf32_SystemHzAdd1 will not be 1.0 in float for some reason? so it will bug passive air drain and drift costs to insane values
		phi_r9->airDrain = (s32) ((f32) CustomPassiveAirDrain(arg0, phi_r31->passiveAirDrain));
		phi_r9->driftCost = (s32) ((f32) CustomDriftCosts(arg0, phi_r31->driftingAirCost));
		phi_r9->boostCost = CustomBoostCosts(arg0, phi_r31->boostCost);
		phi_r9->tornadoCost = CustomTornadoCosts(arg0, phi_r31->tornadoCost);
		if ((u8) temp_r7->type == 2) {
			phi_r9->airDrain = (s32) (1.3f * (f32) phi_r9->airDrain);
		}
		temp_f7 = phi_r9->baseTopSpeed;
		if (temp_f7 > temp_f6) {
			phi_r9->baseTopSpeed = temp_f6;
		} else {
			temp_f2_2 = 0.555556f;
			if (temp_f7 < temp_f2_2) {
				phi_r9->baseTopSpeed = temp_f2_2;
			}
		}
		temp_f7_2 = phi_r9->baseAccel;
		if (temp_f7_2 > temp_f6) {
			phi_r9->baseAccel = temp_f6;
		} else {
			temp_f2_3 = 0.555556f;
			if (temp_f7_2 < temp_f2_3) {
				phi_r9->baseAccel = temp_f2_3;
			}
		}
		temp_f7_3 = phi_r9->topSpeed;
		if (temp_f7_3 > temp_f6) {
			phi_r9->topSpeed = temp_f6;
		} else {
			temp_f2_4 = 0.555556f;
			if (temp_f7_3 < temp_f2_4) {
				phi_r9->topSpeed = temp_f2_4;
			}
		}
		temp_f7_4 = phi_r9->tier1Accel;
		if (temp_f7_4 > temp_f5) {
			phi_r9->tier1Accel = temp_f5;
		} else {
			temp_f2_5 = 0.00154321f;
			if (temp_f7_4 < temp_f2_5) {
				phi_r9->tier1Accel = temp_f2_5;
			}
		}
		temp_f7_5 = phi_r9->tier2Accel;
		if (temp_f7_5 > temp_f4) {
			phi_r9->tier2Accel = temp_f4;
		} else {
			temp_f2_6 = 0.000231481f;
			if (temp_f7_5 < temp_f2_6) {
				phi_r9->tier2Accel = temp_f2_6;
			}
		}
		temp_f7_6 = phi_r9->tier3Accel;
		if (temp_f7_6 > temp_f4) {
			phi_r9->tier3Accel = temp_f4;
		} else {
			temp_f2_7 = 0.000108025f;
			if (temp_f7_6 < temp_f2_7) {
				phi_r9->tier3Accel = temp_f2_7;
			}
		}
		temp_f7_7 = phi_r9->offroadSpeedCap;
		if (temp_f7_7 > temp_f3_2) {
			phi_r9->offroadSpeedCap = temp_f3_2;
		} else {
			temp_f2_8 = 10.0f;
			if (temp_f7_7 < temp_f2_8) {
				phi_r9->offroadSpeedCap = temp_f2_8;
			}
		}
		phi_r29++;
		phi_r9++;
		phi_r31++;
		phi_r30++;
	} while (phi_r29 < 3);
	fakeplayer = (FakePlayerStruct *) temp_r8;
	/*
	if ((u8) arg0->aiControl == 1) {
		fakeplayer->unkBC = (f32) 0.8f;
		fakeplayer->unkC0 = (f32) 0.0253073f;
		fakeplayer->unkC4 = (f32) 0.00959931f;
		fakeplayer->unkC8 = (f32) 0.00261799f;
		fakeplayer->unkCC = (f32) 0.0261799f;
		fakeplayer->unkD0 = (f32) 1.0472f;
		fakeplayer->unkD4 = (f32) 0.174533f;
		fakeplayer->unkD8 = (f32) 0.0244346f;
		fakeplayer->unkDC = (f32) 0.601852f;
		fakeplayer->unkE0 = (f32) -0.000462963f;
		fakeplayer->unkE4 = (f32) 2.07f;
		fakeplayer->unkE8 = 0x3C;
	} else {
	*/
	fakeplayer->unkBC = (f32) ((temp_f1_2 * temp_r6->unk54) + temp_r5->turningSpeedLoss);
	fakeplayer->unkC0 = (f32) ((temp_f1_2 * temp_r6->unk58) + temp_r5->handling);
	fakeplayer->unkC4 = (f32) ((temp_f1_2 * temp_r6->unk5C) + temp_r5->backAxelHandling);
	fakeplayer->unkC8 = (f32) ((temp_f1_2 * temp_r6->unk60) + temp_r5->frontAxelHandling);
	fakeplayer->unkCC = (f32) (0.0261799f + temp_r5->driftRadius);
	fakeplayer->unkD0 = (f32) (1.0472f + temp_r5->driftRotation);
	fakeplayer->unkD4 = (f32) (0.174533f + temp_r5->backAxelDriftRotation);
	fakeplayer->unkD8 = (f32) (0.0244346f + temp_r5->frontAxelDriftRotation);
	fakeplayer->unkDC = (f32) (0.601852f + temp_r5->unk44);
	fakeplayer->unkE0 = (f32) (-0.000462963f + temp_r5->unk48);
	fakeplayer->unkE4 = (f32) (2.07f + temp_r5->unk4C);
	fakeplayer->unkE8 = CustomDriftDashFrames(arg0, temp_r5->driftDashFrames) + 0x3C;
	//}
	//temp_f2_9 = 1.0f;
	if (exLoadsptr && exLoadsptr->characterExLoadID == E10REXLoad) {
		arg0->typeAttributes = PowerType | temp_r5->extraTypeAttributes;
	} else if (isSuperCharacter(*arg0, MetalSonic)) {
		arg0->typeAttributes = (SpeedType | PowerType);
	} else {
		arg0->typeAttributes = ((1 << temp_r7->type) | temp_r5->extraTypeAttributes);
	}
	//fakeplayer->unkB8 = (f32) (temp_r5->weight + (1.0f + temp_r7->speedMultiplier + temp_r5->speedHandlingMultiplier));
	weight = CharacterWeights[arg0->character];
    if (exLoadsptr != nullptr) {
        GetCustomEXLoadWeight(exLoadsptr->characterExLoadID, weight);
    }
    fakeplayer->unkB8 = (f32) (temp_r5->weight + weight);
	fakeplayer->unkA8 = (s32) temp_r8->maxAir;
	fakeplayer->unkAC = 0;
	fakeplayer->unkB0 = 0;
	fakeplayer->unkB4 = 0;
	fakeplayer->unkEC = (f32) (1.0f + temp_r5->trickAirGainMultiplier);
	fakeplayer->unkF0 = (f32) (1.0f + temp_r5->shortcutAirGainMultiplier);
	fakeplayer->unkF4 = (f32) (1.0f + temp_r5->QTEAirGainMultiplier);
	specialFlags = fakeplayer->unkF8 = CustomSpecialFlags(arg0, temp_r5->specialFlags);
	arg0->typeAttributes = (arg0->typeAttributes * ((((u32) specialFlags >> 0xDU) & 1) ^ 1));
	if ((specialFlags & 0x8000) != 0) {
		fakeplayer->unkA8 = ((temp_r8 + arg0->level)->maxAir * 0x1E) / 100;
	}
	if ((specialFlags & 0x10000) != 0) {
		fakeplayer->unkA8 = ((temp_r8 + arg0->level)->maxAir * 0x32) / 100;
	}
	temp_f1_3 = fakeplayer->unkBC;
	temp_f0_2 = 1.0f;
	if (temp_f1_3 > temp_f0_2) {
		fakeplayer->unkBC = temp_f0_2;
	} else {
		temp_f0_3 = 0.0f;
		if (temp_f1_3 < temp_f0_3) {
			fakeplayer->unkBC = temp_f0_3;
		}
	}
	temp_f1_4 = fakeplayer->unkC0;
	temp_f0_4 = 0.174533f;
	if (temp_f1_4 > temp_f0_4) {
		fakeplayer->unkC0 = temp_f0_4;
	} else {
		temp_f0_5 = 0.0f;
		if (temp_f1_4 < temp_f0_5) {
			fakeplayer->unkC0 = temp_f0_5;
		}
	}
	temp_f1_5 = fakeplayer->unkC4;
	temp_f0_6 = 0.174533f;
	if (temp_f1_5 > temp_f0_6) {
		fakeplayer->unkC4 = temp_f0_6;
	} else {
		temp_f0_7 = 0.0f;
		if (temp_f1_5 < temp_f0_7) {
			fakeplayer->unkC4 = temp_f0_7;
		}
	}
	temp_f1_6 = fakeplayer->unkC8;
	temp_f0_8 = 0.174533f;
	if (temp_f1_6 > temp_f0_8) {
		fakeplayer->unkC8 = temp_f0_8;
		return;
	}
	temp_f0_9 = 0.0f;
	if (temp_f1_6 < temp_f0_9) {
		fakeplayer->unkC8 = temp_f0_9;
	}

	for (i = 0; i < 8; i++) {
		SuperNeoMetalHUDColors[i] = 0x1239B8FF;
	}
}