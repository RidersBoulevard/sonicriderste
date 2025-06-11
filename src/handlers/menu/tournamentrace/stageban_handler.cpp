#include "stageban_handler.hpp"
#include "containers/graphicalobject.hpp"
#include "lib/sound.hpp"
#include "lib/stdlib.hpp"
#include "riders/player.hpp"

BSS_StageBan bss_StageBans;
u32 bss_StageBans_isBanned;

constexpr std::array<NeutralStages, NeutralStageAmount> AllowedNeutralStages = {{
		{0, 0},// metal city
		{1, 1},// red canyon
		{2, 1},// ice factory
		{6, 0},// digital dimension
		{7, 1},// sega illusion
}};

ASMDefined void* const gp2DSys;
ASMDefined void PADReset(u32);
ASMDefined void *lbl_8021BB40;
ASMDefined void *lbl_8021BB44;

ASMUsed void ClearStageBanBSS() {
	//memset(&bss_StageBans, 0, sizeof(bss_StageBans));
	bss_StageBans = {};
}

ASMUsed u8 CheckStageBan(StageSelectObject2 *stageSelectObject) {
	BSS_StageBan *bss = &bss_StageBans;
	const u32 playerIndex = bss->playerControl == bss->player_port[0] ? 0 : 1;

	bool canPickHeroes = true;
	bool canPickBabylon = true;
	u8 stageToBePicked = 0xFF; // if both stages are banned, it'll stay 0xFF
	if (bss->bannedStages[stageSelectObject->selectedStageSquare].babylonStage) {
		if (bss->bannedStages[stageSelectObject->selectedStageSquare].heroesStage_playerIndex != playerIndex) {
			stageToBePicked = 0;
			canPickBabylon = false;
		} else {
			stageToBePicked = 1;
		}
	} else {
		stageToBePicked = 1;
	}

	if (bss->bannedStages[stageSelectObject->selectedStageSquare].heroesStage) {
		if (bss->bannedStages[stageSelectObject->selectedStageSquare].heroesStage_playerIndex != playerIndex) {
			stageToBePicked = 1;
			canPickHeroes = false;
		} else {
			stageToBePicked = 0;
		}
	} else {
		stageToBePicked = 0;
	}

	return (canPickHeroes || canPickBabylon) ? stageToBePicked : 0xFF;
}

void StageBanGraphicHandler(/*GraphicalObject *subMenu,*/
							StageSelectObject2 *stageSelectObject,
							HeroesStageButton_GraphicalData *heroes_button,
							BabylonStageButton_GraphicalData *babylon_button) {
	BSS_StageBan *bss = &bss_StageBans;
	const BannedStages bannedStages = bss->bannedStages[stageSelectObject->selectedStageSquare];

	if (stageSelectObject->subMenuStatus != 0) {
		if (bannedStages.heroesStage == 1) {
			heroes_button->inactiveButtonRGBA_1 = BannedStageGraphicColors[bannedStages.heroesStage_playerIndex];
			heroes_button->inactiveButtonRGBA_2 = BannedStageGraphicColors[bannedStages.heroesStage_playerIndex];
		} else {
			heroes_button->inactiveButtonRGBA_1 = 0x323232FF;
			heroes_button->inactiveButtonRGBA_2 = 0x323232FF;
		}
		if (bannedStages.babylonStage == 1) {
			babylon_button->inactiveButtonRGBA_1 = BannedStageGraphicColors[bannedStages.babylonStage_playerIndex];
			babylon_button->inactiveButtonRGBA_2 = BannedStageGraphicColors[bannedStages.babylonStage_playerIndex];
		} else {
			babylon_button->inactiveButtonRGBA_1 = 0x323232FF;
			babylon_button->inactiveButtonRGBA_2 = 0x323232FF;
		}
	}
}


void TournamentRace_SetLapCountRules(s32 lapCount) {
    s32 newSettings = RuleSettings & ~0x63; // get rid of the lap count
    newSettings |= lapCount;
    RuleSettings = newSettings;
    GameData.ruleSettings = static_cast<u32>(newSettings);
}

