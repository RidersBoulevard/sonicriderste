#pragma once

#include "context.hpp"
#include "gears/supertails.hpp"

struct WindmasterInfo {
	u8 lastShortcutType; // 1 = rail, 2 = fly
    bool FirstRail;
    bool jumpedFromRail;
    bool hasFlown;
    f32 railJumpDelay;
    bool qteEntered;
};

extern WindmasterInfo PlayerWMInfo[8];

extern const s32 DynamoLevelCosts[3];
extern const s32 DynamoLevelStartingAmounts[3];
extern const u32 DynamoLevelRings[3];

void Player_Dynamo(Player *player);

// void Player_DeltaStream(Player *player);