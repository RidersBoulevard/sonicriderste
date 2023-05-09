#include "context.hpp"
#include "lib/sound.hpp"
#include "stageban_handler.hpp"
#include "containers/graphicalobject.hpp"

BSS_StageBan bss_StageBans;
u32 bss_StageBans_isBanned;

constexpr NeutralStages AllowedNeutralStages[NeutralStageAmount] = {
		{0, 0}, // metal city
		{1, 1}, // red canyon
		{2, 1}, // ice factory
		{6, 0}, // digital dimension
		{7, 1}, // sega illusion
};

global void *gp2DSys;
global void PADReset(u32);
global void *lbl_8021BB40;
global void *lbl_8021BB44;

ASMUsed void ClearStageBanBSS() {
	TRK_memset(&bss_StageBans, 0, sizeof(bss_StageBans));
}

ASMUsed u8 CheckStageBan(StageSelectObject2 *stageSelectObject) {
	u32 playerIndex;
	BSS_StageBan *bss = &bss_StageBans;
	if (bss->playerControl == bss->player1Port) {
		playerIndex = 0;
	} else {
		playerIndex = 1;
	}

	bool canPickHeroes = true, canPickBabylon = true;
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

	return (canPickHeroes | canPickBabylon) ? stageToBePicked : 0xFF;
}

