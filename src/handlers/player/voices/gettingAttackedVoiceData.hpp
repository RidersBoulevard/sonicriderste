#pragma once

constexpr std::array<std::optional<u8>, TotalCharacterAmount> GettingAttackedVoice{
		0x04,			// Sonic
		0x04,			// Tails
		0x04,			// Knuckles
		0x04,			// Amy
		0x04,			// Jet
		0x04,			// Storm
		0x04,			// Wave
		0x04,			// Eggman
		0x04,			// Cream
		0x04,			// Rouge
		0x04,			// Shadow
		0x04,			// Super Sonic
		0x06,			// Nights
		0x04,			// Aiai
		0x04,			// Ulala
		0x02,			// E10G
		0x02,			// E10R
		0x03,			// Silver
		0x03,			// Metal Sonic
		0x03,			// Emerl
		0x03,			// Blaze
		0x03,			// Chaos
		0x03,			// Tikal
};

constexpr std::array<std::optional<u8>, EXLoadCount> EXLoadGettingAttackedVoice {
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
		0x03,			// Miku
		std::nullopt,	// Metal Sonic Scarf
		0x06,			// Reala
		0x03,			// Gongon
};
