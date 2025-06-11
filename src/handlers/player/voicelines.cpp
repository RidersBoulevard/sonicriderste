#include "voicelines.hpp"
#include "lib/sound.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
// All the data in arrays and stuff is in these files.
#include "voices/attackingVoiceData.hpp"
#include "voices/characterPassingPlayerVoiceData.hpp"
#include "voices/characterSelectVoiceData.hpp"
#include "voices/gettingAttackedVoiceData.hpp"
#include "voices/raceVoiceData.hpp"
#include "voices/trickJumpVoiceData.hpp"
#include "voices/trickLandVoiceData.hpp"
#pragma GCC diagnostic pop

inline void HandleVoiceLineWithExLoad(const Player &player, std::span<const std::optional<u8>, std::to_underlying(Character::Total)> mainLines, std::span<const std::optional<u8>, EXLoadCount> exloadLines) {
	const auto &exLoadVoice = exloadLines[player.characterExload().id()];
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
	const auto &exLoadLines = EXLoadAttackingVoiceTable[player.characterExload().id()];

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
	const auto &exLoadLines = EXLoadTrickLandVoiceTable[player.characterExload().id()];

	const auto &rank = player.trickCount > 10 ? TrickRanks::TenPlus : player.trickRank;

	const auto selectedLine = player.hasExload()
									? characterLines.combine(exLoadLines)[rank]
									: characterLines[rank];
	if(!selectedLine) { return; }
	Sound::PlaySound(player, *selectedLine);
}

ASMUsed void HandleCharacterAttack(const Player &attackedPlayer, const bool skipPlayingAttackedVoice = false) {
	const auto *attackingPlayer = attackedPlayer.attackedPlayer;
	if(attackingPlayer != nullptr){
		if(attackedPlayer.aiControl && attackingPlayer->aiControl){ return; }
		PlayAttackingVoice(*attackingPlayer);
	}

	if(skipPlayingAttackedVoice) { return; }
	PlayGettingAttackedVoice(attackedPlayer);
}

template<typename VoiceSelectFunc, size_t exloadSize, typename ...Args>
auto VoiceHandler(const Player &player, const VoiceSelectFunc &regularFunc, std::span<const VoiceSelectFunc, exloadSize> exloadFuncs, Args && ...args){
	auto func = regularFunc;
	if(player.hasGearExload()){
		func = exloadFuncs[player.gearExload().id()] ?: func; // https://gcc.gnu.org/onlinedocs/gcc-9.2.0/gcc/Conditionals.html#Conditionals
	}

	if(player.hasCharacterExload() && (player.character == player.characterExload().character)){
		func = exloadFuncs[player.characterExload().id()] ?: func;
	}
	if(func == nullptr) { std::unreachable(); }
	return func(std::forward<Args>(args)...);
}

ASMDefined std::array<u8, 0x1C> lbl_8021BC14;

ASMUsed void raceVoiceHandler_RaceEndVoice(const Player &player) {
	if(lbl_8021BC14[0x14] == 3) { return; } // This was in the old code, not sure what it does
	if(player.aiControl) { return; }

	auto selectedVoice = VoiceHandler(player,
									  raceEndLines[player.character],
									  std::span(exLoadRaceEndLines),
									  player, player.placement_counter == 0
									  );
	Sound::PlaySound(player, selectedVoice);
}

ASMUsed void raceVoiceHandler_RaceStartVoice(u8 playerIndex) {
	const auto &player = players[playerIndex];

	auto selectedVoice = VoiceHandler(player,
									  raceStartLines[player.character],
									  std::span(exLoadRaceStartLines),
									  player
	);
	Sound::PlaySound(player, selectedVoice);
}

ASMUsed void PlayCSSVoice(const Player &player){
	auto selectedVoice = VoiceHandler(player,
									  characterSelectLines[player.character],
									  std::span(exLoadCharacterSelectLines),
									  player
	);
	Sound::PlaySound(Sound::ID::CSSV, selectedVoice);
}