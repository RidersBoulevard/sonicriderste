#include "cosmetics/player/exloads.hpp"
#include "gears/blastGaugeGears.hpp"
#include "gears/hypersonic.hpp"

ASMUsed void Player_StickyFingersRequiem(Player *attackingPlayer, Player *attackedPlayer) {
    s32 airToAdd, extraAir, thirtyPercentCurrent, thirtyPercentMax;
    f32 scalingMultiplier;
    if (!attackingPlayer->specialFlags.hasAny(stickyFingers)) return;
    
    if (attackingPlayer->extremeGear == ExtremeGear::Accelerator) {
        airToAdd = (attackingPlayer->gearStats[attackingPlayer->level].maxAir * 20) / 100;
        thirtyPercentCurrent = (attackedPlayer->currentAir * 30) / 100;
        thirtyPercentMax = (attackedPlayer->gearStats[attackedPlayer->level].maxAir * 30) / 100;

        if (thirtyPercentCurrent > thirtyPercentMax) extraAir = thirtyPercentMax;
        else extraAir = thirtyPercentCurrent;

        scalingMultiplier = static_cast<f32>(attackingPlayer->gearStats[attackingPlayer->level].maxAir) /
                            static_cast<f32>(attackedPlayer->gearStats[attackedPlayer->level].maxAir);

        airToAdd += static_cast<s32>(static_cast<f32>(extraAir) * scalingMultiplier);

        attackingPlayer->currentAir += airToAdd;

    } else if (isSuperCharacter(*attackingPlayer, Shadow)) {
        attackingPlayer->currentAir += (attackingPlayer->gearStats[attackingPlayer->level].maxAir * 30) / 100;
    }
    else if (!attackingPlayer->specialFlags.hasAny(ringGear)) {
        if (attackingPlayer->extremeGear == ExtremeGear::CoverP) {attackingPlayer->rings += 30;}
        else attackingPlayer->rings += 15;
    } /*else if (attackingPlayer->character == SuperSonic) {
		EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*attackingPlayer);
        if (exLoads.gearExLoadID == DarkSonicEXLoad) {
            BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[attackingPlayer->index];
            bgInfo->currentGauge += 60000;
            HyperSonicInfo *hsInfo = &PlayerHyperSonicInfo[attackingPlayer->index];
            if (hsInfo->hyperdriveEnabled){
				attackingPlayer->currentAir += (attackingPlayer->gearStats[attackingPlayer->level].maxAir * 15) / 100;
			}
        }
    }*/ else {
        attackingPlayer->currentAir += (attackingPlayer->gearStats[attackingPlayer->level].maxAir * 15) / 100;
    }
}