ASMUsed void Player_WinTournamentRace(const Player *player) {
	// triggers when last lap finish line is crossed
	BSS_StageBan *bss = &bss_StageBans;

	if (bss->tournamentRace == 0u) { return; }
    if (bss->isTestLap) {
        bss->isTestLap = false;
        bss->hasTestLapOccurred = true;
        return;
    }

	auto index = player->index & 1u;
	bss->playerControl = bss->player_port[index];
	bss->player_banCount[index] = 4;
	index ^= 1; // update other player
	bss->player_banCount[index] = 0;
	bss->roundState = 1;

	if (bss->currentRound < 255) {
		bss->currentRound += 1;
	}

	// reset banned stages
	bss->bannedStages = {};
	//memset(&bss->bannedStages, 0, sizeof(bss->bannedStages));

	if (player->index == 0) {
		// player 1 won
		if (bss->player1Score < 15) {// max size of bitfield
			bss->player1Score += 1;
		}
	} else {
		// player 2 won
		if (bss->player2Score < 15) {// max size of bitfield
			bss->player2Score += 1;
		}
	}
}

ASMUsed void Player_TournamentRaceEarlyExit(PauseScreenObject1 *pauseInfo) {
	auto &bss = bss_StageBans;
	if (bss.tournamentRace == 0u) { return; }
    if (bss.isTestLap) {
        bss.isTestLap = false;
        bss.hasTestLapOccurred = true;
        return;
    }

	// see if both players are at the start line
	auto atStartLineCount = 0u;
	for (const auto &player : getCurrentPlayerList()) {
		if(player.currentLap == 0){
			atStartLineCount++;
		}
		// if (player->placement == 0) firstPlayer = player;
	}

	if (atStartLineCount != 2 && InGamePlayerCount == 2) {
		// if both players aren't at start line, detect who paused, and make them lose the point
		const auto &pausedPlayer = players[pauseInfo->pausedPlayerIndex];
		if ((pausedPlayer.currentLap <= (RuleSettings & 0x7F)) || (pausedPlayer.placement != 0)) {
			// if paused player has crossed finish line and is the winner, the winner has been already determined
			// this code will only run if it's not a finish line quit
			const u32 wonPlayerIndex = pauseInfo->pausedPlayerIndex ^ 1; // reverse player index to determine the winner
			const auto &wonPlayer = players[wonPlayerIndex];
			Player_WinTournamentRace(&wonPlayer);
		}
	}
}

inline void SwitchSubMenuStage(StageSelectObject2 *stageSelectObject) {
	if (stageSelectObject->subMenuStatus == 0) { return; }
	Sound::PlaySound(Sound::ID::VSFX, 0x00);
	stageSelectObject->selectedSubMenuStage ^= 1;
	stageSelectObject->subMenuDelayFrames = 20;
}

bool CheckForNeutralStageBan(StageSelectObject2 *stageSelectObject) {
	for (auto AllowedNeutralStage : AllowedNeutralStages) {
		if (AllowedNeutralStage.selectedStageSquare == stageSelectObject->selectedStageSquare &&
			AllowedNeutralStage.selectedSubMenuStage == stageSelectObject->selectedSubMenuStage) {
			return true;
		}
	}
	return false;
}

