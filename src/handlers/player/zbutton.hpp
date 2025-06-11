#pragma once

#include "riders/player.hpp"

extern std::array<u8, MaxPlayerCount> PlayerZButtonHeldTimers;

ASMDefined void Player_ZTurbulenceRetractAir(Player &player);
ASMDefined void Player_SetInvulnerabilityFromZTurbulence(Player &player);
void Player_IgnoreTurbulenceHandler(Player &player);