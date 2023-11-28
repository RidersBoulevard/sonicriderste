#pragma once

constexpr std::array<TrickLandVoiceLineList, TotalCharacterAmount> TrickLandVoiceTable {{
		{.cRank = 0x01, .aRank = 0x02},								// Sonic
		{.cRank = 0x01, .aRank = 0x02},								// Tails
		{.cRank = 0x01, .aRank = 0x02},								// Knuckles
		{.cRank = 0x01, .aRank = 0x02},								// Amy
		{.cRank = 0x01, .aRank = 0x02},								// Jet
		{.cRank = 0x01, .aRank = 0x02},								// Storm
		{.cRank = 0x01, .aRank = 0x02},								// Wave
		{.cRank = 0x01, .aRank = 0x02},								// Eggman
		{.cRank = 0x01, .aRank = 0x02},								// Cream
		{.cRank = 0x01, .aRank = 0x02},								// Rouge
		{.cRank = 0x01, .aRank = 0x02},								// Shadow
		{.cRank = 0x01, .aRank = 0x02},								// Super Sonic
		{.cRank = 0x01, .aRank = 0x02, .xRank = 0x3},				// Nights
		{.cRank = 0x01, .aRank = 0x02},								// Aiai
		{.cRank = 0x01, .aRank = 0x02},								// Ulala
		{.aRank = 0x00},               								// E10G
		{.cRank = 0x01, .aRank = 0x02},								// E10R
		{.cRank = 0x01, .aRank = 0x02},								// Silver
		{.cRank = 0x01, .aRank = 0x02, .tenPlus = 0x6},				// Metal Sonic
		{.cRank = 0x01, .aRank = 0x02},								// Emerl
		{.cRank = 0x01, .aRank = 0x02},								// Blaze
		{.cRank = 0x01, .aRank = 0x02},								// Chaos
		{.cRank = 0x01, .aRank = 0x02},								// Tikal
}};

constexpr std::array<TrickLandVoiceLineList, EXLoadCount> EXLoadTrickLandVoiceTable {{
		{},
		{},															// Perfect Nazo
		{},															// E10R
		{},															// Dark Sonic
		{},															// Hyper Sonic
		{},															// Stardust Speeder
		{},															// Hyper HangOn
		{},															// The Professional
		{},															// Ollie King Gear
		{},															// Windmaster Jet
		{},															// Neo Metal
		{},															// E99
		{},															// The Beast
		{},															// E10B
		{.cRank = 0x01, .aRank = 0x02, .sRank = 10, .xRank = 11},	// Miku
		{},															// Metal Sonic Scarf
		{.cRank = 1, .aRank = 2, .xRank = 3},						// Reala
		{.cRank = 1, .aRank = 2},									// Gongon
}};