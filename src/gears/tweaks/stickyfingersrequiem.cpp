#include "cosmetics/player/exloads.hpp"
#include "gears/blastGaugeGears.hpp"
#include "gears/hypersonic.hpp"
#include "gears/faster.hpp"
#include "handlers/player/specialflagtweaks.hpp"
#include "lib/sound.hpp"
#include "tweaks/player/archetype/boostarchetypejcbc.hpp"

ASMUsed void Player_StickyFingersRequiem(Player *attackingPlayer, Player *attackedPlayer) {
    s32 airToAdd, extraAir, thirtyPercentCurrent, thirtyPercentMax;
    f32 scalingMultiplier;

    if (!attackingPlayer->specialFlags.hasAny(SpecialFlags::stickyFingers)) return;
    
    if (attackingPlayer->extremeGear == ExtremeGear::Accelerator) {
        if (attackingPlayer->gearExload().exLoadID == EXLoad::HyperHangOn) {
            attackingPlayer->currentAir += (attackingPlayer->gearStats[attackingPlayer->level].maxAir * 20) / 100;
            BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[attackingPlayer->index];
            bgInfo->currentGauge -= 20000;
            if (bgInfo->currentGauge < 0) bgInfo->currentGauge = 0; // bodge
            return;
        }
        airToAdd = (attackingPlayer->gearStats[attackingPlayer->level].maxAir * 20) / 100;
        thirtyPercentCurrent = (attackedPlayer->currentAir * 30) / 100;
        thirtyPercentMax = (attackedPlayer->gearStats[attackedPlayer->level].maxAir * 30) / 100;

        if (thirtyPercentCurrent > thirtyPercentMax) extraAir = thirtyPercentMax;
        else extraAir = thirtyPercentCurrent;

        scalingMultiplier = static_cast<f32>(attackingPlayer->gearStats[attackingPlayer->level].maxAir) /
                            static_cast<f32>(attackedPlayer->gearStats[attackedPlayer->level].maxAir);

        airToAdd += static_cast<s32>(static_cast<f32>(extraAir) * scalingMultiplier);

        attackingPlayer->currentAir += airToAdd;

    } else if (attackingPlayer->isSuperCharacter(Character::Shadow)) {
        attackingPlayer->currentAir += (attackingPlayer->gearStats[attackingPlayer->level].maxAir * 20) / 100;
    } else if (attackingPlayer->extremeGear == ExtremeGear::AdvantageF && attackingPlayer->specialFlags.hasAny(SpecialFlags::ringGear)) {
        attackingPlayer->currentAir += (attackingPlayer->gearStats[attackingPlayer->level].maxAir * 5) / 100;
    }
    else if (!attackingPlayer->specialFlags.hasAny(SpecialFlags::ringGear)) {
        if (attackingPlayer->extremeGear == ExtremeGear::CoverP) {attackingPlayer->rings += 20;}
        else if (attackingPlayer->extremeGear == ExtremeGear::Gambler) {attackingPlayer->rings += 5;}
        else { attackingPlayer->rings += 15; }
    }
    else {
        attackingPlayer->currentAir += (attackingPlayer->gearStats[attackingPlayer->level].maxAir * 15) / 100;
    }
}