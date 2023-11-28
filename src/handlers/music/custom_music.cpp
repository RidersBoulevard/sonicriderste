#include "custom_music.hpp"
#include "lib/stdlib.hpp"
#include "riders/gamemode.hpp"
#include "riders/stage.hpp"
#include "cosmetics/player/exloads.hpp"
#include <format>

struct ActivePad {
	u32 holdButtons;
	u32 toggleButtons;
};
extern ActivePad gsActivePad;

SubFont CustomMusicTextData;

USED void CustomMusicRandomizer(const u32 mode) {
	u32 songID = 0;
	u32 i;
	Controller *input = players[0].input;
	const Player *SHOPlayer = nullptr;
	bool HOCheck = false;
	bool XCheck = false;

	switch(mode) {
		case 0:// stage music mode
			songID = lbl_RNG_Number(4);
			for(i = 0; i < MaxControllerCount; i++) {
				if(input[i].holdFaceButtons.hasAny(DPadUp)) {
					songID = 3;
					break;
				}
				if(input[i].holdFaceButtons.hasAny(DPadDown)) {
					songID = 2;
					break;
				}
				if(input[i].holdFaceButtons.hasAny(DPadLeft)) {
					songID = 1;
					break;
				}
				if(input[i].holdFaceButtons.hasAny(DPadRight)) {
					songID = 0;
					break;
				}
				if(input[i].holdFaceButtons.hasAny(AButton)) {
					songID = 4;
					break;
				}
			}
			for(i = 0; i < MaxControllerCount; i++) {
				const Player &player = players[i];
					if (player.extremeGear == ExtremeGear::HangOn) 
					{HOCheck = true;}
					if (player.extremeGear == ExtremeGear::SuperHangOn && SHOPlayer == nullptr)
					{SHOPlayer = &player;}
					// Feature: Hang-On/Super Hang-on music can now be suppressed by holding the X Button while the stage is loading.
						// Credit: Sirky
					// ------------------------------------------------------
					if (player.input->holdFaceButtons.hasAny(XButton)) 
					{XCheck = true;}
					// ------------------------------------------------------
			}
				
				if (SHOPlayer != nullptr) {
                    if (XCheck) {break;}
                    if (lbl_RNG_Number(100) == 5) {
						songID = 0x7C; // ooh toxic
						break;
					}
					songID = SHOPlayer->character + 5;

					EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*SHOPlayer);

					switch (exLoads.characterExLoadID) {
						case E10REXLoad:
							break;
						case E10BEXLoad:
							songID += 146; // SH162
							break;
						case HatsuneMikuEXLoad:
							songID += 128; // SH142
							break;
						default:
							break;
					}
					break;
				}
				if (HOCheck) {
					if (XCheck) {break;}
					songID = 0x7D;
					break;
				}
			break;
		case 1:// menu music mode
			songID = lbl_RNG_Number(MENU_MUSIC_COUNT);
			break;
		case 2:// race end music mode
			songID = lbl_RNG_Number(RACE_END_MUSIC_COUNT);
			break;
		case 3:// battle music mode
			songID = lbl_RNG_Number(BATTLE_MUSIC_COUNT);
			break;
		default:
			break;
	}

	bss_CustomMusicID = songID;
}

USED bool CustomMusicPlayer(const u32 mode) {
	const u32 &songID = bss_CustomMusicID;

	switch(mode) {
		case 0:                              // play stage music
			if(songID == 4) { return FALSE; }// play vanilla music
			if(songID == 0x7C)               // britney spears
			{
				PlayADX(gpasAdxtHandle_Bgm, easterEgg);
				break;
			}
			if(songID == 0x7D) {
				// hang on
				PlayADX(gpasAdxtHandle_Bgm, HangOnMusic);
				break;
			}
			if(songID > 4){ // super hang on
				const std::string superHangOn = std::format("SH{}.ADX", songID - 5);
				PlayADX(gpasAdxtHandle_Bgm, superHangOn.c_str());
				break;
			}
			PlayADX(gpasAdxtHandle_Bgm, stageMusic[(CurrentStage - 1) * 4 + songID]);
			break;
		case 1:// play menu music
			if(songID >= MENU_MUSIC_COUNT) { return FALSE; }
			PlayADX(gpasAdxtHandle_Bgm, menuMusic[songID]);
			break;
		case 2:// play race end music
			if(songID >= RACE_END_MUSIC_COUNT) { return FALSE; }
			PlayADX(gpasAdxtHandle_Bgm, raceEndMusic[songID]);
			break;
		case 3:// play battle music
			if(songID >= BATTLE_MUSIC_COUNT) { return FALSE; }
			PlayADX(gpasAdxtHandle_Bgm, battleMusic[songID]);
			break;
		default:
			break;
	}

	return TRUE;
}

USED const char *FixInvincibilitySongReplay() {
	u32 songID = bss_CustomMusicID;
	return stageMusic[(CurrentStage - 1) * 4 + songID];
}

extern u8 lbl_001E9680[];
void PlayVanillaStageMusic() {
	u8 musicIndex = lbl_001E9680[CurrentStage - 1];
	PlayADX(gpasAdxtHandle_Bgm, lbl_001E99BC[musicIndex]);
}

void CustomMusicChanger_OnPauseMenu() {
	u32 songID = bss_CustomMusicID;
	Player *player1 = players.data();

	if(CurrentGameMode != BattleMode) {
		// accounts for if SHO/HO music is playing
		if(songID > 4) return;
	}

	if(player1->input->toggleFaceButtons & YButton) {
		songID++;
		if(CurrentGameMode != BattleMode) {
			if(songID > 4) songID = 0;
			bss_CustomMusicID = songID;
			if(!CustomMusicPlayer(0)) {
				PlayVanillaStageMusic();
			}
		} else {
			if(songID >= BATTLE_MUSIC_COUNT) songID = 0;
			bss_CustomMusicID = songID;
			CustomMusicPlayer(3);
		}
	}
}

void CustomMusicChanger_MainMenu() {
	if(gsActivePad.toggleButtons & YButton) {
		u32 songID = bss_CustomMusicID;
		songID++;
		if(songID >= MENU_MUSIC_COUNT) songID = 0;
		bss_CustomMusicID = songID;
		CustomMusicPlayer(1);
	}
}