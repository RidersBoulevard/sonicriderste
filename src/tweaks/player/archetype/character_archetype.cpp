#include "character_archetype.hpp"
#include "cosmetics/player/exloads.hpp"
#include "riders/stage.hpp"

USED void Player_CharacterArchetype(Player *player) {
    if (CurrentStage == BabylonGarden || CurrentStage == SkyRoad) {
        // If the turbulence on these stages try to access this, deny it and return none
        // The old behavior interpreted this "player" to be sonic and always returned late booster,
        // So that is replicated here.
        if (player->index == InGamePlayerCount) {
            player->characterArchetype = CharacterArchetype::LateBooster;
            return;
        }
    }

    CharacterArchetype archetype = AllCharacterArchetypes[player->character];

    if (player->hasCharacterExload()) {
        const CharacterArchetype newArchetype = player->characterExload().archetype();
        if (newArchetype != CharacterArchetype::NoEXLoad) {
            archetype = newArchetype;
        }
    }

    if (player->isSuperCharacter(Character::MetalSonic)) {
        archetype = CharacterArchetype::None;
    }

    player->characterArchetype = archetype;
}