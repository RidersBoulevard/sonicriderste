#pragma once

using CharacterSelectVoiceFunc = u8 (*)(const Player &);

constexpr CharacterSelectVoiceFunc noCharacterSelectVoice = nullptr;

constexpr CharacterSelectVoiceFunc defaultCharacterSelectVoice = [](const Player &player) -> u8 {
	return player.character;
};

constexpr CharacterSelectVoiceFunc extraCharacterSelectVoice = [](
        [[maybe_unused]] const Player &player) -> u8 {
    return Character::Shadow;
};

template<u8 voiceID>
constexpr CharacterSelectVoiceFunc exloadCharacterSelectVoice = [](const Player &) -> u8 {
	return voiceID;
};

constexpr std::array<CharacterSelectVoiceFunc, Character::Total + 1> characterSelectLines{
		defaultCharacterSelectVoice,	// Sonic
		defaultCharacterSelectVoice,	// Tails
		defaultCharacterSelectVoice,	// Knuckles
		defaultCharacterSelectVoice,	// Amy
		defaultCharacterSelectVoice,	// Jet
		defaultCharacterSelectVoice,	// Storm
		defaultCharacterSelectVoice,	// Wave
		defaultCharacterSelectVoice,	// Eggman
		defaultCharacterSelectVoice,	// Cream
		defaultCharacterSelectVoice,	// Rouge
		defaultCharacterSelectVoice,	// Shadow
		defaultCharacterSelectVoice,	// Super Sonic
		defaultCharacterSelectVoice,	// Nights
		defaultCharacterSelectVoice,	// Aiai
		defaultCharacterSelectVoice,	// Ulala
		defaultCharacterSelectVoice,	// E10G
		defaultCharacterSelectVoice,	// E10R
		defaultCharacterSelectVoice,	// Silver
		defaultCharacterSelectVoice,	// Metal Sonic
		defaultCharacterSelectVoice,	// Emerl
		defaultCharacterSelectVoice,	// Blaze
		defaultCharacterSelectVoice,	// Chaos
		defaultCharacterSelectVoice,	// Tikal
        extraCharacterSelectVoice,      // Extra Variable Character
};

constexpr std::array<CharacterSelectVoiceFunc, EXLoadCount> exLoadCharacterSelectLines{
		noCharacterSelectVoice,			// Having no voice func on exloads means default to regular character
		noCharacterSelectVoice,			// Perfect Nazo
		noCharacterSelectVoice,			// E10R
		noCharacterSelectVoice,			// Dark Sonic
		exloadCharacterSelectVoice<24>,	// Hyper Sonic
		noCharacterSelectVoice,			// Stardust Speeder
		noCharacterSelectVoice,			// Hyper HangOn
		noCharacterSelectVoice,			// The Professional
		noCharacterSelectVoice,			// Ollie King Gear
		noCharacterSelectVoice,			// Windmaster Jet
		exloadCharacterSelectVoice<23>,	// Neo Metal
		noCharacterSelectVoice,			// E99
		noCharacterSelectVoice,			// The Beast
		noCharacterSelectVoice,			// E10B
		exloadCharacterSelectVoice<26>,	// Miku
		noCharacterSelectVoice,			// Metal Sonic Scarf
		exloadCharacterSelectVoice<27>,	// Reala
		exloadCharacterSelectVoice<25>,	// Gongon
};