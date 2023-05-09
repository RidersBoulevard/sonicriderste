#include "sound.hpp"
#include "cosmetics/player/exloads.hpp"

struct VoiceLineList{
	static constexpr const u8 defaultValue = 0xFF;
	u8 cRank = defaultValue;
	u8 aRank = defaultValue;
	u8 sRank = defaultValue;
	u8 xRank = defaultValue;
	u8 tenPlus = defaultValue;
};

constexpr std::array<VoiceLineList, TotalCharacterAmount> TrickLandVoiceTable = {{
	{.cRank = 0x01, .aRank = 0x02}, // Sonic
	{.cRank = 0x01, .aRank = 0x02}, // Tails
	{.cRank = 0x01, .aRank = 0x02}, // Knuckles
	{.cRank = 0x01, .aRank = 0x02}, // Amy
	{.cRank = 0x01, .aRank = 0x02}, // Jet
	{.cRank = 0x01, .aRank = 0x02}, // Storm
	{.cRank = 0x01, .aRank = 0x02}, // Wave
	{.cRank = 0x01, .aRank = 0x02}, // Eggman
	{.cRank = 0x01, .aRank = 0x02}, // Cream
	{.cRank = 0x01, .aRank = 0x02}, // Rouge
	{.cRank = 0x01, .aRank = 0x02}, // Shadow
	{.cRank = 0x01, .aRank = 0x02}, // SuperSonic
	{.cRank = 0x01, .aRank = 0x03}, // Nights
	{.cRank = 0x01, .aRank = 0x02}, // Aiai
	{.cRank = 0x01, .aRank = 0x02}, // Ulala
	{.aRank = 0x00},                // E10G
	{.cRank = 0x01, .aRank = 0x02}, // E10R
	{.cRank = 0x01, .aRank = 0x02}, // Silver
	{.cRank = 0x01, .aRank = 0x02}, // MetalSonic
	{.cRank = 0x01, .aRank = 0x02}, // Emerl
	{.cRank = 0x01, .aRank = 0x02}, // Blaze
	{.cRank = 0x01, .aRank = 0x02}, // Chaos
	{.cRank = 0x01, .aRank = 0x02}, // Tikal
}};

constexpr std::array<VoiceLineList, EXLoadCount> EXLoadTrickLandVoiceTable = {{
     {},
     {},
     {},
     {},
     {},
     {},
     {},
     {},
     {},
     {},
     {},
     {},
     {},
     {},
     {.cRank = 0x08, .aRank = 0x09}, // Miku
     {},
     {.cRank = 20, .aRank = 21}, // Reala
     {.cRank = 10, .aRank = 11}, // GonGon
}};

ASMUsed void HandleTrickLand(Player &player){
	if(player.aiControl) { return; }
	const VoiceLineList &characterLines = TrickLandVoiceTable[player.character];

    EnabledEXLoads exLoads{};
    FetchEnabledEXLoadIDs(&player, exLoads);

    const VoiceLineList &exLoadLines = EXLoadTrickLandVoiceTable[exLoads.characterExLoadID];

	u8 selectedLine = VoiceLineList::defaultValue;
	switch(player.trickRank){
		case XRank:
			selectedLine = characterLines.xRank;
            if (exLoadLines.xRank != VoiceLineList::defaultValue) { selectedLine = exLoadLines.xRank; }
			if(selectedLine != VoiceLineList::defaultValue) { break; }
			[[fallthrough]];
		case SRank ... SPlusRank:
			selectedLine = characterLines.sRank;
            if (exLoadLines.sRank != VoiceLineList::defaultValue) { selectedLine = exLoadLines.sRank; }
			if(selectedLine != VoiceLineList::defaultValue) { break; }
			[[fallthrough]];
		case APlusRank:
			selectedLine = characterLines.aRank;
            if (exLoadLines.aRank != VoiceLineList::defaultValue) { selectedLine = exLoadLines.aRank; }
			if(selectedLine != VoiceLineList::defaultValue) { break; }
			[[fallthrough]];
		case BRank ... ARank:
			break; // Don't play anything
		case CRank:
			selectedLine = characterLines.cRank;
            if (exLoadLines.cRank != VoiceLineList::defaultValue) { selectedLine = exLoadLines.cRank; }
			break;
		default:
			break;
	}
	if(player.trickCount > 10 && characterLines.tenPlus != VoiceLineList::defaultValue){ // NOLINT(readability-magic-numbers)
		selectedLine = characterLines.tenPlus;
	}
	if(selectedLine == VoiceLineList::defaultValue) { return; }
	Sound::PlaySound(player, selectedLine);
}

/*
constexpr std::array<u8, TotalCharacterAmount> AttackedVoiceTable = {
        0x04,   // Sonic
        0x04,   // Tails
        0x04,   // Knuckles
        0x04,   // Amy
        0x04,   // Jet
        0x04,   // Storm
        0x04,   // Wave
        0x04,   // Eggman
        0x04,   // Cream
        0x04,   // Rouge
        0x04,   // Shadow
        0x04,   // SuperSonic
        0x06,   // Nights
        0x04,   // Aiai
        0x04,   // Ulala
        0x02,   // E10G
        0x02,   // E10R
        0x03,   // Silver
        0x03,   // MetalSonic
        0x03,   // Emerl
        0x03,   // Blaze
        0x03,   // Chaos
        0x03    // Tikal
};

ASMUsed void HandleCharacterAttack(Player &player){
	if(player.aiControl) { return; }
	const u8 &selectedLine = AttackedVoiceTable[player.character];
	Sound::PlaySound(player, selectedLine);
}
*/