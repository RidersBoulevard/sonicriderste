#include "lib/stdlib.hpp"
#include "riders/player.hpp"

ASMUsed void Player_TrickOfGreed(Player *player) {
    if (player->subState == 0 && player->trickRank == SRank) {
        if (player->character == SuperSonic) { return; }
        if (player->specialFlags.hasAny(ringGear)) {
			const auto mult = player->extremeGear == ExtremeGear::Accelerator ? 5 : 3;
	        const s32 newAir = player->currentAir - ((player->gearStats[0].maxAir / 100) * mult);

            player->currentAir = clamp(newAir, 0);
        } else if (player->extremeGear == ExtremeGear::SuperHangOn) {
            player->currentAir -= 10000;
        }
    }
}