ASMUsed void StageBanHandler(GraphicalObject *subMenu) {
	void *objectptr = static_cast<u32 *>(gp2DSys) + 0x6030 / 4;
	void **object = static_cast<void **>(objectptr);
	auto *stageSelectObject = static_cast<StageSelectObject2 *>(*object);
	void *graphicalData = subMenu->graphicalData;
	void *subMenuGraphicalData = static_cast<u32 *>(graphicalData) + (0x18 / 4) + subMenu->idStruct.graphicalDataID;
	void **temp = static_cast<void **>(subMenuGraphicalData);
	void *heroes_graphical = static_cast<u32 *>(*temp) + (0x8 / 4) + 3;
	void *babylon_graphical = static_cast<u32 *>(*temp) + (0x8 / 4) + 4;
	void **temp2 = static_cast<void **>(heroes_graphical);
	void **temp3 = static_cast<void **>(babylon_graphical);
	auto *heroes_button = static_cast<HeroesStageButton_GraphicalData *>(*temp2);
	auto *babylon_button = static_cast<BabylonStageButton_GraphicalData *>(*temp3);

	BSS_StageBan &bss = bss_StageBans;
	if (bss.tournamentRace) {
		switch (bss.roundState) {
			case 0:{
				u32 portCount = 0;
				for(u32 i = 0; i < 4; i++){
					const Controller &loopInput = GameControllers[i];
					if (loopInput.initStatus2 != ~0u) {
						portCount += 1;
						if (portCount == 1) {
							bss.player_port[0] = loopInput.port;
						} else {
							bss.player_port[1] = loopInput.port;
						}
					}
					if(portCount > 2){
						break;
					}
				}

				if (bss.currentRound == 1) {
					bss.player_banCount[0] = 1;
					bss.player_banCount[1] = 2;
				}

				bss.roundState += 1;
				break;
			}
			case 1:
				if (bss.currentRound == 1) {
					bss.playerControl = bss.player_port[0];
					if (bss.player_banCount[0] == 0) {
						bss.roundState += 1;
						SwitchSubMenuStage(stageSelectObject);
					}
				} else {
					if (bss.playerControl == bss.player_port[0]) {
						u8 banCount = bss.player_banCount[0];
						if (banCount == 0) {
							bss.playerControl = bss.player_port[1];
							bss.roundState += 1;
							SwitchSubMenuStage(stageSelectObject);
						}
					} else {
						u8 banCount = bss.player_banCount[1];
						if (banCount == 0) {
							bss.playerControl = bss.player_port[0];
							bss.roundState += 1;
							SwitchSubMenuStage(stageSelectObject);
						}
					}
				}
				break;
			case 2:
				if (bss.currentRound == 1) {
					bss.playerControl = bss.player_port[1];
					if (bss.player_banCount[1] == 0) {
						bss.roundState += 1;
						SwitchSubMenuStage(stageSelectObject);
					}
				}
				break;
			case 3:
				if (bss.currentRound == 1) {
					bss.playerControl = bss.player_port[0];
				}
				break;
			default:
				break;
		}

		for (u32 i = 0; i < 4; i++) {
			if (i != bss.playerControl) {
				auto *subInputBase = static_cast<Controller_ *>(lbl_8021BB40);
				auto *subInputBase2 = static_cast<Controller_ *>(lbl_8021BB44);
				Controller_ *subInput = subInputBase + i;
				Controller_ *subInput2 = subInputBase2 + i;
				Controller &otherInput = GameControllers[i];
				subInput->status = 0xFF;
				subInput2->status = 0xFF;
				PADReset(0x80000000UL >> i);
				otherInput.initStatus = false;
				otherInput.initStatus2 = ~0U;

				otherInput.leftStickHorizontal = 0;
				otherInput.leftStickVertical = 0;
				otherInput.filler2[0] = std::byte{0};
				otherInput.filler2[1] = std::byte{0};
			}
		}

		const Controller &playerInput = GameControllers[bss.playerControl];
		bool isBanned = FALSE;
		if (playerInput.toggleFaceButtons.hasAny(Buttons::X)) {
			u8 banCount;
			u8 banPlayerIndex;
			if (bss.playerControl == bss.player_port[0]) {
				banCount = bss.player_banCount[0];
				banPlayerIndex = 0;
			} else {
				banCount = bss.player_banCount[1];
				banPlayerIndex = 1;
			}

			bool canBan = true;
			if (stageSelectObject->selectedSubMenuStage == 0) {
				// heroes stage
				if (banCount != 0 && bss.bannedStages[stageSelectObject->selectedStageSquare].heroesStage != 1) {
					if (bss.currentRound == 1) {
						canBan = CheckForNeutralStageBan(stageSelectObject);
					}

					if (canBan) {
						bss.bannedStages[stageSelectObject->selectedStageSquare].heroesStage = true;
						bss.bannedStages[stageSelectObject->selectedStageSquare].heroesStage_playerIndex = banPlayerIndex;
						isBanned = TRUE;
					}
				}
			} else {
				// babylon stage
				if (banCount != 0 && bss.bannedStages[stageSelectObject->selectedStageSquare].babylonStage != 1) {
					if (bss.currentRound == 1) {
						canBan = CheckForNeutralStageBan(stageSelectObject);
					}

					if (canBan) {
						bss.bannedStages[stageSelectObject->selectedStageSquare].babylonStage = true;
						bss.bannedStages[stageSelectObject->selectedStageSquare].babylonStage_playerIndex = banPlayerIndex;
						isBanned = TRUE;
					}
				}
			}

			if (banCount != 0 && isBanned) {
				Sound::PlaySound(Sound::ID::MSFX, 0);
				if (bss.playerControl == bss.player_port[0]) {
					bss.player_banCount[0] = banCount - 1;
				} else {
					bss.player_banCount[1] = banCount - 1;
				}

				if (stageSelectObject->subMenuStatus != 0) {
					if (bss.bannedStages[stageSelectObject->selectedStageSquare].heroesStage) {
						if (bss.bannedStages[stageSelectObject->selectedStageSquare].heroesStage_playerIndex !=
							banPlayerIndex) {
							if (bss.bannedStages[stageSelectObject->selectedStageSquare].babylonStage) {
								if (bss.bannedStages[stageSelectObject->selectedStageSquare].babylonStage_playerIndex !=
									banPlayerIndex) {
									// force player out of sub menu
									Sound::PlaySound(Sound::ID::VSFX, 0x02);
									stageSelectObject->subMenuControl = 0;
									stageSelectObject->subMenuStatus = 3;
								}
							}
							Sound::PlaySound(Sound::ID::VSFX, 0x00);
							stageSelectObject->selectedSubMenuStage ^= 1;
							stageSelectObject->subMenuDelayFrames = 20;
						}
					} else if (bss.bannedStages[stageSelectObject->selectedStageSquare].babylonStage) {
						if (bss.bannedStages[stageSelectObject->selectedStageSquare].babylonStage_playerIndex !=
							banPlayerIndex) {
							if (bss.bannedStages[stageSelectObject->selectedStageSquare].heroesStage) {
								if (bss.bannedStages[stageSelectObject->selectedStageSquare].heroesStage_playerIndex !=
									banPlayerIndex) {
									// force player out of sub menu
									Sound::PlaySound(Sound::ID::VSFX, 0x02);
									stageSelectObject->subMenuControl = 0;
									stageSelectObject->subMenuStatus = 3;
								}
							}
							Sound::PlaySound(Sound::ID::VSFX, 0x00);
							stageSelectObject->selectedSubMenuStage ^= 1;
							stageSelectObject->subMenuDelayFrames = 20;
						}
					}
				}
			}
		} else if (playerInput.toggleFaceButtons.hasAny(Buttons::Y)) {
			// unban a stage
			u8 banCount;
			u8 banPlayerIndex;
			if (bss.playerControl == bss.player_port[0]) {
				banCount = bss.player_banCount[0];
				banPlayerIndex = 0;
			} else {
				banCount = bss.player_banCount[1];
				banPlayerIndex = 1;
			}

			if (stageSelectObject->selectedSubMenuStage == 0) {
				// heroes stage
				if (bss.bannedStages[stageSelectObject->selectedStageSquare].heroesStage == 1) {
					bss.bannedStages[stageSelectObject->selectedStageSquare].heroesStage = 0;

					Sound::PlaySound(Sound::ID::MSFX, 1);
					if (bss.playerControl == bss.player_port[0]) {
						bss.player_banCount[0] = banCount + 1;
					} else {
						bss.player_banCount[1] = banCount + 1;
					}

					isBanned = TRUE; // setting this to true so that HUD updates
				}
			} else {
				// babylon stage
				if (bss.bannedStages[stageSelectObject->selectedStageSquare].babylonStage == 1) {
					bss.bannedStages[stageSelectObject->selectedStageSquare].babylonStage = 0;

					Sound::PlaySound(Sound::ID::MSFX, 1);
					if (bss.playerControl == bss.player_port[0]) {
						bss.player_banCount[0] = banCount + 1;
					} else {
						bss.player_banCount[1] = banCount + 1;
					}

					isBanned = TRUE; // setting this to true so that HUD updates
				}
			}
		}
		bss_StageBans_isBanned = isBanned;
		StageBanGraphicHandler(/*subMenu,*/ stageSelectObject, heroes_button, babylon_button);
	} else {
		heroes_button->inactiveButtonRGBA_1 = 0x323232FF;
		heroes_button->inactiveButtonRGBA_2 = 0x323232FF;
		babylon_button->inactiveButtonRGBA_1 = 0x323232FF;
		babylon_button->inactiveButtonRGBA_2 = 0x323232FF;
	}
}

