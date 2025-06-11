#include "filehandler_dat.hpp"
#include "cosmetics/player/exloads.hpp"
#include "riders/player.hpp"
#include "lib/sound.hpp"
#include "lib/stdlib.hpp"

struct DATFileBSS {
	std::array<u32, 0x20> ints1;
	std::array<u32, 0x20> ints2;
};

//ASMDefined s8 lbl_1001A6E4;
//ASMDefined DATFileBSS lbl_10026620;
ASMDefined fillerData<0x100> lbl_801B2D98;

constexpr std::array<const char *, std::to_underlying(Character::Total)> EN_CharacterVoiceFiles = {{
		"10SONIC.DAT",
		"10TAILS.DAT",
		"10KNUCK.DAT",
		"10AMY.DAT",
		"10JET.DAT",
		"10STORM.DAT",
		"10WAVE.DAT",
		"10EGGM.DAT",
		"10CREAM.DAT",
		"10ROUGE.DAT",
		"10SHADO.DAT",
		"10SS.DAT",
		"10NIGHT.DAT",
		"10AIAI.DAT",
		"10ULALA.DAT",
		"10ER1.DAT",
		"10ER2.DAT",
		"10SILV.DAT",
		"10METAL.DAT",
		"10EMERL.DAT",
		"10BLAZE.DAT",
		"10CHAOS.DAT",
		"10TIKAL.DAT",
}};

constexpr std::array<const char *, std::to_underlying(Character::Total)> JP_CharacterVoiceFiles = {{
		"10JSONIC.DAT",
		"10JTAILS.DAT",
		"10JKNUCK.DAT",
		"10JAMY.DAT",
		"10JJET.DAT",
		"10JSTORM.DAT",
		"10JWAVE.DAT",
		"10JEGGM.DAT",
		"10JCREAM.DAT",
		"10JROUGE.DAT",
		"10JSHADO.DAT",
		"10JSS.DAT",
		"10NIGHT.DAT",
		"10JAIAI.DAT",
		"10JULALA.DAT",
		"10JER1.DAT",
		"10JER2.DAT",
		"10SILV.DAT",
		"10METAL.DAT",
		"10EMERL.DAT",
		"10BLAZE.DAT",
		"10CHAOS.DAT",
		"10TIKAL.DAT",
}};

// custom data

constexpr std::array<const char *, 2> E10Dats = {
		"10EB2.DAT",
		"10ER2.DAT",
};

/**
 * Selects the Dat file to used based on Game Language, Character, and Exloads
 *
 * @param player The player we are wanting to load the dat file for. Needed for exloads and checking the character
 * @param language The language we want to load for. Defaults to whatever the game language is. Needs testing for if this works without bugs
 * @return The Filename of the selected dat file
 */
const char *selectDatFile(Player &player, Language language = GameLanguage) {
	player.characterVoiceID = static_cast<Sound::ID::CharacterSoundIDs>(0); // Make sure we clear out the old value just in case the value is somehow never set
	if(player.hasExload()) {
		switch(player.characterExload().exLoadID) {
			case EXLoad::GonGon:
				player.characterVoiceID = Sound::ID::GonGon;
				return "10GONGO.DAT";
			case EXLoad::HatsuneMiku:
				player.characterVoiceID = Sound::ID::Miku;
				return "10MIKU.DAT";
			case EXLoad::Reala:
				player.characterVoiceID = Sound::ID::Reala;
				return "10REALA.DAT";
			case EXLoad::NeoMetal:
				player.characterVoiceID = Sound::ID::NeoMetalSonic;
				return "10NEO.DAT";
			default: break;
		}
		switch(player.gearExload().exLoadID) {
			case EXLoad::HyperSonic:
				player.characterVoiceID = Sound::ID::HyperSonic;
				return "10HS.DAT";
			default: break;
		}
	}
	if(isSuperCharacter(player, Character::MetalSonic)) {
		player.characterVoiceID = Sound::ID::NeoMetalSonic;
		return "10NEO.DAT";
	}
	const auto &selectedCharacter = player.character;
	if(selectedCharacter == Character::E10R) {
		player.characterVoiceID = Sound::ID::E10R;
		return E10Dats[0];
	}
	const std::span characterDatFile =
			language == Language::Japanese
					? JP_CharacterVoiceFiles
					: EN_CharacterVoiceFiles;
	if(selectedCharacter < characterDatFile.size()) {
		player.characterVoiceID = CharacterVoiceIDs[selectedCharacter];
		return characterDatFile[selectedCharacter];
	}
	std::unreachable();
}

ASMUsed void LoadCharacterDATFile() {
	//memset(&lbl_801B2D98, 0x7F, 0xFF);
	std::fill(lbl_801B2D98.begin(), lbl_801B2D98.end()-1, std::byte{0x7F});
	for(auto &currentPlayer: getCurrentPlayerList()) {
		if(currentPlayer.character < Character::Total) {
			LoadDATFileDynamic(selectDatFile(currentPlayer));
		}
	}
}