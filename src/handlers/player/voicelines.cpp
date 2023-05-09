#include "voicelines.hpp"
#include "lib/sound.hpp"
#include "cosmetics/player/exloads.hpp"

ASMUsed void PlayPassingCharacterVoice(Player *player) {
    s32 voice = CharacterPassingPlayerVoice[player->character];

    EnabledEXLoads exLoads{};
    FetchEnabledEXLoadIDs(player, exLoads);

    const s32 exLoadVoice = EXLoadCharacterPassingVoice[exLoads.characterExLoadID];
    if (exLoadVoice != -1) {
        voice = exLoadVoice;
    }

    if (voice != -1) {
        PlayAudioFromDAT(voice);
    }
}

// currently unused, a lot of places to patch for this
global void PlayGettingAttackedVoice(Player *player) {
    s32 voice = gas32GettingAttackedVoice[player->character];

    EnabledEXLoads exLoads{};
    FetchEnabledEXLoadIDs(player, exLoads);

    const s32 exLoadVoice = EXLoadGettingAttackedVoice[exLoads.characterExLoadID];
    if (exLoadVoice != -1) {
        voice = exLoadVoice;
    }

    if (voice != -1) {
        PlayAudioFromDAT(voice);
    }
}

ASMUsed void PlayTrickJumpVoice(Player *player) {
    s32 voice = gas32TrickJumpVoice[player->character];

    EnabledEXLoads exLoads{};
    FetchEnabledEXLoadIDs(player, exLoads);

    const s32 exLoadVoice = EXLoadTrickJumpVoice[exLoads.characterExLoadID];
    if (exLoadVoice != -1) {
        voice = exLoadVoice;
    }

    if (voice != -1) {
        PlayAudioFromDAT(voice);
    }
}