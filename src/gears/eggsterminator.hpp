#pragma once

#include "context.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct EggInfo {
	u32 extraLevel;
    u8 emeraldsInserted;
    BOOL magnetShieldEnabled;
    u8 refillAir;
};
#pragma GCC diagnostic pop

extern struct EggInfo PlayerEggsterminator[8];

global void lbl_0008CC74(u32);

global void Player_Eggsterminator(struct Player *player);
