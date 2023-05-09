// 100% match

#include "context.hpp"

extern f32 gf32_SystemHzAdd1;
// extern ? lbl_001CE810; // rodata

struct LevelTypeStats {
	/* 0x0 */ f32 unk0;
	/* 0x4 */ f32 unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ f32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
} LevelTypeStats;

struct TypeStats {
	struct LevelTypeStats stats[3];
	/* 0x54 */ f32 unk54;
	/* 0x58 */ f32 unk58;
	/* 0x5C */ f32 unk5C;
	/* 0x60 */ f32 unk60;
} TypeStats;

extern const struct TypeStats StatPresets[3];

struct FakePlayerStruct {
	PlayerGearStats stats[3];
	/* 0xA8 */ s32 unkA8;
	/* 0xAC */ s32 unkAC;
	/* 0xB0 */ s32 unkB0;
	/* 0xB4 */ s32 unkB4;
	/* 0xB8 */ f32 unkB8;
	/* 0xBC */ f32 unkBC;
	/* 0xC0 */ f32 unkC0;
	/* 0xC4 */ f32 unkC4;
	/* 0xC8 */ f32 unkC8;
	/* 0xCC */ f32 unkCC;
	/* 0xD0 */ f32 unkD0;
	/* 0xD4 */ f32 unkD4;
	/* 0xD8 */ f32 unkD8;
	/* 0xDC */ f32 unkDC;
	/* 0xE0 */ f32 unkE0;
	/* 0xE4 */ f32 unkE4;
	/* 0xE8 */ s32 unkE8;
	/* 0xEC */ f32 unkEC;
	/* 0xF0 */ f32 unkF0;
	/* 0xF4 */ f32 unkF4;
	/* 0xF8 */ u32 unkF8;
	/* 0xFC */ f32 unkFC;
} _PlayerStruct;

void InitGearData(struct Player *arg0) {
	struct GearLevelStats *phi_r31;
	const struct LevelTypeStats *phi_r30;
	f32 temp_f0;
	f32 temp_f0_2;
	f32 temp_f0_3;
	f32 temp_f0_4;
	f32 temp_f0_5;
	f32 temp_f0_6;
	f32 temp_f0_7;
	f32 temp_f0_8;
	f32 temp_f0_9;
	f32 temp_f1;
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
	f32 temp_f2_9;
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
	s32 temp_r29;
	struct Gear *temp_r5;
	const struct TypeStats *temp_r6;
	struct Character *temp_r7;
	struct PlayerGearStats *temp_r8;
	struct PlayerGearStats *phi_r9;

	struct FakePlayerStruct *fakeplayer;
	s32 phi_r29;

	temp_r8 = &arg0->gearStats[0];
	//temp_f1 = 1.0f;
	temp_f6 = 2.07f;
	temp_r7 = arg0->characterptr;
	temp_r5 = arg0->gearptr;

	temp_f3 = temp_r7->speedMultiplier;
	temp_f2 = temp_r5->speedHandlingMultiplier;
	temp_f5 = 0.0115741f;

	temp_f0 = 1.0f + temp_f3 + temp_f2;
	temp_r6 = &StatPresets[temp_r7->statPreset];
	temp_f1_2 = (1.0f - temp_f3) - temp_f2;

	temp_f4 = 0.00771605f;
	temp_f3_2 = 200.0f;
	phi_r9 = temp_r8;
	phi_r30 = &temp_r6->stats[0];
	phi_r31 = &temp_r5->levelStats[0];
	phi_r29 = 0;
	do {
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
			phi_r9->baseTopSpeed = (f32) ((temp_f0 * phi_r30->unk0) + temp_r5->topSpeed);
			phi_r9->baseAccel = (f32) ((temp_f0 * phi_r30->unk4) + temp_r5->topSpeed);
			phi_r9->topSpeed = (f32) ((temp_f0 * phi_r30->unk8) + temp_r5->topSpeed);
			phi_r9->tier1Accel = (f32) ((temp_f1_2 * phi_r30->unkC) + temp_r5->acceleration);
			phi_r9->tier2Accel = (f32) ((temp_f1_2 * phi_r30->unk10) + temp_r5->acceleration);
			phi_r9->tier3Accel = (f32) ((temp_f1_2 * phi_r30->unk14) + temp_r5->acceleration);
			phi_r9->offroadSpeedCap = (f32) ((temp_f0 * phi_r30->unk18) + temp_r5->offRoadSpeed);
			phi_r9->driftDashSpeed = (f32) phi_r31->driftDashSpeed;
			phi_r9->boostSpeed = (f32) phi_r31->boostSpeed;
		}
		phi_r9->maxAir = (s32) phi_r31->maxAir;
		phi_r9->airDrain = (s32) ((f32) phi_r31->passiveAirDrain * gf32_SystemHzAdd1);
		phi_r9->driftCost = (s32) ((f32) phi_r31->driftingAirCost * gf32_SystemHzAdd1);
		phi_r9->boostCost = (s32) phi_r31->boostCost;
		phi_r9->tornadoCost = (s32) phi_r31->tornadoCost;
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
	fakeplayer = (struct FakePlayerStruct *) temp_r8;
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
		fakeplayer->unkE8 = (s32) (temp_r5->driftDashFrames + 0x3C);
	}
	//temp_f2_9 = 1.0f;
	arg0->typeAttributes = ((1 << temp_r7->type) | temp_r5->extraTypeAttributes);
	fakeplayer->unkB8 = (f32) (temp_r5->weight + (1.0f + temp_r7->speedMultiplier + temp_r5->speedHandlingMultiplier));
	fakeplayer->unkA8 = (s32) temp_r8->maxAir;
	fakeplayer->unkAC = 0;
	fakeplayer->unkB0 = 0;
	fakeplayer->unkB4 = 0;
	fakeplayer->unkEC = (f32) (1.0f + temp_r5->trickAirGainMultiplier);
	fakeplayer->unkF0 = (f32) (1.0f + temp_r5->shortcutAirGainMultiplier);
	fakeplayer->unkF4 = (f32) (1.0f + temp_r5->QTEAirGainMultiplier);
	fakeplayer->unkF8 = (u32) temp_r5->specialFlags;
	arg0->typeAttributes = (arg0->typeAttributes * ((((u32) temp_r5->specialFlags >> 0xDU) & 1) ^ 1));
	if ((temp_r5->specialFlags & 0x8000) != 0) {
		fakeplayer->unkA8 = (u32) (((temp_r8 + arg0->level)->maxAir * 0x1E) / 100);
	}
	if ((temp_r5->specialFlags & 0x10000) != 0) {
		fakeplayer->unkA8 = (u32) (((temp_r8 + arg0->level)->maxAir * 0x32) / 100);
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
}

