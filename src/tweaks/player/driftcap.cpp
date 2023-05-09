#include "context.hpp"

constexpr f32 EmerlDriftCapBonus[3] = {
    pSpeed(30), pSpeed(15), pSpeed(5)
};

ASMUsed void* Player_DriftCap(Player *player, f32 return_array[2], f32 currentDriftCap, f32 f3) {
    f32 addCap;
    // f32 return_array[2];
    PlayerGearStats &gearstats = player->gearStats[player->level];
    
    currentDriftCap += player->level * pSpeed(10);

    if (player->character == Emerl) {
        currentDriftCap += EmerlDriftCapBonus[player->level];
    }

    if (player->extremeGear == SlideBooster) {
        currentDriftCap -= pSpeed(10);
    } else {
        switch (player->characterArchetype) {
            case DriftArchetype:
            case NoTypeArchetype:
                if (player->specialFlags & 0x00200000UL) addCap = pSpeed(10);
                else addCap = pSpeed(20);

                currentDriftCap += addCap;
                f3 += addCap;
                break;

            case AllRounder:
                if (player->specialFlags & 0x00200000UL) addCap = pSpeed(5);
                else addCap = pSpeed(10);

                currentDriftCap += addCap;
                f3 += addCap;
                break;
			default:
				break;
        }

        if (!(player->specialFlags & 0x00400000UL)) {
            switch (player->extremeGear) {
                case Beginner:
                    currentDriftCap -= pSpeed(50);
                    currentDriftCap += gearstats.driftDashSpeed;
                    currentDriftCap += pSpeed(10);
                    break;

                case TurboStar:
                    if (player->level4) currentDriftCap += pSpeed(10);
                    break;

                case CoverP:
                    currentDriftCap += pSpeed(20);
                    currentDriftCap += pSpeed(10);
                    break;
                
                case MagicCarpet:
                case AutoSlider:
                    currentDriftCap += pSpeed(10);
                    break;

                default:
                    if (player->specialFlags & ringGear) {
                        currentDriftCap += pSpeed(10);
                    }
                    break;
            }
        } else {
            currentDriftCap += pSpeed(10);
        }
    }

    return_array[0] = currentDriftCap;
    return_array[1] = f3;
    return return_array;
}