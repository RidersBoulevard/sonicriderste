#include "globalDefs.hpp"
#include "riders/player.hpp"

/**
 * Controls the respawn timer for an individual picked up ring.
 */
ASMUsed u32 RingRespawnTimer(const u8 playerIndex) {
    constexpr u32 DEFAULT_RESPAWN_TIMER = 360;
    constexpr u32 MAGNET_RESPAWN_TIMER = DEFAULT_RESPAWN_TIMER / 2;

    Player &player = players[playerIndex];

    return player.statusEffectFlags.hasAny(PlayerStatus::MagnetStatus) ?
        MAGNET_RESPAWN_TIMER : DEFAULT_RESPAWN_TIMER;
}