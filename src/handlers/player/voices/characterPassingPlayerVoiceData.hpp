#pragma once

constexpr std::array<std::optional<u8>, Character::Total> CharacterPassingPlayerVoice {
		0x05,			// Sonic
		0x05,			// Tails
		0x05,			// Knuckles
		0x05,			// Amy
		0x05,			// Jet
		0x05,			// Storm
		0x05,			// Wave
		0x05,			// Eggman
		0x05,			// Cream
		0x05,			// Rouge
		0x05,			// Shadow
		0x05,			// Super Sonic
		0x0A,			// Nights
		0x05,			// Aiai
		0x05,			// Ulala
		std::nullopt,	// E10G
		std::nullopt,	// E10R
		0x04,			// Silver
		0x04,			// Metal Sonic
		0x04,			// Emerl
		0x04,			// Blaze
		0x04,			// Chaos
		0x04,			// Tikal
};

constexpr std::array<std::optional<u8>, EXLoadCount> EXLoadCharacterPassingVoice {
		std::nullopt,
		std::nullopt,	// Perfect Nazo
		std::nullopt,	// E10R
		std::nullopt,	// Dark Sonic
		std::nullopt,	// Hyper Sonic
		std::nullopt,	// Stardust Speeder
		std::nullopt,	// Hyper HangOn
		std::nullopt,	// The Professional
		std::nullopt,	// Ollie King Gear
		std::nullopt,	// Windmaster Jet
		std::nullopt,	// Neo Metal
		std::nullopt,	// E99
		std::nullopt,	// The Beast
		std::nullopt,	// E10B
		0x04,			// Miku
		std::nullopt,	// Metal Sonic Scarf
		0x0A,			// Reala
		0x04,			// Gongon
};