#include "context.hpp"
#include "handlers/player/SetPlayerState.hpp"
#include "lib/sound.hpp"

void func_SetPlayerActionType(Player *player, u32 state) {
	f32 temp_f0;
	f32 temp_f0_10;
	f32 temp_f0_11;
	f32 temp_f0_12;
	f32 temp_f0_13;
	f32 temp_f0_14;
	f32 temp_f0_15;
	f32 temp_f0_2;
	f32 temp_f0_3;
	f32 temp_f0_4;
	f32 temp_f0_5;
	f32 temp_f0_6;
	f32 temp_f0_7;
	f32 temp_f0_8;
	f32 temp_f0_9;
	f32 temp_f2;
	f32 temp_f2_2;
	f32 temp_f2_3;
	f32 temp_f2_4;
	s32 temp_r30;
	s32 temp_r5_2;
	s32 temp_r7_2;
	u8 temp_r0;
	u8 temp_r0_2;
	u8 temp_r28;
	u8 temp_r3_2;
	u8 temp_r5;
	u8 temp_r7;
	u8 temp_r8;
	u8 temp_r9;
	ParticleObject1 *temp_r10;
	PlayerCameraStruct *temp_r3;
	u32 phi_r5;
	f32 phi_f2;
	f32 phi_f2_2;
	f32 phi_f1;
	struct PlayerGearStats *gearStats;
	u32 gearSpecialFlags;
	u32 i;


	temp_r7 = player->state;
	temp_r5 = player->unk1037;
	if (((temp_r7 == 3) & ((u32) ((4 - temp_r5) | (temp_r5 - 4)) >> 0x1FU)) == 0) {
		if (temp_r7 == 0x1B) {
			lbl_00187ED0(player);
		}
		player->unk1044 = (u8) player->qteState;
		player->qteState = (u8) player->previousState;
		player->previousState = (u8) player->state;
		player->state = state;
		player->unk1037 = 0;
		player->unkAB8 = (f32) -0.138889f;
		phi_r5 = 0;
		if (((player->lightBoard_flag & 0x200) != 0) && ((u8) player->state != 5)) {
			phi_r5 = 1;
		}
		temp_r0 = player->previousState;
		if (((temp_r0 == 0x16) | (((player->movementFlags >> 0xAU) & 1) | (temp_r0 == 0x15)) | phi_r5) != 0) {
			temp_r8 = player->state;
			if ((((u32) ((0x16 - temp_r8) | (temp_r8 - 0x16)) >> 0x1FU) &
				 (((u32) ((6 - temp_r8) | (temp_r8 - 6)) >> 0x1FU) &
				  ((u32) ((0x15 - temp_r8) | (temp_r8 - 0x15)) >> 0x1FU))) != 0) {
				lbl_Player_BoostEndFunction(player);
			}
		}
		player->unkB10 = 1;
		if ((player->flags & 0x2800) == 0) {
			gearStats = &player->gearStats[player->level];
			temp_f2 = gearStats->topSpeed;
			//phi_f2 = temp_f2;
			if ((u8) player->playerType == 1) {
				temp_f0 = 0.231481f;
				temp_f2 = temp_f2 + player->unk7F8;
				//phi_f2 = temp_f2_2;
				if (temp_f2 < temp_f0) {
					temp_f2 = temp_f0;
				} else {
					temp_f0_2 = 1.38889f;
					if (temp_f2 > temp_f0_2) {
						temp_f2 = temp_f0_2;
					}
				}
			}
			player->speedCap = temp_f2;
		}
		if ((s32) player->currentAir > 0) {
			player->flags = (u32) (player->flags & 0xFFFFFEFF);
			player->flags = (u32) (player->flags | 4);
		}
		switch (state) {
			case 0:
				//temp_f0_3 = 0.0f;
				gearSpecialFlags = player->gearptr->specialFlags;
				player->unkB08 = 0x80F;
				player->unkB08 = (s32) (player->unkB08 & (-1 - (((gearSpecialFlags >> 4U) & 1) * 0x2400)));
				player->unkB2C = 0.0f;
				player->unkB28 = 0.0f;
				player->unkB24 = 0.0f;
				break;
			case 5:
				if ((player->lightBoard_flag & 0x200) != 0) {
					player->state = 0x16U;
					lbl_0009195C(player);
				} else {
					gearSpecialFlags = player->gearptr->specialFlags;
					player->unkB08 = 0x28F70F;
					player->unkB08 = (s32) (player->unkB08 & (-1 - (((gearSpecialFlags >> 4U) & 1) * 0x2400)));
					player->flags = (u32) (player->flags | 4);
					player->flags = (u32) (player->flags & 0x022A092F);
					player->jumpCharge = 0;
					player->unkD9E = -1;
					player->subState = (s8) 0;
					temp_r9 = player->previousState;
					if (((temp_r9 == 0xB) | ((temp_r9 == 0xA) | ((temp_r9 == 9) | ((temp_r9 == 8) | ((temp_r9 == 6) |
																									 (temp_r9 ==
																									  7)))))) != 0) {
						player->unkD8C = (s32) -1;
					}
					if (((1 << player->previousState) & 0x7F80) != 0) {
						temp_r3 = &PlayerCameraStruct + player->index;
						temp_r3->cameraPreset = 0xFF;
						temp_r3->cameraPresetProperty = 0;
					}
				}
				break;
			case 7:
				gearStats = &player->gearStats[player->level];
				temp_f2_4 = gearStats->topSpeed;
				//phi_f2_2 = temp_f2_3;
				if ((u8) player->playerType == 1) {
					temp_f0_4 = 0.231481f;
					temp_f2_4 += player->unk7F8;
					//phi_f2_2 = temp_f2_4;
					if (temp_f2_4 < temp_f0_4) {
						temp_f2_4 = temp_f0_4;
					} else {
						temp_f0_5 = 1.38889f;
						if (temp_f2_4 > temp_f0_5) {
							temp_f2_4 = temp_f0_5;
						}
					}
				}
				temp_f0_6 = 0.833333f;
				phi_f1 = temp_f2_4;
				if (phi_f1 < temp_f0_6) {
					phi_f1 = temp_f0_6;
				}
				if (((player->flags & 0x4000) != 0) || ((u8) player->previousState == 0x10)) {
					player->gravity = (f32) (0.1f * -((0.33f * (player->speed / phi_f1)) - 0.8f));
				} else {
					temp_f0_7 = player->speed;
					if (temp_f0_7 > phi_f1) {
						player->gravity = (f32) 0.007f;
					} else {
						player->gravity = (f32) (0.1f * -((0.33f * (temp_f0_7 / phi_f1)) - 0.4f));
					}
				}
				temp_f0_8 = gf32_SystemHzAdd1;
				player->gravity = (f32) (player->gravity * (temp_f0_8 * temp_f0_8));
				gearSpecialFlags = player->gearptr->specialFlags;
				player->unkB08 = 0x9000F;
				player->unkB08 = (s32) (player->unkB08 & (-1 - ((((u32) gearSpecialFlags >> 4U) & 1) * 0x2400)));
				break;
			case 8:
			case 9:
			case 10:
			case 11:
				temp_f0_9 = 0.0f;
				player->unkB2C = temp_f0_9;
				player->unkB28 = temp_f0_9;
				player->unkB24 = temp_f0_9;
				/* fallthrough */
			case 12:
				temp_f0_10 = 0.0f;
				gearSpecialFlags = player->gearptr->specialFlags;
				player->unkB08 = 0x9040F;
				player->unkB08 = (s32) (player->unkB08 & (-1 - ((((u32) gearSpecialFlags >> 4U) & 1) * 0x2400)));
				player->flags = (u32) (player->flags & 0xFFFFFEFF);
				player->unk9F7 = 0;
				player->unkA00 = 0;
				player->unk590 = temp_f0_10;
				player->unk58C = temp_f0_10;
				player->unk5AC = temp_f0_10;
				player->unk5A8 = temp_f0_10;
				player->unk5C8 = temp_f0_10;
				player->unk5C4 = temp_f0_10;
				if ((u8) player->aiControl == 0) {
					lbl_10087C40 = gu32GameCnt;
					PlaySound(0xA901, 0x02);
				}
				if ((RuleSettings & 0x400000) == 0) {
					if ((player->specialFlags & 2) != 0) {
						switch (player->rampType) {
							case 0:
							case 1:
							case 2:
								player->trickTrajectory = 4U;
								player->startingTrick = (u8) player->rampType;
								break;
							case 3:
							case 4:
								player->trickTrajectory = 3U;
								break;
						}
					}
					temp_r3_2 = player->unk9FD;
					if (temp_r3_2 != 0) {
						player->trickTrajectory = (u8) (temp_r3_2 - 1);
					}
				} else {
					if ((((u32) (2 - player->trickTrajectory) >> 0x1FU) &
						 ((CurrentStage == 6) & (player->rampType == 1) &
						  ((CurrentGameMode == 0x190) | (CurrentGameMode == 0x12C)))) != 0) {
						player->trickTrajectory = 2U;
					}
				}
				break;
			case 13:
			case 14:
				for (i = 0; i < 2; i++) {
					player->secondUnk[i] = player->firstUnk[i];
				}
				player->unk1064 = (u8) player->greenCave_subState;
				if ((player->specialFlags & 2) != 0) {
					player->trickTrajectory = 3U;
				}
				break;
			case 6:
				gearSpecialFlags = player->gearptr->specialFlags;
				player->unkB08 = 0x9000F;
				player->unkB08 = (s32) (player->unkB08 & (-1 - ((((u32) gearSpecialFlags >> 4U) & 1) * 0x2400)));
				if ((u8) player->aiControl == 0) {
					lbl_10087C40 = gu32GameCnt;
					PlaySound(0xA901, 0x02);
				}
				break;
			case 17:
				temp_f0_11 = 0.0f;
				gearSpecialFlags = player->gearptr->specialFlags;
				player->unkB08 = 0x80C00;
				player->unkB08 = (s32) (player->unkB08 & (-1 - ((((u32) gearSpecialFlags >> 4U) & 1) * 0x2400)));
				player->unkB2C = temp_f0_11;
				player->unkB28 = temp_f0_11;
				player->unkB24 = temp_f0_11;
				break;
			case 18:
				temp_f0_12 = 0.0f;
				gearSpecialFlags = player->gearptr->specialFlags;
				player->unkB08 = 0x80E00;
				player->unkB08 = (s32) (player->unkB08 & (-1 - ((((u32) gearSpecialFlags >> 4U) & 1) * 0x2400)));
				player->unkB2C = temp_f0_12;
				player->unkB28 = temp_f0_12;
				player->unkB24 = temp_f0_12;
				break;
			case 19:
				gearSpecialFlags = player->gearptr->specialFlags;
				player->unkB08 = 0xA040F;
				player->unkB08 = (s32) (player->unkB08 & (-1 - ((((u32) gearSpecialFlags >> 4U) & 1) * 0x2400)));
				break;
			case 22:
				temp_f0_13 = 0.0f;
				gearSpecialFlags = player->gearptr->specialFlags;
				player->unkB08 = 0;
				player->unkB08 = (s32) (player->unkB08 & (-1 - ((((u32) gearSpecialFlags >> 4U) & 1) * 0x2400)));
				player->flags = (u32) (player->flags | 0x100000);
				player->unkB2C = temp_f0_13;
				player->unkB28 = temp_f0_13;
				player->unkB24 = temp_f0_13;
				break;
			case 23:
				temp_f0_14 = 0.0f;
				gearSpecialFlags = player->gearptr->specialFlags;
				player->unkB08 = 0;
				player->unkB08 = (s32) (player->unkB08 & (-1 - ((((u32) gearSpecialFlags >> 4U) & 1) * 0x2400)));
				player->unkB2C = temp_f0_14;
				player->unkB28 = temp_f0_14;
				player->unkB24 = temp_f0_14;
				break;
			case 2:
				gearSpecialFlags = player->gearptr->specialFlags;
				player->unkB08 = 0;
				player->unkB08 = (s32) (player->unkB08 & (-1 - ((((u32) gearSpecialFlags >> 4U) & 1) * 0x2400)));
				break;
			case 21:
				player->flags = (u32) (player->flags | 0x100000);
				gearSpecialFlags = player->gearptr->specialFlags;
				player->unkB08 = 0;
				player->unkB08 = (s32) (player->unkB08 & (-1 - ((((u32) gearSpecialFlags >> 4U) & 1) * 0x2400)));
				break;
			case 26:
			case 27:
			case 28:
			case 29:
				temp_f0_15 = 0.0f;
				gearSpecialFlags = player->gearptr->specialFlags;
				player->unkB08 = 0;
				player->unkB08 = (s32) (player->unkB08 & (-1 - ((((u32) gearSpecialFlags >> 4U) & 1) * 0x2400)));
				player->unkB2C = temp_f0_15;
				player->unkB28 = temp_f0_15;
				player->unkB24 = temp_f0_15;
				break;
			case 25:
				player->unkB10 = 1;
				gearSpecialFlags = player->gearptr->specialFlags;
				// player->unkB08 = 0xF; // original line

				// C24CDFE4, gives ability to jump in run
				player->unkB08 = 0x8170F;

				player->unkB08 = (s32) (player->unkB08 & (-1 - ((((u32) gearSpecialFlags >> 4U) & 1) * 0x2400)));
				break;
			case 16:
				if ((player->flags & 0x100) != 0) {
					func_ClearPlayerMotionTable(player);
					func__RegistPlayerMotionTable(player, 1, 0, 10.0f, 0.0f);
					func__RegistPlayerMotionTable(player, 0x46, 8, 10.0f, 0.0f);
					func_StartPlayerMotionTable(player);
				}
				gearSpecialFlags = player->gearptr->specialFlags;
				player->unkB08 = 0x1C0603;
				player->unkB08 = (s32) (player->unkB08 & (-1 - ((((u32) gearSpecialFlags >> 4U) & 1) * 0x2400)));
				func_GetDirectTrickBonus(player, 1);
				player->flags = (u32) (player->flags & 0xFFFF7F7F);
				player->trickFail = (u8) (player->trickFail | 8);
				player->unk1033 = 0;
				break;
			case 3:
				if ((s32) CurrentGameMode == 0x190) {
					lbl_000FC114(player);
				}
				break;
		}

		if ((((u32) player->unkBB4 >> 8U) & 1 & ((((u32) player->flags >> 8U) & 1) ^ 1) &
			 ((&Gears + player->extremeGear)->model == 0xF)) != 0) {
			struct MagicCarpetParticle *mcparticle = &gsParFullParam_MagicCarpet;

			temp_r28 = player->index;
			temp_r30 = gpsTexList_Particle;

			temp_r10 = ((struct ParticleObject *) SetTask(&func_Particle_Task, gsParFullParam_MagicCarpet.unk6,
														  2))->object1;

			temp_r10->unk72 = temp_r28;
			temp_r10->unk0 = (s32) mcparticle->unk74;
			temp_r10->unk4 = (s32) mcparticle->unk78;
			temp_r10->unk8 = (s32) mcparticle->unk7C;
			temp_r10->unk30 = (s32) mcparticle->unk98;
			temp_r10->unk34 = (s32) mcparticle->unk9C;
			temp_r10->unk38 = (s32) mcparticle->unkA0;
			temp_r10->unk3C = (s32) mcparticle->unk8C;
			temp_r10->unk40 = (s32) mcparticle->unk90;
			temp_r10->unk44 = (s32) mcparticle->unk94;
			temp_r10->unk10 = (s32) mcparticle->unk80;
			temp_r10->unk14 = (s32) mcparticle->unk84;
			temp_r10->unk18 = (s32) mcparticle->unk88;
			temp_r10->unk48 = &gcosNnSystemVecZeroFast;
			temp_r10->unk60 = mcparticle;
			temp_r10->texList = temp_r30;
			temp_r10->baseModelMatrix = (void *) &player->unkC4;
			temp_r10->unk73 = -1;
			temp_r10->unk74 = 0;
			player->unkBAC = (s32) (player->unkBAC | 0x4000);
			player->unk1065 = 0x1E;
		}
	}
}
