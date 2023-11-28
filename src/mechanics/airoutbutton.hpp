#pragma once

#include "riders/player.hpp"

struct CheatCodeInput {
    u8 buttonCount; // keeps incrementing to a certain point until it reaches the desired button count, where it then triggers
    u8 startingButton; // if needed, this can keep track of the starting button
    u16 delayFrames; // how many frames you have in between button presses to successfully perform cheat code
};

void Player_AirOutButton(Player *player);