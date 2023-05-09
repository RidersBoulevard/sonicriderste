#pragma once

#include "context.hpp"
#include "gears/supertails.hpp"

struct WindmasterInfo {
	u8 lastShortcutType; // 1 = rail, 2 = fly
    BOOL FirstRail;
    BOOL jumpedFromRail;
    BOOL hasFlown;
    f32 railJumpDelay;
    BOOL qteEntered;
};

extern struct WindmasterInfo PlayerWMInfo[8];

extern const s32 DynamoLevelCosts[3];
extern const s32 DynamoLevelStartingAmounts[3];
extern const u32 DynamoLevelRings[3];

void Player_Dynamo(struct Player *player);

// void Player_DeltaStream(struct Player *player);