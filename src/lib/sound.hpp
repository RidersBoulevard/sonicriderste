#pragma once

#include "riders/character.hpp"
#include "types.hpp"

#include <array>

ASMDefined void PlayAudioFromDAT(u32);

namespace Sound{
	namespace ID{
		// This is for getting the ID of a specific character
		enum CharacterSoundIDs : u16{
			VSFX			= 0xA900,
			IDKSFX			= 0xA901, // Possibly want to rename this one
			Sonic 			= 0xA902,
			Tails 			= 0xA903,
			Knuckles 		= 0xA904,
			Jet 			= 0xA905,
			Wave 			= 0xA906,
			Storm 			= 0xA907,
			Amy 			= 0xA908,
			Cream 			= 0xA909,
			Eggman 			= 0xA90A,
			E10G 			= 0xA90B,
			E10R 			= 0xA90C,
			Shadow 			= 0xA90D,
			Rouge 			= 0xA90E,
			EXTSFX 			= 0xA90F, // Not sure what this is but its used
			SuperSonic 		= 0xA91E,
			Aiai 			= 0xA91F,
			Ulala 			= 0xA920,
			SFX				= 0xA932,
			Silver 			= 0xA933,
			MetalSonic 		= 0xA934,
			Emerl 			= 0xA935,
			Blaze 			= 0xA936,
			Chaos 			= 0xA937,
			Tikal 			= 0xA938,
			MSFX			= 0xA939,
			Nights 			= 0xA93A,
			// exloads below
			NeoMetalSonic	= 0xA97B,
			HyperSonic		= 0xA97C,
			GonGon 			= 0xA97D,
			Miku 			= 0xA97E,
			Reala 			= 0xA97F,
		};
	}

	constexpr u32 ComposeSound(const ID::CharacterSoundIDs &characterSoundID, const u8 &soundID){
		return static_cast<u32>(characterSoundID << 16 | soundID << 8);
	}

	inline void PlaySound(const ID::CharacterSoundIDs &characterSoundID, const u8 &soundID){
		PlayAudioFromDAT(ComposeSound(characterSoundID, soundID));
	}

	namespace SFX{
		enum SFXs{ // Enums dump their values into the parent scope, which will be this SFX namespace
			FallingIntoAPit 		= ComposeSound(ID::SFX, 0x00),
			MagnetShield 			= ComposeSound(ID::SFX, 0x01),
			TornadoSlingshot 		= ComposeSound(ID::SFX, 0x02),
			SuperTransformation 	= ComposeSound(ID::SFX, 0x03),
			TenOrMoreTricks 		= ComposeSound(ID::SFX, 0x04),
			TornadoIgnore 			= ComposeSound(ID::SFX, 0x05),
			TornadoHit 				= ComposeSound(ID::SFX, 0x06),
			MaximumMI 				= ComposeSound(ID::SFX, 0x07),
			MIActive 				= ComposeSound(ID::SFX, 0x08),
			MIDormant 				= ComposeSound(ID::SFX, 0x09),
			MIZero 					= ComposeSound(ID::SFX, 0x0A),
			RailHoopDash 			= ComposeSound(ID::SFX, 0x0B),
			EmerlCopyType 			= ComposeSound(ID::SFX, 0x0C),
			EmerlLoseType 			= ComposeSound(ID::SFX, 0x0D),
			MaxJumpCharge 			= ComposeSound(ID::SFX, 0x0E),
			HyperDrive 				= ComposeSound(ID::SFX, 0x0F),
			HyperBoost 				= ComposeSound(ID::SFX, 0x10),
			HyperStream 			= ComposeSound(ID::SFX, 0x11),
			HOTrans1 				= ComposeSound(ID::SFX, 0x12),
			HOTrans2 				= ComposeSound(ID::SFX, 0x13),
			HOTrans3 				= ComposeSound(ID::SFX, 0x14),
			GoldExp 				= ComposeSound(ID::SFX, 0x15),
			CovFModeSwitch 			= ComposeSound(ID::SFX, 0x16),
			SuperLeap 				= ComposeSound(ID::SFX, 0x17)
		};
	}

	namespace VSFX{
		enum VSFXs{
			MenuScroll = ComposeSound(ID::VSFX, 0x00),
			MenuConfirm = ComposeSound(ID::VSFX, 0x01),
			MenuBack = ComposeSound(ID::VSFX, 0x02)
		};
	}

	namespace IDKSFX{
		enum IDKSFXs{

		};
	}
}

// This is for getting the ID by the characters index
constexpr std::array<Sound::ID::CharacterSoundIDs, TotalCharacterAmount> CharacterVoiceIDs = {
		Sound::ID::Sonic,
		Sound::ID::Tails,
		Sound::ID::Knuckles,
		Sound::ID::Amy,
		Sound::ID::Jet,
		Sound::ID::Storm,
		Sound::ID::Wave,
		Sound::ID::Eggman,
		Sound::ID::Cream,
		Sound::ID::Rouge,
		Sound::ID::Shadow,
		Sound::ID::SuperSonic,
		Sound::ID::Nights,
		Sound::ID::Aiai,
		Sound::ID::Ulala,
		Sound::ID::E10G,
		Sound::ID::E10R,
		Sound::ID::Silver,
		Sound::ID::MetalSonic,
		Sound::ID::Emerl,
		Sound::ID::Blaze,
		Sound::ID::Chaos,
		Sound::ID::Tikal,
};

namespace Sound{
	constexpr u32 ComposeSound(const u8 &characterID, const u8 &soundID){
		return static_cast<u32>(CharacterVoiceIDs[characterID] << 16 | soundID << 8);
	}

	inline void PlaySound(const u8 &characterID, const u8 &soundID){
		PlayAudioFromDAT(ComposeSound(characterID, soundID));
	}
}