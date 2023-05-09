#pragma once

#include "context.hpp"

constexpr f32 HangOn_Speeds[4][2] = {
        // top speed, boost speed
        {pSpeed(185), pSpeed(230)}, // gear 1
        {pSpeed(215), pSpeed(260)}, // gear 2
        {pSpeed(245), pSpeed(280)}, // gear 3
};

void Player_HangOn(Player *player);
