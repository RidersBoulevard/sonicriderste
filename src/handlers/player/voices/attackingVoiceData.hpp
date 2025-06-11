#pragma once

using AVLP = AttackingVoiceLinePair;
// Todo: Do... something... about this mess
constexpr std::array<AttackingVoiceLineList, Character::Total> AttackingVoiceTable {{
		{// Sonic
				.lvl1 = AVLP(0x6,	{0x1b}),
				.lvl2 = AVLP(		{0x1a, 0x6}),
				.lvl3 = AVLP(		{0x27}),
		},
		{// Tails
				.lvl1 = AVLP(0x6, 	{0x2B}),
				.lvl2 = AVLP(0x6, 	{0x1A, 0x6}),
				.lvl3 = AVLP(0x7),
		},
		{// Knuckles
				.lvl1 = AVLP(		{0x1C}),
				.lvl2 = AVLP(0x6, 	{0x28, 0x2D, 0x6}),
				.lvl3 = AVLP(0x7, 	{0x1A, 0x2D}),
		},
		{ // Amy
				.lvl1 = AVLP(0x6),
				.lvl2 = AVLP(0x7, 	{0x6}),
				.lvl3 = AVLP(0x8, 	{0x23}),
		},
		{// Jet
				.lvl1 = AVLP(0x6),
				.lvl2 = AVLP(0x7,	{0x25, 0x6}),
				.lvl3 = AVLP(0x8,	{0x1A}),
		},
		{// Storm
				.lvl1 = AVLP(0x6),
				.lvl2 = AVLP(0x7,	{0x6}),
				.lvl3 = AVLP(0x8),
		},
		{// Wave
				.lvl1 = AVLP(0x6),
				.lvl2 = AVLP(0x7,	{0x2F, 0x6}),
				.lvl3 = AVLP(0x8),
		},
		{// Eggman
				.lvl1 = AVLP(0x6,	{0x33}),
				.lvl2 = AVLP(0x7,	{0x35, 0x6}),
				.lvl3 = AVLP(0x8,	{0x34}),
		},
		{// Cream
				.lvl1 = AVLP(0x6),
				.lvl2 = AVLP(0x7,	{0x6}),
				.lvl3 = AVLP(0x8),
		},
		{// Rouge
				.lvl1 = AVLP({0x29}),
				.lvl2 = AVLP(0x3,	{0x6}),
				.lvl3 = AVLP({}),
		},
		{// Shadow
				.lvl1 = AVLP(0x6),
				.lvl2 = AVLP(0x7,	{0x28, 0x6}),
				.lvl3 = AVLP(0x8,	{0x28}),
		},
		{// Super Sonic
		 .lvl1 = AVLP(		{0x6})
		},
		{// Nights
				.lvl1 = AVLP(16,	{}),
				.lvl2 = AVLP(17,	{0x1C, 0x6}),
				.lvl3 = AVLP(18,	{}),
				//.lvl4 = AVLP(19,	{}),
		},
		{// Aiai
				.lvl1 = AVLP(0x6),
				.lvl2 = AVLP(0x7),
				.lvl3 = AVLP(0x8),
		},
		{// Ulala
				.lvl1 = AVLP(0x6),
				.lvl2 = AVLP(		{}),
				.lvl3 = AVLP(		{}),
		},
		{// E10G
				.lvl1 = AVLP(0x3, 	{0x1B}),
				.lvl2 = AVLP(		{0x6}),
				.lvl3 = AVLP(		{}),
		},
		{// E10R
				.lvl1 = AVLP(0x3, 	{0x1B}),
				.lvl2 = AVLP(		{0x6}),
				.lvl3 = AVLP(		{}),
		},
		{// Silver
				.lvl1 = AVLP(0x5),
				.lvl2 = AVLP(0x6,	{0x28, 0x6}),
				.lvl3 = AVLP(0x7,	{0x28}),
		},
		{// Metal Sonic
				.lvl1 = AVLP(0x5,	{0x1b}),
				.lvl2 = AVLP(		{0x1a, 0x6}),
				.lvl3 = AVLP(		{0x27}),
		},
		{// Emerl
				.lvl1 = AVLP(0x5,	{0x2B}),
				.lvl2 = AVLP(0x5,	{0x1A, 0x6}),
				.lvl3 = AVLP(0x6),
		},
		{// Blaze
				.lvl1 = AVLP(0x5),
				.lvl2 = AVLP(0x6,	{0x2F, 0x6}),
				.lvl3 = AVLP(0x7),
		},
		{// Chaos
				.lvl1 = AVLP({0x1C}),
				.lvl2 = AVLP(0x5, {0x28, 0x2D, 0x6}),
				.lvl3 = AVLP(0x6, {0x1A, 0x2D}),
		},
		{// Tikal
				.lvl1 = AVLP(0x5),
				.lvl2 = AVLP(0x6,	{0x2F, 0x6}),
				.lvl3 = AVLP(0x7),
		}
}};

constexpr std::array<AttackingVoiceLineList, EXLoadCount> EXLoadAttackingVoiceTable {{
		{}, // Empty braces mean default to regular character, Anything else will override
		{},// Perfect Nazo
		{},// E10R
		{},// Dark Sonic
		{},// Hyper Sonic
		{},// Stardust Speeder
		{},// Hyper HangOn
		{},// The Professional
		{},// Ollie King Gear
		{},// Windmaster Jet
		{},// Neo Metal
		{},// E99
		{},// The Beast
		{},// E10B
		{// Miku
				.lvl1 = AVLP(0x5),
				.lvl2 = AVLP(0x6),
				.lvl3 = AVLP(0x7),
		},
		{},// Metal Sonic Scarf
		{// Reala
				.lvl1 = AVLP(16),
				.lvl2 = AVLP(17, 	{0x1C, 0x6}),
				.lvl3 = AVLP(18),
				//.lvl4 = AVLP(19,	{}),
		},
		{// GonGon
				.lvl1 = AVLP(0x5),
				.lvl2 = AVLP(0x6),
				.lvl3 = AVLP(0x7),
		},
}};
