#include "SetPlayerState.hpp"
#include "lib/sound.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-magic-numbers"
void func_SetPlayerActionType(Player *playerPtr, u32 stateInt) {
	constexpr f32 zero = 0.0F;
	auto state = static_cast<PlayerState>(stateInt);
	u8 temp_r0;
	//u8 temp_r0_2;
	u8 temp_r3_2;
	u8 temp_r5;
	u8 temp_r7;
	u8 temp_r8;
	PlayerCameraStruct *temp_r3;
	u32 phi_r5;
	//f32 phi_f2;
	//f32 phi_f2_2;
	f32 phi_f1;
	PlayerGearStats *gearStats;
	u32 gearSpecialFlags;
	u32 i;

	Player &player = *playerPtr;
	temp_r7 = player.state;
	temp_r5 = player.unk1037;
	if(((temp_r7 == 3) & (static_cast<u32>((4 - temp_r5) | (temp_r5 - 4)) >> 0x1FU)) == 0) {
		if(temp_r7 == 0x1B) {
			lbl_00187ED0(playerPtr);
		}
		player.unk1044 = player.qteState;
		player.qteState = player.previousState;
		player.previousState = player.state;
		player.state = state;
		player.unk1037 = 0;
		player.unkAB8 = -0.138889F;
		phi_r5 = 0;
		if(((player.lightBoard_flag & 0x200) != 0) && (player.state != 5)) {
			phi_r5 = 1;
		}
		temp_r0 = player.previousState;
		if((static_cast<u32>(temp_r0 == 0x16) | (((player.movementFlags >> 0xAU) & 1) | static_cast<u32>(temp_r0 == 0x15)) | phi_r5) != 0) {
			temp_r8 = player.state;
			//if ((((u32) ((0x16 - temp_r8) | (temp_r8 - 0x16)) >> 0x1FU) & (((u32) ((6 - temp_r8) | (temp_r8 - 6)) >> 0x1FU) & ((u32) ((0x15 - temp_r8) | (temp_r8 - 0x15)) >> 0x1FU))) != 0) {
			// if (((((u32) ((AttackedByPlayer - temp_r8) | (temp_r8 - AttackedByPlayer))) >> 0x1FU) & (((u32) ((AttackingPlayer - temp_r8) | (temp_r8 - AttackingPlayer))) >> 0x1FU)) != 0) {
			//     lbl_Player_BoostEndFunction(player);
			// }
			if(player.characterArchetype == BoostArchetype) {
				if(temp_r8 == Run || temp_r8 == TurbulenceRide || player.previousState == Death) {
					lbl_Player_BoostEndFunction(playerPtr);
				}
			} else if(((static_cast<u32>((0x16 - temp_r8) | (temp_r8 - 0x16)) >> 0x1FU) & ((static_cast<u32>((6 - temp_r8) | (temp_r8 - 6)) >> 0x1FU) & (static_cast<u32>((0x15 - temp_r8) | (temp_r8 - 0x15)) >> 0x1FU))) != 0) {
				lbl_Player_BoostEndFunction(playerPtr);
			}
		}
		player.unkB10 = 1;
		if((player.flags & 0x2800) == 0) {
			gearStats = &player.gearStats[player.level];
			f32 temp_f2 = (CurrentStage == DigitalDimension && player.qteState == 0x11) ? 0.9F : gearStats->topSpeed;
			if(player.playerType) {
				const f32 temp_f0 = 0.231481F;
				temp_f2 = temp_f2 + player.unk7F8;
				if(temp_f2 < temp_f0) {
					temp_f2 = temp_f0;
				} else {
					const f32 temp_f0_2 = 1.38889F;
					if(temp_f2 > temp_f0_2) {
						temp_f2 = temp_f0_2;
					}
				}
			}
			player.speedCap = temp_f2;
		}
		if(player.currentAir > 0) {
			player.flags &= 0xFFFFFEFF;
			player.flags |= 4;
		}
		switch(state) {
			case PlayerState::QTE:
				gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0x80F;
				player.unkB08 = player.unkB08 & (~0U - (((gearSpecialFlags >> 4U) & 1) * 0x2400));
				player.unkB2C = zero;
				player.unkB28 = zero;
				player.unkB24 = zero;
				break;
			case PlayerState::Cruise:
				if((player.lightBoard_flag & 0x200) != 0) {
					player.state = AttackedByPlayer;
					lbl_0009195C(playerPtr);
				} else {
					gearSpecialFlags = player.specialFlags;
					player.unkB08 = 0x28F70F;
					player.unkB08 = player.unkB08 & (~0U - (((gearSpecialFlags >> 4U) & 1) * 0x2400));
					player.flags = player.flags | 4;
					player.flags = player.flags & 0x022A092F;
					player.jumpCharge = 0;
					player.unkD9E = -1;
					player.subState = 0;

					switch(player.previousState){
						case 6 ... 0xB:
							player.unkD8C = -1;
							break;
						default: break;
					}
					//temp_r9 = player.previousState;
					//if(((temp_r9 == 0xB) | ((temp_r9 == 0xA) | ((temp_r9 == 9) | ((temp_r9 == 8) | ((temp_r9 == 6) | (temp_r9 == 7)))))) != 0) {
					//	player.unkD8C = -1;
					//}
					if(((1 << player.previousState) & 0x7F80) != 0) {
						temp_r3 = &playerCameraStruct[player.index];
						temp_r3->cameraPreset = 0xFF;
						temp_r3->cameraPresetProperty = 0;
					}
				}
				break;
			case PlayerState::Fall: {
				gearStats = &player.gearStats[player.level];
				f32 temp_f2_4 = gearStats->topSpeed;
				if(player.playerType) {
					const f32 temp_f0_4 = 0.231481F;
					temp_f2_4 += player.unk7F8;
					if(temp_f2_4 < temp_f0_4) {
						temp_f2_4 = temp_f0_4;
					} else {
						const f32 temp_f0_5 = 1.38889F;
						if(temp_f2_4 > temp_f0_5) {
							temp_f2_4 = temp_f0_5;
						}
					}
				}
				const f32 temp_f0_6 = 0.833333F;
				phi_f1 = temp_f2_4;
				if(phi_f1 < temp_f0_6) {
					phi_f1 = temp_f0_6;
				}
				if(((player.flags & 0x4000) != 0) || (player.previousState == 0x10)) {
					player.gravity = 0.1f * -((0.33f * (player.speed / phi_f1)) - 0.8F);
				} else {
					const f32 temp_f0_7 = player.speed;
					if(temp_f0_7 > phi_f1) {
						player.gravity = 0.007f;
					} else {
						player.gravity = 0.1f * -((0.33f * (temp_f0_7 / phi_f1)) - 0.4f);
					}
				}
				const f32 temp_f0_8 = gf32_SystemHzAdd1;
				player.gravity *= temp_f0_8 * temp_f0_8;
				gearSpecialFlags = player.gearptr->specialFlags;
				if(player.characterArchetype == BoostArchetype) {
					player.unkB08 = 0x9040F;
				} else {
					player.unkB08 = 0x9000F;
				}
				player.unkB08 = player.unkB08 & (~0U - (((gearSpecialFlags >> 4U) & 1) * 0x2400));
				break;
			}
			case PlayerState::FrontflipRamp:
			case PlayerState::BackflipRamp:
			case PlayerState::UnknownTrick:
			case PlayerState::HalfPipeTrick: {
				player.unkB2C = zero;
				player.unkB28 = zero;
				player.unkB24 = zero;
				[[fallthrough]];
			}
			case PlayerState::ManualRamp: {
				gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0x9040F;
				player.unkB08 = player.unkB08 & (~0U - (((gearSpecialFlags >> 4U) & 1) * 0x2400));
				player.flags = player.flags & 0xFFFFFEFF;
				player.unk9F7 = 0;
				player.unkA00 = 0;
				player.unk590 = zero;
				player.unk58C = zero;
				player.unk5AC = zero;
				player.unk5A8 = zero;
				player.unk5C8 = zero;
				player.unk5C4 = zero;
				if(!player.aiControl) {
					lbl_10087C40 = static_cast<s32>(gu32GameCnt);
					Sound::PlaySound(Sound::ID::IDKSFX, 0x02);
				}
				if((RuleSettings & 0x400000) == 0) {
					gearSpecialFlags = player.specialFlags;
					if((isSuperCharacter(player, Knuckles) && player.last_movementFlags & boosting) || (CurrentStage == DarkDesert && player.state == BackflipRamp)) {
						gearSpecialFlags |= legendEffect;
					}
					if((gearSpecialFlags & legendEffect) != 0) {
						switch(player.rampType) {
							case 0:
							case 1:
							case 2:
								if(CurrentGameMode == TagMode) {
									player.trickTrajectory = 3U;
								} else {
									player.trickTrajectory = 4U;
								}
								player.startingTrick = player.rampType;
								break;
							case 3:
							case 4:
								player.trickTrajectory = 3U;
								break;
							default:
								break;
						}
					}
					temp_r3_2 = player.unk9FD;
					if(temp_r3_2 != 0) {
						player.trickTrajectory = temp_r3_2 - 1;
					}
				} else {
					if(((static_cast<u32>(2 - player.trickTrajectory) >> 0x1FU) & ((CurrentStage == 6) & (player.rampType == 1) & ((CurrentGameMode == 0x190) | (CurrentGameMode == 0x12C)))) != 0) {
						player.trickTrajectory = 2U;
					}
				}
				break;
			}
			case PlayerState::TurbulenceTrick:
			case PlayerState::TurbulenceTrick2:
				for(i = 0; i < 2; i++) {
					player.secondUnk[i] = player.firstUnk[i];
				}
				player.unk1064 = player.greenCave_subState;
				if((player.specialFlags & 2) != 0) {
					player.trickTrajectory = 3U;
				}
				break;
			case PlayerState::Jump:
				gearSpecialFlags = player.gearptr->specialFlags;

				if(player.characterArchetype == BoostArchetype) {
					player.unkB08 = 0x9040F;
				} else {
					player.unkB08 = 0x9000F;
				}

				player.unkB08 = player.unkB08 & (~0U - (((gearSpecialFlags >> 4U) & 1) * 0x2400));
				if(!player.aiControl) {
					lbl_10087C40 = static_cast<s32>(gu32GameCnt);
					Sound::PlaySound(Sound::ID::IDKSFX, 0x02);
				}
				break;
			case PlayerState::QTE2: {
				gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0x80C00;
				player.unkB08 = player.unkB08 & (~0U - (((gearSpecialFlags >> 4U) & 1) * 0x2400));
				player.unkB2C = zero;
				player.unkB28 = zero;
				player.unkB24 = zero;
				break;
			}
			case PlayerState::RailGrind: {
				gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0x80E00;
				player.unkB08 = player.unkB08 & (~0U - (((gearSpecialFlags >> 4U) & 1) * 0x2400));
				player.unkB2C = zero;
				player.unkB28 = zero;
				player.unkB24 = zero;
				break;
			}
			case PlayerState::Fly:
				gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0xA040F;
				player.unkB08 = player.unkB08 & (~0U - (((gearSpecialFlags >> 4U) & 1) * 0x2400));
				break;
			case PlayerState::AttackedByPlayer: {
				gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0;
				player.unkB08 = player.unkB08 & (~0U - (((gearSpecialFlags >> 4U) & 1) * 0x2400));
				player.flags |= 0x100000;
				player.unkB2C = zero;
				player.unkB28 = zero;
				player.unkB24 = zero;
				break;
			}
			case PlayerState::Stun: {
				gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0;
				player.unkB08 = player.unkB08 & (~0U - (((gearSpecialFlags >> 4U) & 1) * 0x2400));
				player.unkB2C = zero;
				player.unkB28 = zero;
				player.unkB24 = zero;
				break;
			}
			case PlayerState::Unknown:
				gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0;
				player.unkB08 = player.unkB08 & (~0U - (((gearSpecialFlags >> 4U) & 1) * 0x2400));
				break;
			case PlayerState::AttackingPlayer:
				player.flags |= 0x100000;
				gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0;
				player.unkB08 = player.unkB08 & (~0U - (((gearSpecialFlags >> 4U) & 1) * 0x2400));
				break;
			case 26:
			case 27:
			case 28:
			case 29: {
				gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0;
				player.unkB08 = player.unkB08 & (~0U - (((gearSpecialFlags >> 4U) & 1) * 0x2400));
				player.unkB2C = zero;
				player.unkB28 = zero;
				player.unkB24 = zero;
				break;
			}
			case PlayerState::Run:
				player.unkB10 = 1;
				gearSpecialFlags = player.gearptr->specialFlags;
				// player.unkB08 = 0xF; // original line

				// C24CDFE4, gives ability to jump in run
				player.unkB08 = 0x28F70F;

				player.unkB08 = player.unkB08 & (~0U - (((gearSpecialFlags >> 4U) & 1) * 0x2400));
				break;
			case PlayerState::TurbulenceRide:
				if((player.flags & 0x100) != 0) {
					func_ClearPlayerMotionTable(playerPtr);
					func__RegistPlayerMotionTable(playerPtr, 1, 0, 10.0f, 0.0f);
					func__RegistPlayerMotionTable(playerPtr, 0x46, 8, 10.0f, 0.0f);
					func_StartPlayerMotionTable(playerPtr);
				}
				gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0x1C0603;
				player.unkB08 = player.unkB08 & (~0U - (((gearSpecialFlags >> 4U) & 1) * 0x2400));
				func_GetDirectTrickBonus(playerPtr, 1);
				player.flags &= 0xFFFF7F7F;
				player.trickFail |= 8;
				player.unk1033 = 0;
				break;
			case PlayerState::Death:
				if(CurrentGameMode == 0x190U) {
					lbl_000FC114(playerPtr);
				}
				break;
			default:
				break;
		}

		if(((player.unkBB4 >> 8U) & 1 & (((player.flags >> 8U) & 1) ^ 1) & static_cast<u32>(Gears[player.extremeGear].model == 0xF)) != 0) {
			MagicCarpetParticle *mcparticle = &gsParFullParam_MagicCarpet;

			ParticleObject1 *temp_r10 = (reinterpret_cast<ParticleObject *>(SetTask(&func_Particle_Task, gsParFullParam_MagicCarpet.unk6, 2)))->object1;

			temp_r10->unk72 = player.index;
			temp_r10->unk0 = mcparticle->unk74;
			temp_r10->unk4 = mcparticle->unk78;
			temp_r10->unk8 = mcparticle->unk7C;
			temp_r10->unk30 = mcparticle->unk98;
			temp_r10->unk34 = mcparticle->unk9C;
			temp_r10->unk38 = mcparticle->unkA0;
			temp_r10->unk3C = mcparticle->unk8C;
			temp_r10->unk40 = mcparticle->unk90;
			temp_r10->unk44 = mcparticle->unk94;
			temp_r10->unk10 = mcparticle->unk80;
			temp_r10->unk14 = mcparticle->unk84;
			temp_r10->unk18 = mcparticle->unk88;
			temp_r10->unk48 = &gcosNnSystemVecZeroFast;
			temp_r10->unk60 = mcparticle;
			temp_r10->unk68 = static_cast<u32>(gpsTexList_Particle);
			temp_r10->unk6C = &player.unkC4;
			temp_r10->unk73 = -1;
			temp_r10->unk74 = 0;
			player.unkBAC |= 0x4000;
			player.unk1065 = 0x1E;
		}
	}
}

#pragma clang diagnostic pop