#include "deathplane.hpp"
#include "handlers/player/SetPlayerState.hpp"

void Player_DeathPlane(Player *player) {
    if (player->y < -2000.0f) {
        if (player->state != Death) {
            func_SetPlayerActionType(player, Death);
        }
    }
}