#include "custom_music.hpp"
#include <format>
#include "cosmetics/player/exloads.hpp"
#include "lib/stdlib.hpp"
#include "riders/gamemode.hpp"
#include "riders/stage.hpp"
#include "handlers/menu/debugmenu/debugmenu.hpp"

struct ActivePad {
	Flag<Buttons> holdButtons;
	Flag<Buttons> toggleButtons;
};
extern ActivePad gsActivePad;

SubFont CustomMusicTextData;

bool CustomMusic_SkipSHOHOMusic;

USED void CustomMusicRandomizer(const u32 mode) {
	u32 songID              = 0;
	const Player *SHOPlayer = nullptr;
	bool HOCheck            = false;
	CustomMusic_SkipSHOHOMusic = false;
	const bool isVanillaPlaylist = DebugMenu_IsVanillaPlaylist();

	switch(mode) {
		case 0: // stage music mode
			if (isVanillaPlaylist) {
				songID = 4; // vanilla
			} else {
				songID = lbl_RNG_Number(4);
			for(auto &input: GameControllers) {
				if(input.holdFaceButtons.hasAny(Buttons::DPadUp)) {
						songID = 3;
						break;
					}
				if(input.holdFaceButtons.hasAny(Buttons::DPadDown)) {
						songID = 2;
						break;
					}
				if(input.holdFaceButtons.hasAny(Buttons::DPadLeft)) {
						songID = 1;
						break;
					}
				if(input.holdFaceButtons.hasAny(Buttons::DPadRight)) {
						songID = 0;
						break;
					}
				if(input.holdFaceButtons.hasAny(Buttons::A)) {
						songID = 4;
						break;
					}
				}
			}

			for(auto &player: getHumanPlayers()) {
				if(player.extremeGear == ExtremeGear::HangOn) {
				HOCheck = true;
				}
				if(player.extremeGear == ExtremeGear::SuperHangOn && SHOPlayer == nullptr) {
				SHOPlayer = &player;
				}
				// Feature: Hang-On/Super Hang-on music can now be suppressed by holding the X Button while the stage is loading.
				// Credit: Sirky
				// ------------------------------------------------------
				if(player.input->holdFaceButtons.hasAny(Buttons::X)) {
					CustomMusic_SkipSHOHOMusic = true;
				}
				// ------------------------------------------------------
			}

			if(SHOPlayer != nullptr) {
				if(CustomMusic_SkipSHOHOMusic && !isVanillaPlaylist) {
					break;
				}
                if (lbl_RNG_Number(100) == 5) {
					songID = 0x7C; // ooh toxic
					break;
				}
				songID = SHOPlayer->character + 5;

				switch(SHOPlayer->characterExload().exLoadID) {
					case EXLoad::E10R: break;
					case EXLoad::E10B: songID += 146; // SH162
						break;
					case EXLoad::HatsuneMiku: songID += 128; // SH142
						break;
					default: break;
				}
				break;
			}
			if(HOCheck) {
				if(CustomMusic_SkipSHOHOMusic && !isVanillaPlaylist) {
					break;
				}
				songID = 0x7D;
				break;
			}
			break;
		case 1: // menu music mode
			if (!isVanillaPlaylist) {
				songID = lbl_RNG_Number(MENU_MUSIC_COUNT);
			}
			break;
		case 2: // race end music mode
			if (!isVanillaPlaylist) {
				songID = lbl_RNG_Number(RACE_END_MUSIC_COUNT);
			}
			break;
		case 3: // battle music mode
			if (!isVanillaPlaylist) {
				songID = lbl_RNG_Number(BATTLE_MUSIC_COUNT);
			}
			break;
		default: break;
	}

	bss_CustomMusicID = songID;
}

USED bool CustomMusicPlayer(const u32 mode) {
	const u32 &songID = bss_CustomMusicID;
	const bool isVanillaPlaylist = DebugMenu_IsVanillaPlaylist();

	switch(mode) {
		case 0: // play stage music
			if(songID == 4) {
				return false;
			}                  // play vanilla music
			if(songID == 0x7C) // britney spears
			{
				PlayADX(gpasAdxtHandle_Bgm, easterEgg);
				break;
			}
			if(songID == 0x7D) {
				// hang on
				if (isVanillaPlaylist && CustomMusic_SkipSHOHOMusic) {
					PlayVanillaStageMusic();
				} else {
					PlayADX(gpasAdxtHandle_Bgm, HangOnMusic);
				}
				break;
			}
			if(songID > 4) { // super hang on
				if (isVanillaPlaylist) {
					if (CustomMusic_SkipSHOHOMusic) {
						PlayVanillaStageMusic(); // play vanilla stage music
						break;
					}

					return false; // play vanilla SHO music
				}

				// play custom SHO music
				const std::string superHangOn = std::format("SH{}.ADX", songID - 5);
				PlayADX(gpasAdxtHandle_Bgm, superHangOn.c_str());
				break;
			}

			// play custom stage music
			PlayADX(gpasAdxtHandle_Bgm, stageMusic[(CurrentStage - 1) * 4 + songID]);
			break;
		case 1: // play menu music
			if(songID >= MENU_MUSIC_COUNT || isVanillaPlaylist) {
				return false;
			}
			PlayADX(gpasAdxtHandle_Bgm, menuMusic[songID]);
			break;
		case 2: // play race end music
			if(songID >= RACE_END_MUSIC_COUNT || isVanillaPlaylist) {
				return false;
			}
			PlayADX(gpasAdxtHandle_Bgm, raceEndMusic[songID]);
			break;
		case 3: // play battle music
			if(songID >= BATTLE_MUSIC_COUNT || isVanillaPlaylist) {
				return false;
			}
			PlayADX(gpasAdxtHandle_Bgm, battleMusic[songID]);
			break;
		default: break;
	}

	return true;
}

USED const char *FixInvincibilitySongReplay() {
	const u32 &songID = bss_CustomMusicID;
	return stageMusic[(CurrentStage - 1) * 4 + songID];
}

extern u8 lbl_001E9680[];

void PlayVanillaStageMusic() {
	u8 musicIndex = lbl_001E9680[CurrentStage - 1];
	PlayADX(gpasAdxtHandle_Bgm, lbl_001E99BC[musicIndex]);
}

void CustomMusicChanger_OnPauseMenu() {
	u32 songID      = bss_CustomMusicID;
	Player *player1 = players.data();

	if(CurrentGameMode != BattleMode) {
		// accounts for if SHO/HO music is playing
		if(songID > 4) {
			return;
		}
	}

	if(player1->input->toggleFaceButtons.hasAny(Buttons::Y)) {
		songID++;
		if(CurrentGameMode != BattleMode) {
			if(songID > 4)
				songID = 0;
			bss_CustomMusicID = songID;
			if(!CustomMusicPlayer(0)) {
				PlayVanillaStageMusic();
			}
		} else {
			if(songID >= BATTLE_MUSIC_COUNT)
				songID = 0;
			bss_CustomMusicID = songID;
			CustomMusicPlayer(3);
		}
	}
}

void CustomMusicChanger_MainMenu() {
	if(gsActivePad.toggleButtons.hasAny(Buttons::Y)) {
		u32 songID = bss_CustomMusicID;
		songID++;
		if(songID >= MENU_MUSIC_COUNT)
			songID = 0;
		bss_CustomMusicID = songID;
		CustomMusicPlayer(1);
	}
}
