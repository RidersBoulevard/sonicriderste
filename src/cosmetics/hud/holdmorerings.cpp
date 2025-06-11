#include "riders/player.hpp"

/**
 * Keeps the HUD representation of the player's ring count accurate.
 */
ASMUsed u32 HUD_GearsCanHoldMoreRings(const Player &player) {
    return player.max_rings();
}

/**
 * Handles the player's max ring count.
 */
ASMUsed void Player_MaxRings(Player &player, u32 ringsOverHundred, const u32 currTotalRings) {
	const auto maxRingCount = player.max_rings();
    ringsOverHundred = (ringsOverHundred > (maxRingCount - 100)) ? currTotalRings - maxRingCount : 0;
    player.rings = currTotalRings - ringsOverHundred;
}
