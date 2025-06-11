#pragma once

#include "mechanics/magneticimpulse.hpp"

void Player_Afterburner(const Player &player);

[[nodiscard]] constexpr u32 calculateAfterburnerTimer(const Player &player) {
    return (player.trickCount - 1u) * 60;
}