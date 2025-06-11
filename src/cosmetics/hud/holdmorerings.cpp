#include "riders/player.hpp"

/**
 * Returns the maximum amount of rings the player can hold.
 *
 * @param player The player to get max ring count from.
 * @return Maximum allowed ring count. Defaults to 100.
 */
[[nodiscard]] inline u32 GetMaxRings(Player *player) {
    switch (player->extremeGear) {
        using namespace ExtremeGear;
        case Gambler:
            return 150;
        case ChaosEmerald:
            break;
            // if (player->character != Character::Tails) {
            //     break;
            // }
            // return 200;
        case CoverF:
            return 200;
        case CoverS:
            return 125;
        case CoverP:
            return 300;
        default:
            break;
    }
    return 100;
}

/**
 * Keeps the HUD representation of the player's ring count accurate.
 */
ASMUsed u32 HUD_GearsCanHoldMoreRings(Player *player) {
    return GetMaxRings(player);
}

/**
 * Handles the player's max ring count.
 */
ASMUsed void Player_MaxRings(Player *player, u32 ringsOverHundred, u32 currTotalRings) {
    auto maxRingCount = GetMaxRings(player);
    ringsOverHundred = (ringsOverHundred > (maxRingCount - 100)) ? currTotalRings - maxRingCount : 0;
    player->rings = currTotalRings - ringsOverHundred;
}
