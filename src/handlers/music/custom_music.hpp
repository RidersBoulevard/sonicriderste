#pragma once

#include "handlers/ingame/customtext.hpp"

ASMDefined void PlayADX(void *gpasAdxtHandle_Bgm, const char[]);
ASMDefined void *gpasAdxtHandle_Bgm;
ASMDefined u32 bss_CustomMusicID;
ASMDefined std::array<const char *, 87> lbl_001E99BC;
extern SubFont CustomMusicTextData;

ASMUsed void CustomMusicRandomizer(u32 mode);
ASMUsed bool CustomMusicPlayer(u32 mode);
ASMUsed const char *FixInvincibilitySongReplay();
ASMUsed void CustomMusicChanger_OnPauseMenu();
ASMUsed void CustomMusicChanger_MainMenu();
void PlayVanillaStageMusic();

constexpr auto STAGE_MUSIC_COUNT = 64;
constexpr auto MENU_MUSIC_COUNT = 31;
constexpr auto RACE_END_MUSIC_COUNT = 19;
constexpr auto BATTLE_MUSIC_COUNT = 12;

constexpr std::array<const char *, STAGE_MUSIC_COUNT> stageMusic = {
		"00.ADX", "01.ADX", "02.ADX", "03.ADX",
		"04.ADX", "05.ADX", "06.ADX", "07.ADX",
		"08.ADX", "09.ADX", "0A.ADX", "0B.ADX",
		"0C.ADX", "0D.ADX", "0E.ADX", "0F.ADX",
		"10.ADX", "11.ADX", "12.ADX", "13.ADX",
		"14.ADX", "15.ADX", "16.ADX", "17.ADX",
		"18.ADX", "19.ADX", "1A.ADX", "1B.ADX",
		"1C.ADX", "1D.ADX", "1E.ADX", "1F.ADX",
		"20.ADX", "21.ADX", "22.ADX", "23.ADX",
		"24.ADX", "25.ADX", "26.ADX", "27.ADX",
		"28.ADX", "29.ADX", "2A.ADX", "2B.ADX",
		"2C.ADX", "2D.ADX", "2E.ADX", "2F.ADX",
		"30.ADX", "31.ADX", "32.ADX", "33.ADX",
		"34.ADX", "35.ADX", "36.ADX", "37.ADX",
		"38.ADX", "39.ADX", "3A.ADX", "3B.ADX",
		"3C.ADX", "3D.ADX", "3E.ADX", "3F.ADX"
};

constexpr std::array<const char *, MENU_MUSIC_COUNT> menuMusic = {
		"40.ADX", "41.ADX", "42.ADX", "43.ADX",
		"44.ADX", "45.ADX", "46.ADX", "47.ADX",
		"48.ADX", "49.ADX", "4A.ADX", "4B.ADX",
		"4C.ADX", "4D.ADX", "4E.ADX", "4F.ADX",
		"50.ADX", "51.ADX", "52.ADX", "53.ADX",
		"54.ADX", "55.ADX", "56.ADX", "57.ADX",
		"58.ADX", "59.ADX", "5A.ADX", "5B.ADX",
		"5C.ADX", "5D.ADX", "5E.ADX"
};

constexpr std::array<const char *, RACE_END_MUSIC_COUNT> raceEndMusic = {
		"90.ADX", "91.ADX", "92.ADX", "93.ADX",
		"94.ADX", "95.ADX", "96.ADX", "97.ADX",
		"98.ADX", "99.ADX", "9A.ADX", "9B.ADX",
		"9C.ADX", "9D.ADX", "9E.ADX", "9F.ADX",
		"A0.ADX", "A1.ADX", "A2.ADX"
};

constexpr std::array<const char *, BATTLE_MUSIC_COUNT> battleMusic = {
		"70.ADX", "71.ADX", "72.ADX", "73.ADX",
		"74.ADX", "75.ADX", "76.ADX", "77.ADX",
		"78.ADX", "79.ADX", "7A.ADX", "7B.ADX"
};

constexpr const char *easterEgg = "7C.ADX";

constexpr const char *HangOnMusic = "S22.ADX";