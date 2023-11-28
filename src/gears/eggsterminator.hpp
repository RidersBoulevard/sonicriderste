#pragma once

#include "riders/player.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct EggInfo {
	u32 extraLevel;
    u8 emeraldsInserted;
    bool magnetShieldEnabled;
    u8 refillAir;
};
#pragma GCC diagnostic pop

extern EggInfo PlayerEggsterminator[8];

ASMDefined void lbl_0008CC74(u32);

ASMDefined void Player_Eggsterminator(Player *player);
