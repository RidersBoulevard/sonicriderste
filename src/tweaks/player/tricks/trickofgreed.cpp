#include "context.hpp"

ASMUsed void Player_TrickOfGreed(Player *player) {
    if (player->subState == 0 && player->trickRank == SRank) {
        if (player->character == SuperSonic) return;
        if (player->specialFlags & ringGear) {
			auto mult = player->extremeGear == Accelerator ? 5 : 3;
	        s32 newAir = player->currentAir - ((player->gearStats[0].maxAir / 100) * mult);

            player->currentAir = clamp(newAir, 0);

        } else if (player->extremeGear == SuperHangOn) {
            player->currentAir -= 10000;
        }
    }
}