const struct TypeStats StatPresets[3] = {
		{
				0.462962955236435f,
				0.717592597007751f,
				0.75f,
				0.00385802472010255f,
				0.000848765426781028f,
				0.000308641989249736f,
				50,
				0.462962955236435f,
				0.717592597007751f,
				0.810185194015503f,
				0.00385802472010255f,
				0.000848765426781028f,
				0.000308641989249736f,
				70,
				0.532407402992249f,
				0.736111104488373f,
				0.870370388031006f,
				0.00441358052194119f,
				0.000563271576538682f,
				0.000540123437531292f,
				70,
				0.800000011920929f,
				0.0253072753548622f,
				0.00959931127727032f,
				0.00261799385771155f
		},
		{
				0.648148119449615f,
				0.671296298503876f,
				0.717592597007751f,
				0.00555555615574121f,
				0.000254629616392776f,
				0.000123456789879128f,
				40,
				0.648148119449615f,
				0.671296298503876f,
				0.777777791023254f,
				0.00555555615574121f,
				0.000794753083027899f,
				0.000123456789879128f,
				60,
				0.671296298503876f,
				0.717592597007751f,
				0.837962985038757f,
				0.00624999590218067f,
				0.00115740741603076f,
				0.000231481477385387f,
				60,
				0.899999976158142f,
				0.0253072753548622f,
				0.00959931127727032f,
				0.00261799385771155f
		},
		{
				0.370370358228683f,
				0.671296298503876f,
				0.731481492519379f,
				0.0030864195432514f,
				0.000895061704795808f,
				0.000578703708015382f,
				80,
				0.462962955236435f,
				0.671296298503876f,
				0.791666686534882f,
				0.00385802472010255f,
				0.000640432117506862f,
				0.000192901221453212f,
				80,
				0.462962955236435f,
				0.671296298503876f,
				0.851851880550385f,
				0.00385802472010255f,
				0.000640432117506862f,
				0.000578703708015382f,
				90,
				0.800000011920929f,
				0.0253072753548622f,
				0.00959931127727032f,
				0.00261799385771155f
		},
};