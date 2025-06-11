#include "cosmetics/player/exloads.hpp"
#include "gears/blastGaugeGears.hpp"
#include "gears/hypersonic.hpp"
#include "handlers/player/specialflagtweaks.hpp"
#include "lib/sound.hpp"

ASMUsed void Player_StickyFingersRequiem(Player *attackingPlayer, Player *attackedPlayer) {
    s32 airToAdd, extraAir, thirtyPercentCurrent, thirtyPercentMax;
    f32 scalingMultiplier;
    if (!attackingPlayer->specialFlags.hasAny(SpecialFlags::stickyFingers)) return;
    
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

    } else if (isSuperCharacter(*attackingPlayer, Character::Shadow)) {
        attackingPlayer->currentAir += (attackingPlayer->gearStats[attackingPlayer->level].maxAir * 20) / 100;
    } else if (attackingPlayer->extremeGear == ExtremeGear::AdvantageF && attackingPlayer->specialFlags.hasAny(SpecialFlags::ringGear)) {
        attackingPlayer->currentAir += (attackingPlayer->gearStats[attackingPlayer->level].maxAir * 5) / 100;
    }
    else if (!attackingPlayer->specialFlags.hasAny(SpecialFlags::ringGear)) {
        if (attackingPlayer->extremeGear == ExtremeGear::CoverP) {attackingPlayer->rings += 30;}
        else if (attackingPlayer->extremeGear == ExtremeGear::Gambler) {attackingPlayer->rings += 5;}
        else { attackingPlayer->rings += 15; }
    } else if (attackingPlayer->character == Character::SuperSonic) {
		// EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*attackingPlayer);
        // if (attackingPlayer->gearExload().exLoadID == EXLoad::DarkSonic) {
        //     BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[attackingPlayer->index];
        //     bgInfo->currentGauge += 25000;
        //     SpecialFlagInfo *spfInfo = &PlayerSpecialFlagInfo[attackingPlayer->index];
        //     if (spfInfo->totalBoostTime <= 30 && spfInfo->totalBoostTime > 0) {
		//         PlayAudioFromDAT(Sound::SFX::MagnetShield);    
        //         attackingPlayer->speed += (spfInfo->gearChange >= 1 && attackingPlayer->level >= 1) ? pSpeed(45.0f) : pSpeed(25.0f);
        //         attackingPlayer->boostDuration += (spfInfo->gearChange >= 1 && attackingPlayer->level >= 1) ? 60 : 30;
		// 		attackingPlayer->currentAir += (attackingPlayer->gearStats[attackingPlayer->level].maxAir * 5) / 100;
        //         spfInfo->totalBoostTime = 61; // spoof to cancel critical hit after this activates once.
        //     }
        // }
    } else {
        attackingPlayer->currentAir += (attackingPlayer->gearStats[attackingPlayer->level].maxAir * 15) / 100;
    }
}