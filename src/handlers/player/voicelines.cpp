#include "voicelines.hpp"
#include "lib/sound.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
// All the data in arrays and stuff is in these files.
#include "voices/gettingAttackedVoiceData.hpp"
#include "voices/attackingVoiceData.hpp"
#include "voices/trickJumpVoiceData.hpp"
#include "voices/characterPassingPlayerVoiceData.hpp"
#include "voices/trickLandVoiceData.hpp"
#include "voices/raceVoiceData.hpp"
#pragma GCC diagnostic pop

inline void HandleVoiceLineWithExLoad(const Player &player, std::span<const std::optional<u8>, TotalCharacterAmount> mainLines, std::span<const std::optional<u8>, EXLoadCount> exloadLines) {
	const auto &exLoadVoice = exloadLines[FetchEnabledEXLoadIDs(player).characterExLoadID];
	auto voice = mainLines[player.character];
	if(exLoadVoice) {
		voice = exLoadVoice;
	}

	Sound::PlaySound(player, voice);
}

ASMUsed void PlayPassingCharacterVoice(const Player &player) {
	HandleVoiceLineWithExLoad(player, CharacterPassingPlayerVoice, EXLoadCharacterPassingVoice);
}

void PlayGettingAttackedVoice(const Player &player) {
	HandleVoiceLineWithExLoad(player, GettingAttackedVoice, EXLoadGettingAttackedVoice);
}

void PlayAttackingVoice(const Player &player) {
	const auto &characterLines = AttackingVoiceTable[player.character];
	const auto exLoads = FetchEnabledEXLoadIDs(player);
	const auto &exLoadLines = EXLoadAttackingVoiceTable[exLoads.characterExLoadID];

	const auto &attackLevel = player.level;
	const auto combinedLines = characterLines.combine(exLoadLines);
	const auto &selectedLine = *combinedLines[attackLevel];
	if(selectedLine.voice) {
		Sound::PlaySound(player, *selectedLine.voice);
	}
	for(const auto &sfx : selectedLine.sfx) {
		if(!sfx) { break; }
		PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, *sfx));
	}
}

ASMUsed void PlayTrickJumpVoice(const Player &player) {
	HandleVoiceLineWithExLoad(player, TrickJumpVoice, EXLoadTrickJumpVoice);
}

ASMUsed void HandleTrickLand(Player &player) {
	if(player.aiControl) { return; }
	const auto &characterLines = TrickLandVoiceTable[player.character];
	const auto exLoads = FetchEnabledEXLoadIDs(player);
	const auto &exLoadLines = EXLoadTrickLandVoiceTable[exLoads.characterExLoadID];

	const auto &rank = player.trickCount > 10 ? TrickRanks::TenPlus : player.trickRank;

	const auto &selectedLine = exLoads.isExloadEnabled()
									? characterLines.combine(exLoadLines)[rank]
									: characterLines[rank];
	if(!selectedLine) { return; }
	Sound::PlaySound(player, *selectedLine);
}

ASMUsed void HandleCharacterAttack(const Player &attackedPlayer, const bool skipPlayingAttackedVoice = false) {
	const auto &attackingPlayer = *attackedPlayer.attackedPlayer;
	if(attackedPlayer.aiControl && attackingPlayer.aiControl) { return; }
	PlayAttackingVoice(attackingPlayer);

	if(skipPlayingAttackedVoice) { return; }
	PlayGettingAttackedVoice(attackedPlayer);
}

ASMDefined std::array<u8, 0x1C> lbl_8021BC14;

ASMUsed void raceVoiceHandler_RaceEndVoice(const Player &player) {
	if(lbl_8021BC14[0x14] == 3) { return; } // This was in the old code, not sure what it does
	if(player.aiControl) { return; }

	auto func = raceEndLines[player.character];
	auto exLoadInfo = FetchEnabledEXLoadIDs(player);
	if(exLoadInfo.isExloadEnabled()){
		if(exLoadInfo.gearExLoadID != NoneEXLoad){
			func = exLoadRaceEndLines[exLoadInfo.gearExLoadID] ?: func; // https://gcc.gnu.org/onlinedocs/gcc-9.2.0/gcc/Conditionals.html#Conditionals
		}

		if(exLoadInfo.characterExLoadID != NoneEXLoad){
			func = exLoadRaceEndLines[exLoadInfo.characterExLoadID] ?: func;
		}
	}
	if(func == nullptr) { return; }
	auto selectedVoice = func(player, player.placement_counter == 0);
	Sound::PlaySound(player, selectedVoice);
}

ASMUsed void raceVoiceHandler_RaceStartVoice(u8 playerIndex) {
	const auto &player = players[playerIndex];

	auto func = raceStartLines[player.character];
	const auto exLoadInfo = FetchEnabledEXLoadIDs(player);
	if(exLoadInfo.isExloadEnabled()){
		if(exLoadInfo.gearExLoadID != NoneEXLoad){
			auto temp = exLoadRaceStartLines[exLoadInfo.gearExLoadID];
			if(temp != nullptr){
				func = temp;
			}
		}

		if(exLoadInfo.characterExLoadID != NoneEXLoad){
			auto temp = exLoadRaceStartLines[exLoadInfo.characterExLoadID];
			if(temp != nullptr){
				func = temp;
			}
		}
	}
	auto selectedVoice = func(player);
	Sound::PlaySound(player, selectedVoice);
}