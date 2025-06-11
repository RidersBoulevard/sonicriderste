#pragma once

#include "riders/controller.hpp"
#include "riders/object.hpp"
#include <optional>

namespace DebugMenuOptions {
	enum Page1 {
		DisableMusic,                        //= 1 << 0,
		MagneticImpulse,                     //= 1 << 1,
		Autopilot,                           //= 1 << 2,
		ExtremeDetach,                       //= 1 << 3,
		TornadoIgnore,                       //= 1 << 4,
		TimerActivity_ActiveInSingleplayer,  //= 1 << 5,
		TimerActivity_ActiveIn1v1,           //= 1 << 6,
		TimerActivity_ActiveIn1v1Middle,     //= 1 << 7,
		TimerActivity_ActiveIn3OrMorePlayers,//= 1 << 8,
		DisableHUDPartial,
		DisableHUDFull,
		InfiniteAir,
		InfiniteRings,
		AlwaysMaxMI,
		MusicPlaylist, // if this enum is toggled it's vanilla playlist
        ClutchAsTexture,
        ClutchAsAirGauge
	};
	constexpr auto PAGE1OPTIONCOUNT = 12;
	constexpr auto DefaultPage1Options = 1 << MagneticImpulse
	                                     | 1 << Autopilot
	                                     | 1 << TornadoIgnore
	                                     | 1 << TimerActivity_ActiveInSingleplayer
	                                     | 1 << ExtremeDetach
                                         | 1 << ClutchAsAirGauge;
}// namespace DebugMenuOptions

struct DebugMenuData {
	std::array<std::span<u8>, DebugMenuOptions::PAGE1OPTIONCOUNT> page1Options;

	u8 state = 0;
	u8 selectedItemRow = 0;
	u8 selectedItemColumn = 0;
	u8 maximumItems = 0;

	// bit field that corresponds to enum DebugMenuOptions
	u32 toggledPageOptions = DebugMenuOptions::DefaultPage1Options;
};

struct Text2dFileData {
	std::array<void *, 2> textData;
	std::array<void *, 2> extraTextData;
};

struct AllPlayerInputs {
	Flag<Buttons> holdButtons;
	Flag<Buttons> toggleButtons;
};

struct TitleSequenceObject1 {
	fillerData<0xD> filler;
	s8 currentButtonIndex;
	s8 lastButtonIndex;
	s8 currentMode;
};

struct Text2dFileHeader {
	u16 width;
	u16 height;
	u32 unknown;
	u32 textCount;
};

extern Text2dFileData DebugMenu_TextData;
extern DebugMenuData DebugMenu_Data;

bool DebugMenu_CheckOption(u32 option);
void DebugMenu_ToggleOption(std::span<u8> options);
void DebugMenu_ToggleOptionSet(std::span<u8> options, u32 direction);
void DebugMenu_HandleAllToggles(std::span<u8> options, u32 direction);
u32 DebugMenu_FetchTextIDAllToggles(std::span<u8> options);
ASMUsed void DebugMenu_Handler(ObjectNode *object, AllPlayerInputs *inputs);
std::optional<u8> DebugMenu_FetchOptionFromOptionSet(std::span<u8> options);
u32 DebugMenu_FetchTextID(u32 option);
ASMUsed void DebugMenu_RenderDescription();
[[nodiscard]] bool DebugMenu_IsVanillaPlaylist();
