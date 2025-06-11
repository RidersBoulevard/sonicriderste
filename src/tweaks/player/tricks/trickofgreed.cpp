#include "lib/stdlib.hpp"
#include "riders/player.hpp"
#include "gears/blastGaugeGears.hpp"
#include "gears/supers.hpp"

ASMUsed void Player_TrickOfGreed(Player *player) {
    if (player->subState == 0 && player->trickRank == SRank) {
        if (player->character == Character::SuperSonic && player->extremeGear == ExtremeGear::ChaosEmerald) { return; }
        if (player->specialFlags.hasAny(SpecialFlags::ringGear)) {
			const auto mult = 3;
	        const s32 newAir = player->currentAir - ((player->gearStats[0].maxAir / 100) * mult);

            player->currentAir = clamp(newAir);

            // if(isSuperCharacter(*player, Character::Shadow)) {
            //     SuperShadowInfo *sshInfo = &PlayerSuperShadowInfo[player->index];
            //     if (sshInfo->inhibitorRingsOff) return;
	        //     BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];
            //     bgInfo->currentGauge += 30000;
            // }
        } else if (player->extremeGear == ExtremeGear::SuperHangOn) {
            player->currentAir -= 10000;
        } 
        else if (player->extremeGear == ExtremeGear::Gambler) {
            auto newRings = static_cast<s32>(player->rings - 5);
            player->rings = static_cast<u32>(clamp(newRings));
        }
    }
}