void StageBanGraphicHandler(/*GraphicalObject *subMenu,*/ StageSelectObject2 *stageSelectObject,
							HeroesStageButton_GraphicalData *heroes_button,
							BabylonStageButton_GraphicalData *babylon_button) {
	BSS_StageBan *bss = &bss_StageBans;
	BannedStages bannedStages = bss->bannedStages[stageSelectObject->selectedStageSquare];

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

ASMUsed void Player_WinTournamentRace(Player *player) {
	// triggers when last lap finish line is crossed
	BSS_StageBan *bss = &bss_StageBans;

	if (!bss->tournamentRace) return;

	u32 index = player->index;
	bss->playerControl = *(&bss->player1Port + index);
	*(&bss->player1_banCount + index) = 4;
	index ^= 1; // update other player
	*(&bss->player1_banCount + index) = 0;
	bss->roundState = 1;

	if (bss->currentRound < 255)
		bss->currentRound += 1;

	// reset banned stages
	TRK_memset(&bss->bannedStages, 0, sizeof(bss->bannedStages));

	if (player->index == 0) {
		// player 1 won
		if (bss->player1Score < 15) // max size of bitfield
			bss->player1Score += 1;
	} else {
		// player 2 won
		if (bss->player2Score < 15) // max size of bitfield
			bss->player2Score += 1;
	}
}

ASMUsed void Player_TournamentRaceEarlyExit(PauseScreenObject1 *pauseInfo) {
	BSS_StageBan *bss = &bss_StageBans;
	Player *player = players.data(); // todo: indexes...
	Player *pausedPlayer = players.data();
	Player *wonPlayer = players.data();
	u32 i, wonPlayerIndex;
	u32 atStartLineCount = 0;

	if (!bss->tournamentRace) return;

	// see if both players are at the start line
	for (i = 0; i < InGamePlayerCount; i++, player++) {
		atStartLineCount += (player->currentLap == 0) ? TRUE : FALSE;
		// if (player->placement == 0) firstPlayer = player;
	}

	if (atStartLineCount != 2 && InGamePlayerCount == 2) {
		// if both players aren't at start line, detect who paused, and make them lose the point
		pausedPlayer += pauseInfo->pausedPlayerIndex;
		if (!((pausedPlayer->currentLap > (RuleSettings & 0x7F)) && (pausedPlayer->placement == 0))) {
			// if paused player has crossed finish line and is the winner, the winner has been already determined
			// this code will only run if it's not a finish line quit
			wonPlayerIndex = pauseInfo->pausedPlayerIndex ^ 1; // reverse player index to determine the winner
			wonPlayer += wonPlayerIndex;
			Player_WinTournamentRace(wonPlayer);
		}
	}
}

inline void SwitchSubMenuStage(StageSelectObject2 *stageSelectObject) {
	if (stageSelectObject->subMenuStatus == 0) return;
	Sound::PlaySound(Sound::ID::VSFX, 0x00);
	stageSelectObject->selectedSubMenuStage ^= 1;
	stageSelectObject->subMenuDelayFrames = 20;
}

bool CheckForNeutralStageBan(StageSelectObject2 *stageSelectObject) {
	u32 i;
	for (i = 0; i < NeutralStageAmount; i++) {
		if (AllowedNeutralStages[i].selectedStageSquare == stageSelectObject->selectedStageSquare &&
			AllowedNeutralStages[i].selectedSubMenuStage == stageSelectObject->selectedSubMenuStage) {
			return true;
		}
	}
	return false;
}

ASMUsed void StageBanHandler(GraphicalObject *subMenu) {
	u8 banCount, banPlayerIndex;
	BOOL isBanned = FALSE;
	BSS_StageBan &bss = bss_StageBans;
	void *objectptr = (u32 *) gp2DSys + 0x6030 / 4;
	void **object = (void **) objectptr;
	auto *stageSelectObject = (StageSelectObject2 *) *object;
	void *graphicalData = subMenu->graphicalData;
	void *subMenuGraphicalData = (u32 *) graphicalData + (0x18 / 4) + subMenu->idStruct.graphicalDataID;
	void **temp = (void **) subMenuGraphicalData;
	void *heroes_graphical = (u32 *) *temp + (0x8 / 4) + 3;
	void *babylon_graphical = (u32 *) *temp + (0x8 / 4) + 4;
	void **temp2 = (void **) heroes_graphical;
	void **temp3 = (void **) babylon_graphical;
	auto *heroes_button = (HeroesStageButton_GraphicalData *) *temp2;
	auto *babylon_button = (BabylonStageButton_GraphicalData *) *temp3;

	if (bss.tournamentRace) {
		switch (bss.roundState) {
			case 0:{
				u32 portCount = 0;
				u32 i = 0;
				do {
					const Controller &loopInput = GameControllers[i];
					if (loopInput.initStatus2 != u32(~0)) {
						portCount += 1;
						if (portCount == 1) {
							bss.player1Port = loopInput.port;
						} else {
							bss.player2Port = loopInput.port;
						}
					}
					i++;
				} while (portCount < 2 && i < 4);

				if (bss.currentRound == 1) {
					bss.player1_banCount = 1;
					bss.player2_banCount = 2;
				}

				bss.roundState += 1;
				break;
			}
			case 1:
				if (bss.currentRound == 1) {
					bss.playerControl = bss.player1Port;
					if (bss.player1_banCount == 0) {
						bss.roundState += 1;
						SwitchSubMenuStage(stageSelectObject);
					}
				} else {
					if (bss.playerControl == bss.player1Port) {
						banCount = bss.player1_banCount;
						if (banCount == 0) {
							bss.playerControl = bss.player2Port;
							bss.roundState += 1;
							SwitchSubMenuStage(stageSelectObject);
						}
					} else {
						banCount = bss.player2_banCount;
						if (banCount == 0) {
							bss.playerControl = bss.player1Port;
							bss.roundState += 1;
							SwitchSubMenuStage(stageSelectObject);
						}
					}
				}
				break;
			case 2:
				if (bss.currentRound == 1) {
					bss.playerControl = bss.player2Port;
					if (bss.player2_banCount == 0) {
						bss.roundState += 1;
						SwitchSubMenuStage(stageSelectObject);
					}
				}
				break;
			case 3:
				if (bss.currentRound == 1) {
					bss.playerControl = bss.player1Port;
				}
				break;
			default:
				break;
		}

		for (u32 i = 0; i < 4; i++) {
			if (i != bss.playerControl) {
				auto *subInputBase = (Controller_ *) lbl_8021BB40;
				auto *subInputBase2 = (Controller_ *) lbl_8021BB44;
				Controller_ *subInput, *subInput2;
				subInput = subInputBase + i;
				subInput2 = subInputBase2 + i;
				Controller &otherInput = GameControllers[i];
				subInput->status = 0xFF;
				subInput2->status = 0xFF;
				PADReset(0x80000000UL >> i);
				otherInput.initStatus = false;
				otherInput.initStatus2 = ~0U;

				otherInput.leftStickHorizontal = 0;
				otherInput.leftStickVertical = 0;
				otherInput.filler2[0] = 0;
				otherInput.filler2[1] = 0;
			}
		}

		const Controller &playerInput = GameControllers[bss.playerControl];
		if (playerInput.toggleFaceButtons & XButton) {
			if (bss.playerControl == bss.player1Port) {
				banCount = bss.player1_banCount;
				banPlayerIndex = 0;
			} else {
				banCount = bss.player2_banCount;
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
				if (bss.playerControl == bss.player1Port) {
					bss.player1_banCount = banCount - 1;
				} else {
					bss.player2_banCount = banCount - 1;
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
		} else if (playerInput.toggleFaceButtons & YButton) {
			// unban a stage
			if (bss.playerControl == bss.player1Port) {
				banCount = bss.player1_banCount;
				banPlayerIndex = 0;
			} else {
				banCount = bss.player2_banCount;
				banPlayerIndex = 1;
			}

			if (stageSelectObject->selectedSubMenuStage == 0) {
				// heroes stage
				if (bss.bannedStages[stageSelectObject->selectedStageSquare].heroesStage == 1) {
					bss.bannedStages[stageSelectObject->selectedStageSquare].heroesStage = 0;

					Sound::PlaySound(Sound::ID::MSFX, 1);
					if (bss.playerControl == bss.player1Port) {
						bss.player1_banCount = banCount + 1;
					} else {
						bss.player2_banCount = banCount + 1;
					}

					isBanned = TRUE; // setting this to true so that HUD updates
				}
			} else {
				// babylon stage
				if (bss.bannedStages[stageSelectObject->selectedStageSquare].babylonStage == 1) {
					bss.bannedStages[stageSelectObject->selectedStageSquare].babylonStage = 0;

					Sound::PlaySound(Sound::ID::MSFX, 1);
					if (bss.playerControl == bss.player1Port) {
						bss.player1_banCount = banCount + 1;
					} else {
						bss.player2_banCount = banCount + 1;
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

	if (bss.playerControl == bss.player1Port) {
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
	if (!bss_StageBans.tournamentRace) return;
	Sound::PlaySound(Sound::ID::MSFX, 0x2);
}

ASMUsed bool TournamentRace_CheckPickedStage(StageSelectObject2 *object) {
	BSS_StageBan &bss = bss_StageBans;
	if (!bss.tournamentRace) return true;

	if (object->selectedSubMenuStage == 0) {
		// heroes stage
		if (bss.bannedStages[object->selectedStageSquare].heroesStage) return false;
		else return true;
	} else {
		// babylon stage
		if (bss.bannedStages[object->selectedStageSquare].babylonStage) return false;
		else return true;
	}
}