ASMUsed u32 TournamentRace_CheckSubMenu(StageSelectObject2 *object, u32 stageToBePicked) {
	BSS_StageBan &bss = bss_StageBans;
	u32 playerIndex;

	if (!bss.tournamentRace) return stageToBePicked;

	if (bss.playerControl == bss.player_port[0]) {
		playerIndex = 0;
	} else {
		playerIndex = 1;
	}

	if (stageToBePicked == 0) {
		// heroes stage
		if (bss.bannedStages[object->selectedStageSquare].heroesStage) {
			if (bss.bannedStages[object->selectedStageSquare].heroesStage_playerIndex != playerIndex) {
				stageToBePicked ^= 1;
			}
		}
	} else {
		// babylon stage
		if (bss.bannedStages[object->selectedStageSquare].babylonStage) {
			if (bss.bannedStages[object->selectedStageSquare].babylonStage_playerIndex != playerIndex) {
				stageToBePicked ^= 1;
			}
		}
	}

	return stageToBePicked;
}

ASMUsed void TournamentRace_PlayFinalStagePickedSound() {
	if (bss_StageBans.tournamentRace == 0u) { return; }
	Sound::PlaySound(Sound::ID::MSFX, 0x2);
}

ASMUsed bool TournamentRace_CheckPickedStage(StageSelectObject2 *object) {
	BSS_StageBan &bss = bss_StageBans;
	if (bss.tournamentRace == 0u) { return true; }

	if (object->selectedSubMenuStage == 0) {
		// heroes stage
		return !bss.bannedStages[object->selectedStageSquare].heroesStage;
	}
	// babylon stage
	return !bss.bannedStages[object->selectedStageSquare].babylonStage;
}

