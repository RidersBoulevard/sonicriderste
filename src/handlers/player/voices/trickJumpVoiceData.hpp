#pragma once

constexpr std::array<std::optional<u8>, Character::Total> TrickJumpVoice{
		0x00,			// Sonic
		0x00,			// Tails
		0x00,			// Knuckles
		0x00,			// Amy
		0x00,			// Jet
		0x00,			// Storm
		0x00,			// Wave
		0x00,			// Eggman
		0x00,			// Cream
		0x00,			// Rouge
		0x00,			// Shadow
		0x00,			// Super Sonic
		0x00,			// Nights
		0x00,			// Aiai
		0x00,			// Ulala
		std::nullopt,	// E10G
		0x04,			// E10R
		0x00,			// Silver
		0x00,			// Metal Sonic
		0x00,			// Emerl
		0x00,			// Blaze
		0x00,			// Chaos
		0x00,			// Tikal
};

constexpr std::array<std::optional<u8>, EXLoadCount> EXLoadTrickJumpVoice{
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
		0x00,			// Miku
		std::nullopt,	// Metal Sonic Scarf
		0x00,			// Reala
		0x00,			// Gongon
};