#pragma once

#include "riders/player.hpp"

ASMDefined void Player_ZTurbulenceRetractAir(Player *player);
ASMDefined void Player_SetInvulnerabilityFromZTurbulence(Player *player);
void Player_IgnoreTurbulenceHandler(Player *player);