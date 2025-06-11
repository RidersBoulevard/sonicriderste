#include "SetPlayerState.hpp"
#include "compare_many.hpp"
#include "lib/sound.hpp"
#include "lib/stdlib.hpp"
#include "main.hpp"
#include "riders/gamemode.hpp"
#include "riders/stage.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-magic-numbers"
ASMUsed void func_SetPlayerActionType(Player *playerPtr, u32 stateInt) {
	constexpr f32 zero = 0.0F;
	auto newState = static_cast<PlayerState>(stateInt);

	Player &player = *playerPtr;
	auto &playerState = player.state;
	auto &previousState = player.previousState;
	if((playerState != PlayerState::Death && player.unk1037 != std::to_underlying(PlayerState::Retire)) || stateInt == std::to_underlying(PlayerState::Cruise)) {
		if(playerState == PlayerState::Unknown2) {
			lbl_00187ED0(playerPtr);
		}
		player.unk1044 = player.qteState;
		player.qteState = previousState;
		previousState = playerState;
		playerState = newState;
		player.unk1037 = 0;
		player.unkAB8 = -0.138889F;
		const auto beingAttacked = (player.lightBoard_flag & 0x200) != 0;
		const bool beingAttackedAndNotCruising = beingAttacked && playerState != PlayerState::Cruise;
		if(if_any(previousState, std::is_eq, PlayerState::AttackedByPlayer, PlayerState::AttackingPlayer) || beingAttackedAndNotCruising || player.movementFlags.hasAny(MovementFlags::boosting)) {
			//Fly, Jump, Fall, Run, QTE, TurbulenceRide, QTE2) || (playerState >= FrontflipRamp && playerState <= ManualRamp)
			if(if_all(playerState, std::is_neq, PlayerState::AttackedByPlayer, PlayerState::AttackingPlayer, PlayerState::Jump)) {
				if (player.characterArchetype == CharacterArchetype::Boost) {
					if(if_any(previousState, std::is_eq, PlayerState::AttackedByPlayer, PlayerState::AttackingPlayer) || beingAttackedAndNotCruising) {lbl_Player_BoostEndFunction(playerPtr);}
					else if(if_all(previousState, std::is_neq, PlayerState::Jump, PlayerState::Fall, PlayerState::FrontflipRamp, PlayerState::BackflipRamp, PlayerState::HalfPipeTrick, PlayerState::ManualRamp, PlayerState::RailGrind, PlayerState::Fly) &&
					   if_all(playerState, std::is_neq, PlayerState::Jump, PlayerState::Fall, PlayerState::FrontflipRamp, PlayerState::BackflipRamp, PlayerState::HalfPipeTrick, PlayerState::ManualRamp, PlayerState::Fly)){
						lbl_Player_BoostEndFunction(playerPtr);
					}
				} else {
					lbl_Player_BoostEndFunction(playerPtr);
				}
			}
		}
		player.unkB10 = 1;
		if(!player.flags.hasAny(static_cast<PlayerFlags>(0x2800))) {
			const PlayerGearStats &gearStats = player.gearStats[player.level];
			auto &speedCap = player.speedCap;
			speedCap = (CurrentStage == DigitalDimension && player.qteState == PlayerState::QTE2) ? 0.9F : gearStats.topSpeed;
			if(player.playerType) {
				speedCap += player.unk7F8;
				speedCap = clamp(speedCap, 0.231481F, 1.38889F);
			}
		}
		if(player.currentAir > 0) {
			player.flags &= ~PlayerFlags::Unknown2;
			player.flags |= PlayerFlags::Unknown;
		}
		switch(playerState) {
			case PlayerState::QTE: {
				const auto &gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0x80F;
				player.unkB2C = zero;
				player.unkB28 = zero;
				player.unkB24 = zero;
				break;
			}
			case PlayerState::Cruise:
				if(beingAttacked) {
					playerState = PlayerState::AttackedByPlayer;
					lbl_0009195C(playerPtr);
				} else {
					const auto &playerSpecialFlags = player.specialFlags;
					constexpr auto otherFlags = 0x2400U;
					player.unkB08 = 0x28F70F & (playerSpecialFlags.hasAny(SpecialFlags::noBoosting) ? ~otherFlags : ~0u); // wtf
					player.flags |= static_cast<PlayerFlags>(4);
					player.flags &= static_cast<PlayerFlags>(0x022A092F);
					player.jumpCharge = 0;
					player.unkD9E = -1;
					player.subState = 0;

					switch(previousState) {
						case PlayerState::Jump ... PlayerState::HalfPipeTrick:
							player.unkD8C = -1;
							break;
						default: break;
					}
					if((previousState <= PlayerState::TurbulenceTrick2) && (previousState >= PlayerState::Fall)) {
						PlayerCameraStruct &temp_r3 = playerCameraStruct[player.index];
						temp_r3.cameraPreset = 0xFF;
						temp_r3.cameraPresetProperty = 0;
					}
				}
				break;
			case PlayerState::Fall: {
				const PlayerGearStats &gearStats = player.gearStats[player.level];
				f32 gearTopSpeed = gearStats.topSpeed;
				if(player.playerType) {
					gearTopSpeed += player.unk7F8;
				}
				gearTopSpeed = clamp(gearTopSpeed, pSpeed(179.9999), pSpeed(300.0002));
				const auto &playerSpeed = player.speed;
				if(player.flags.hasAny(PlayerFlags::TurbulenceLRExit) || previousState == PlayerState::TurbulenceRide) {
					player.gravity = 0.1f * -((0.33f * (playerSpeed / gearTopSpeed)) - 0.8F);
				} else {
					if(playerSpeed <= gearTopSpeed) {
						player.gravity = 0.1f * -((0.33f * (playerSpeed / gearTopSpeed)) - 0.4f);
					} else {
						player.gravity = 0.007f;
					}
				}
				player.gravity *= std::pow(gf32_SystemHzAdd1, 2.0f);
				if(player.characterArchetype == CharacterArchetype::Boost) {
					player.unkB08 = 0x9040F;
				} else {
					player.unkB08 = 0x9000F;
				}
				const auto &gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = player.unkB08 & (~0U - (((static_cast<u32>(static_cast<SpecialFlags>(gearSpecialFlags)) >> 4U) & 1) * 0x2400));
				break;
			}
			case PlayerState::FrontflipRamp:
			case PlayerState::BackflipRamp:
			case PlayerState::UnknownTrick:
			case PlayerState::HalfPipeTrick:
				player.unkB2C = zero;
				player.unkB28 = zero;
				player.unkB24 = zero;
				[[fallthrough]];
			case PlayerState::ManualRamp: {
				const auto &gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0x9040F & (~0U - (((static_cast<u32>(static_cast<SpecialFlags>(gearSpecialFlags)) >> 4U) & 1) * 0x2400));
				player.flags &= ~static_cast<PlayerFlags>(0x100);
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
				if((RuleSettings & 0x40'0000) == 0) {
					auto playerSpecialFlags = player.specialFlags;

					bool legendRampNight = (CurrentStage == NightChase 
					&& (player.currentCollision->boundScale == 20.2919673919678f ||
						player.currentCollision->boundScale == 8.23449897766113f));

					bool legendRampSand = (CurrentStage == SandRuins
					&& player.currentCollision->boundScale == 114.946144104004f);

					// (isSuperCharacter(player, Knuckles) &&
					// 	 player.last_movementFlags.hasAny(boosting)) ||

					if ((isSuperCharacter(player, Character::Knuckles) &&
						 player.last_movementFlags.hasAny(MovementFlags::boosting)) ||
						 (CurrentStage == DarkDesert && playerState == PlayerState::BackflipRamp) ||
						 player.specialReciproExtend || 
						 legendRampNight ||
						 legendRampSand) {
						playerSpecialFlags |= SpecialFlags::legendEffect;
					}
					if(playerSpecialFlags.hasAny(SpecialFlags::legendEffect)) {
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
					const auto &temp_r3_2 = player.unk9FD;
					if(temp_r3_2 != 0) {
						player.trickTrajectory = temp_r3_2 - 1;
					}
				} else {
					if(((static_cast<u32>(2 - player.trickTrajectory) >> 0x1FU) & static_cast<u32>(((static_cast<int>(CurrentStage == 6) & static_cast<int>(player.rampType == 1)) != 0) && ((CurrentGameMode == 0x190) || (CurrentGameMode == 0x12C)))) != 0) {
						player.trickTrajectory = 2U;
					}
				}
				break;
			}
			case PlayerState::TurbulenceTrick:
			case PlayerState::TurbulenceTrick2:
				for(u32 i = 0; i < 2; i++) {
					player.secondUnk[i] = player.firstUnk[i];
				}
				player.unk1064 = player.greenCave_subState;
				if(player.specialFlags.hasAny(static_cast<SpecialFlags>(2))) {
					player.trickTrajectory = 3U;
				}
				break;
			case PlayerState::Jump: {
				const auto &gearSpecialFlags = player.gearptr->specialFlags;

				if(player.characterArchetype == CharacterArchetype::Boost) {
					player.unkB08 = 0x9040F;
				} else {
					player.unkB08 = 0x9000F;
				}

				player.unkB08 = player.unkB08 & (~0U - (((static_cast<u32>(static_cast<SpecialFlags>(gearSpecialFlags)) >> 4U) & 1) * 0x2400));
				if(!player.aiControl) {
					lbl_10087C40 = static_cast<s32>(gu32GameCnt);
					Sound::PlaySound(Sound::ID::IDKSFX, 0x02);
				}
				break;
			}
			case PlayerState::QTE2: {
				const auto &gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0x80C00;
				player.unkB08 = player.unkB08 & (~0U - (((static_cast<u32>(static_cast<SpecialFlags>(gearSpecialFlags)) >> 4U) & 1) * 0x2400));
				player.unkB2C = zero;
				player.unkB28 = zero;
				player.unkB24 = zero;
				break;
			}
			case PlayerState::RailGrind: {
				const auto &gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0x80E00;
				player.unkB08 = player.unkB08 & (~0U - (((static_cast<u32>(static_cast<SpecialFlags>(gearSpecialFlags)) >> 4U) & 1) * 0x2400));
				player.unkB2C = zero;
				player.unkB28 = zero;
				player.unkB24 = zero;
				break;
			}
			case PlayerState::Fly: {
				const auto &gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0xA040F;
				player.unkB08 = player.unkB08 & (~0U - (((static_cast<u32>(static_cast<SpecialFlags>(gearSpecialFlags)) >> 4U) & 1) * 0x2400));
				break;
			}
			case PlayerState::AttackedByPlayer: {
				const auto &gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0;
				player.unkB08 = player.unkB08 & (~0U - (((static_cast<u32>(static_cast<SpecialFlags>(gearSpecialFlags)) >> 4U) & 1) * 0x2400));
				player.flags |= static_cast<PlayerFlags>(0x100000);
				player.unkB2C = zero;
				player.unkB28 = zero;
				player.unkB24 = zero;
				break;
			}
			case PlayerState::Stun: {
				const auto &gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0;
				player.unkB08 = player.unkB08 & (~0U - (((static_cast<u32>(static_cast<SpecialFlags>(gearSpecialFlags)) >> 4U) & 1) * 0x2400));
				player.unkB2C = zero;
				player.unkB28 = zero;
				player.unkB24 = zero;
				break;
			}
			case PlayerState::Unknown: {
				const auto &gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0;
				player.unkB08 = player.unkB08 & (~0U - (((static_cast<u32>(static_cast<SpecialFlags>(gearSpecialFlags)) >> 4U) & 1) * 0x2400));
				break;
			}
			case PlayerState::AttackingPlayer: {
				player.flags |= static_cast<PlayerFlags>(0x100000);
				const auto &gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0;
				player.unkB08 = player.unkB08 & (~0U - (((static_cast<u32>(static_cast<SpecialFlags>(gearSpecialFlags)) >> 4U) & 1) * 0x2400));
				break;
			}
			case PlayerState::Run: {
				player.unkB10 = 1;
				const auto &gearSpecialFlags = player.gearptr->specialFlags;
				// player.unkB08 = 0xF; // original line

				// C24CDFE4, gives ability to jump in run
				player.unkB08 = 0x28B70F;
                if (player.extremeGear == ExtremeGear::AdvantageS && player.currentAir > 0) {
                    player.unkB08 |= std::to_underlying(MovementFlags::drifting);
                }

				player.unkB08 = player.unkB08 & (~0U - (((static_cast<u32>(static_cast<SpecialFlags>(gearSpecialFlags)) >> 4U) & 1) * 0x2400));
				break;
			}
			case PlayerState{26}:
			case PlayerState::Unknown2:
			case PlayerState{28}:
			case PlayerState{29}: {
				const auto &gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0;
				player.unkB08 = player.unkB08 & (~0U - (((static_cast<u32>(static_cast<SpecialFlags>(gearSpecialFlags)) >> 4U) & 1) * 0x2400));
				player.unkB2C = zero;
				player.unkB28 = zero;
				player.unkB24 = zero;
				break;
			}
			case PlayerState::TurbulenceRide: {
				if(player.flags.hasAny(static_cast<PlayerFlags>(0x100))) {
					func_ClearPlayerMotionTable(playerPtr);
					func__RegistPlayerMotionTable(playerPtr, 1, 0, 10.0f, 0.0f);
					func__RegistPlayerMotionTable(playerPtr, 0x46, 8, 10.0f, 0.0f);
					func_StartPlayerMotionTable(playerPtr);
				}
				const auto &gearSpecialFlags = player.gearptr->specialFlags;
				player.unkB08 = 0x1C0603;
				player.unkB08 = player.unkB08 & (~0U - (((static_cast<u32>(static_cast<SpecialFlags>(gearSpecialFlags)) >> 4U) & 1) * 0x2400));
				func_GetDirectTrickBonus(playerPtr, 1);
				player.flags &= static_cast<PlayerFlags>(0xFFFF7F7F);
				player.trickFail |= 8;
				player.unk1033 = 0;
				break;
			}
			case PlayerState::Death:
				if(CurrentGameMode == 0x190U) {
					lbl_000FC114(playerPtr);
				}
				break;
			default:
				break;
		}

		if(((player.unkBB4 >> 8U) & 1 & (((static_cast<u32>(static_cast<PlayerFlags>(player.flags)) >> 8U) & 1) ^ 1) & static_cast<u32>(Gears[player.extremeGear].model == 0xF)) != 0) {
			auto *mcparticle = &gsParFullParam_MagicCarpet;

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