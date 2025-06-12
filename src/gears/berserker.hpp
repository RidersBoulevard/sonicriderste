#pragma once

#include "riders/player.hpp"

extern std::array<u8, MaxPlayerCount> Player_BerserkerStatIncreaseMultipliers;

ASMUsed void Player_BerserkerStatGain(Player &attackedPlayer, Player &attackingPlayer);
ASMUsed void Player_BerserkerStatResetTornado(Player &player);
void Player_BerserkerSpeedCheck(Player *player);

// NOTE: MOVE THIS TO ANOTHER FILE LATER
void Player_cancelAttackHack(Player &player);