constexpr std::array<u16, 38> TournamentRacePopupText = {
        0x5045, 0x4B25, 0x4B28, 0x4501, 0x4B21, 0x8002, // "Would"
        0x4A16, 0x4B25, 0x4B28, 0x8002, // "you"
        0x4501, 0x4500, 0x4A12, 0x4A1B, 0x8002, // "like"
        0x4708, 0x4B25, 0x8002, // "to"
        0x4B26, 0x4A1B, 0x4707, 0x4603, 0x4B25, 0x4707, 0x5043, 0x8002, // "perform"
        0x4A19, 0x8002, // "a"
        0x4708, 0x4A1B, 0x4A13, 0x4708, 0x8002, // "test"
        0x4501, 0x4A19, 0x4B26, 0x4C33, 0x8000 // "lap?"
};

ASMDefined u32 DisableCourseScrollFlag = 0;

void TournamentRace_Task() {
	ObjectNode * object = gpsCurrentTask;
    auto* object1 = static_cast<TournamentRaceObject1 *>(object->object);
    switch (object->state) {
        case 2:
            DisableCourseScrollFlag = 1;
            object1->text = TournamentRacePopupText.data();
            SetGenericPopup2dObjectTbl(object1);
            object->state += 1;
            break;
        case 3:
            if (object1->menuState >= 2) {
                object1->buttonsPresetID = 0x41; // "Yes" and "No" options
                object1->totalButtonCount = 2;
                object1->selectedButton = 0;
                object->state += 1;
            }
            break;
        case 4:
            if (gsActivePad.toggleButtons.hasAny(Buttons::A) && object1->menuState == 2) {
                object1->menuState = 3;
                if (object1->selectedButton == 0) {
                    TournamentRace_SetLapCountRules(1);
                    bss_StageBans.isTestLap = true;
                } else {
                    bss_StageBans.isTestLap = false;
                }

                PlayAudioFromDAT(Sound::VSFX::MenuConfirm);
            } else if (gsActivePad.toggleButtons.hasAny(Buttons::DPadUp, Buttons::DPadDown, Buttons::LStickUp, Buttons::LStickDown)) {
                object1->selectedButton ^= 1;
                PlayAudioFromDAT(Sound::VSFX::MenuScroll);
            }

            if (object1->menuState >= 4) {
                DisableCourseScrollFlag = 0;
                Sys2d_ClrObjectGROUP_IN(0xF);
                gNp_DeadTask();
            }
            break;
        default:
            break;
    }
}

ASMUsed void TournamentRace_TaskSet() {
    if (bss_StageBans.tournamentRace != 0 && bss_StageBans.currentRound == 1 && !bss_StageBans.hasTestLapOccurred) {
		SetTask(&TournamentRace_Task, ObjectGroups::Object_TournamentRacePopup, 1);
	}
}

ASMUsed void TournamentRace_ResetLapCount() {
    if (bss_StageBans.hasTestLapOccurred && bss_StageBans.currentRound == 1) {
        TournamentRace_SetLapCountRules(3);
    }
}