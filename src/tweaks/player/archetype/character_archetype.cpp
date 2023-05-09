#include "character_archetype.hpp"
#include "cosmetics/player/exloads.hpp"

USED void Player_CharacterArchetype(Player *player) {
    CharacterArchetypes archetype = AllCharacterArchetypes[player->character];

    EnabledEXLoads exLoads{};
    FetchEnabledEXLoadIDs(player, exLoads);

    if (exLoads.characterExLoadID != NoneEXLoad) {
        CharacterArchetypes const newArchetype = EXLoadArchetypes[exLoads.characterExLoadID];
        if (newArchetype != NoEXLoadArchetype) {
            archetype = newArchetype;
        }
    }

    if (isSuperCharacter(*player, MetalSonic)) {
        player->characterArchetype = NoneArchetype;
    } else {
        player->characterArchetype = archetype;
    }
}