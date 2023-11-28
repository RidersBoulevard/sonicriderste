#include "boostchainmultiplier.hpp"
#include "cosmetics/player/exloads.hpp"


ASMUsed void Player_BoostChainMultiplier(Player *player) {
	const f32 currentBoostSpeed = player->gearStats[player->level].boostSpeed;
    player->speedCap = currentBoostSpeed;

	if (player->specialFlags.hasAny(noBoostChain)) {
		player->speed = currentBoostSpeed;
		return;
	}
    
    // add 0.1% bcm per 1 boost speed above average boost speeds
	f32 additiveBcm = (currentBoostSpeed - BCM_GeneralBoostSpeeds[player->level]) * (0.001f / pSpeed(1));
    if (player->specialFlags.hasAny(ringGear)) { additiveBcm *= (1.0f/3.0f); }
	f32 bcm = lbl_001C7400;
    if (additiveBcm > 0) { bcm += additiveBcm; }

    switch (player->characterArchetype) {
        case CombatArchetype:
            bcm += 0.03f;
            break;
        case NoTypeArchetype:
            bcm += 0.04f;
            break;

        case AllRounder:
            bcm += 0.02f;
            break;
    }

	const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);
    switch (player->extremeGear) {
		using namespace ExtremeGear;
        case BlueStar2:
            bcm += 0.07f;
            break;

        // case DefaultGear:
        //     if (exLoads.gearExLoadID == WindmasterJetEXLoad) bcm += 0.10f;
        //     break;

        case HangOn:
            if (player->hangOn_infiniteRingTimer > 0) bcm += 0.11f;
        //     bcm += 0.11f;
            break;

        case AdvantageF:
            if (exLoads.gearExLoadID == TheProfessionalEXLoad){
                if (player->input->holdFaceButtons.hasAny(AButton) && player->state == Cruise) {
					bcm += 0.35f;
				} else {
					bcm += 0.25f;
				}
            } else {
				bcm += 0.05f;
			}
            break;

        // case TurboStar: // experimental
        // if (exLoads.gearExLoadID == OllieKingGearEXLoad) break;
        //         switch (player->level) {
        //             case 0:
        //             bcm += 0.07f;
        //             break;

        //             case 1:
        //             bcm += 0.05f;
        //             break;

        //             case 2:
        //             if (player->level4 == 0)
        //             {bcm += 0.03f;} else bcm += 0.0f;
        //             break;
        //         }
        //     break;
		default:
			break;
    }

    if (player->specialFlags.hasAny(lowBoost)) {
        bcm = 1.05f;
    }
	player->speed *= bcm;
}