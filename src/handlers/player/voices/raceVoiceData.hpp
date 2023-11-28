#pragma once

#include "lib/stdlib.hpp"
using RaceStartVoiceFunc = u8 (*)(const Player &);

constexpr RaceStartVoiceFunc noRaceStartVoice = nullptr;

template<u8 voiceID>
constexpr RaceStartVoiceFunc defaultRaceStartVoice = [](const Player &) -> u8 {
	return voiceID;
};

constexpr std::array<RaceStartVoiceFunc, TotalCharacterAmount> raceStartLines{
		defaultRaceStartVoice<7>,		// Sonic
		defaultRaceStartVoice<9>,		// Tails
		defaultRaceStartVoice<8>,		// Knuckles
		defaultRaceStartVoice<9>,		// Amy
		defaultRaceStartVoice<9>,		// Jet
		defaultRaceStartVoice<9>,		// Storm
		defaultRaceStartVoice<9>,		// Wave
		defaultRaceStartVoice<9>,		// Eggman
		defaultRaceStartVoice<8>,		// Cream
		defaultRaceStartVoice<8>,		// Rouge
		defaultRaceStartVoice<9>,		// Shadow
		defaultRaceStartVoice<7>,		// Super Sonic
		defaultRaceStartVoice<20>,		// Nights
		defaultRaceStartVoice<9>,		// Aiai
		defaultRaceStartVoice<7>,		// Ulala
		defaultRaceStartVoice<4>,		// E10G
		defaultRaceStartVoice<4>,		// E10R
		defaultRaceStartVoice<11>,		// Silver
		defaultRaceStartVoice<10>,		// Metal Sonic
		defaultRaceStartVoice<8>,		// Emerl
		defaultRaceStartVoice<10>,		// Blaze
		defaultRaceStartVoice<8>,		// Chaos
		defaultRaceStartVoice<11>,		// Tikal
};

constexpr std::array<RaceStartVoiceFunc, EXLoadCount> exLoadRaceStartLines{
		noRaceStartVoice,				// Having no voice func on exloads means default to regular character
		noRaceStartVoice,				// Perfect Nazo
		noRaceStartVoice,				// E10R
		noRaceStartVoice,				// Dark Sonic
		noRaceStartVoice,				// Hyper Sonic
		noRaceStartVoice,				// Stardust Speeder
		noRaceStartVoice,				// Hyper HangOn
		noRaceStartVoice,				// The Professional
		noRaceStartVoice,				// Ollie King Gear
		noRaceStartVoice,				// Windmaster Jet
		noRaceStartVoice,				// Neo Metal
		noRaceStartVoice,				// E99
		noRaceStartVoice,				// The Beast
		noRaceStartVoice,				// E10B
		defaultRaceStartVoice<13>,		// Miku
		noRaceStartVoice,				// Metal Sonic Scarf
		noRaceStartVoice,				// Reala
		defaultRaceStartVoice<10>,		// Gongon
};

using RaceEndVoiceFunc = u8 (*)(const Player &, bool win);

constexpr RaceEndVoiceFunc noRaceEndVoice = nullptr;

template<u8 winID, u8 loseID = winID+1>
constexpr RaceEndVoiceFunc defaultRaceEndVoice = [](const Player &, bool win) -> u8 {
	return win ? winID : loseID;
};

constexpr std::array<RaceEndVoiceFunc, TotalCharacterAmount> raceEndLines{
		defaultRaceEndVoice<8>,			// Sonic
		defaultRaceEndVoice<10>,		// Tails
		defaultRaceEndVoice<9>,			// Knuckles
		defaultRaceEndVoice<10>,		// Amy
		defaultRaceEndVoice<10>,		// Jet
		defaultRaceEndVoice<10>,		// Storm
		defaultRaceEndVoice<10>,		// Wave
		defaultRaceEndVoice<10>,		// Eggman
		defaultRaceEndVoice<9>,			// Cream
		defaultRaceEndVoice<9>,			// Rouge
		defaultRaceEndVoice<10>,		// Shadow
		defaultRaceEndVoice<8>,			// Super Sonic
		defaultRaceEndVoice<21>,		// Nights
		defaultRaceEndVoice<10>,		// Aiai
		defaultRaceEndVoice<8>,			// Ulala
		defaultRaceEndVoice<5>,			// E10G
		defaultRaceEndVoice<5>,			// E10R
		defaultRaceEndVoice<12>,		// Silver
		defaultRaceEndVoice<11>,		// Metal Sonic
		defaultRaceEndVoice<9>,			// Emerl
		defaultRaceEndVoice<11>,		// Blaze
		defaultRaceEndVoice<9>,			// Chaos
		defaultRaceEndVoice<12>,		// Tikal
};

constexpr RaceEndVoiceFunc hyperSonicRaceEndVoice = [](const Player &, bool win) -> u8 {
	if(!win){
		return 11;
	}
	return lbl_RNG_Number(2) + 8;
};

constexpr std::array<RaceEndVoiceFunc, EXLoadCount> exLoadRaceEndLines{
		noRaceEndVoice,					// Having no voice func on exloads means default to regular character
		noRaceEndVoice,					// Perfect Nazo
		noRaceEndVoice,					// E10R
		noRaceEndVoice,					// Dark Sonic
		hyperSonicRaceEndVoice,			// Hyper Sonic
		noRaceEndVoice,					// Stardust Speeder
		noRaceEndVoice,					// Hyper HangOn
		noRaceEndVoice,					// The Professional
		noRaceEndVoice,					// Ollie King Gear
		noRaceEndVoice,					// Windmaster Jet
		noRaceEndVoice,					// Neo Metal
		noRaceEndVoice,					// E99
		noRaceEndVoice,					// The Beast
		noRaceEndVoice,					// E10B
		defaultRaceEndVoice<14>,		// Miku
		noRaceEndVoice,					// Metal Sonic Scarf
		noRaceEndVoice,					// Reala
		defaultRaceEndVoice<11>,		// Gongon
};