#pragma once

#include "context.hpp"

extern "C" void Player_ZTurbulenceRetractAir(struct Player *player);
extern "C" void Player_SetInvulnerabilityFromZTurbulence(struct Player *player);
void Player_IgnoreTurbulenceHandler(struct Player *player);