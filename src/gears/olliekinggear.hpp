#pragma once

#include "context.hpp"

struct OKGInfo {
	u8 OllieBurstStatus;
    f32 currentSpeed;
    // BOOL flyStateEntry;
};

extern struct OKGInfo PlayerOKGInfo[8];
extern const f32 OllieKingGear_BoostSpeeds[7];
extern const f32 OllieKingGear_KickdashSpeeds[3];
extern const f32 OllieKingGear_KickdashCap[3];

void Player_OllieKingGear(struct Player *player);