#include "context.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/stdlib.hpp"

constexpr s16 BoostControl_PowerGearAir[3] = {0xA5, 0x14A, 0x1F4}; // for each level

constexpr s16 BoostControl_AdvantageSAir[3] = {0xF5, 0x1AA, 0x1F4}; // for each level

constexpr s16 BoostControl_FasterAir[3] = {0x67, 0xFA, 0x1F4}; // for each level

constexpr s16 BoostControl_HHOAir[3] = {0x47, 0xDA, 0x0FA}; // for each level

ASMUsed void Player_BoostControl(Player *player) {
	s32 newAir;
	u32 isBoostControlActive = FALSE;
	s32 newBlastGaugeValue;

	u8 controllerPort = player->input->port;
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
	// struct HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];

	if (player->boostDuration == 1) {
		// see if player is initiating boost control
		if (player->input->holdFaceButtons & (BButton | XButton)) {
			if (isSuperCharacter(*player, Shadow)) {
				isBoostControlActive = TRUE;
				newAir = player->currentAir - 183; // subtract roughly 10 rings for each second

				player->currentAir = clamp(newAir, 0);

			} else {
				switch (player->extremeGear) {
					case PowerGear:
						isBoostControlActive = TRUE;
						newAir = player->currentAir - (BoostControl_PowerGearAir[player->level] * 95) / 100;

						player->currentAir = clamp(newAir, 0);
						break;

					case AdvantageS:
						isBoostControlActive = TRUE;
						newAir = player->currentAir - (BoostControl_AdvantageSAir[player->level] * 80) / 100;

						newAir = clamp(newAir, 0);
						if (newAir == 0) {
							isBoostControlActive = FALSE;
						}

                        player->currentAir = newAir;
                        break;

                    case Faster:
                        if (player->characterArchetype == BoostArchetype || player->characterArchetype == TricksterArchetype) break;
                        isBoostControlActive = TRUE;
                        newAir = player->currentAir - BoostControl_AdvantageSAir[player->level];

                        if (newAir < 0) {
                            newAir = 0;
                            isBoostControlActive = FALSE;
                        }

                        player->currentAir = newAir;
                        break;

                    // case SuperHangOn:
                    //     // if (exLoads.gearExLoadID == E99EXLoad)
                    //     // {
                    //     //     if (player->level4 == 1)
                    //     //     {
                    //     //         isBoostControlActive = TRUE;
                    //     //         newAir = player->currentAir - BoostControl_FasterAir[player->level];
                    //     //         if (newAir < 0) newAir = 0;
                    //     //         player->currentAir = newAir;
                    //     //         break;
                    //     //     }
                    //     //     break;
                    //     // }


                    //     if (exLoads.gearExLoadID == HyperHangOnEXLoad)
                    //     {
                    //         if (hhoInfo->saturnMegadriveStatus != 2)
                    //         {
                    //             isBoostControlActive = TRUE;
                    //             newAir = player->currentAir - BoostControl_HHOAir[player->level];
                    //             if (newAir < 0) {
                    //                 newAir = 0;
                    //                 isBoostControlActive = FALSE;
                    //             }

                    //             player->currentAir = newAir;

                    //             if (player->characterArchetype != BoostArchetype){
                    //                 player->gearStats[player->level].boostSpeed = HHOBoostControlSpeeds[player->level];
                    //             } else player->gearStats[player->level].boostSpeed = HHOBoostControlSpeedsBoostArchetype[player->level];

                    //             if (player->characterArchetype != BoostArchetype) player->gearStats[player->level].baseAccel = 100.0f;

                    //             break;
                    //         } else if (hhoInfo->saturnMegadriveStatus == 2)
                    //         {
                    //             isBoostControlActive = TRUE;
                    //             if (AllCharacterArchetypes[player->character] != BoostArchetype) {
                    //                 newAir = player->currentAir - BoostControl_HHOAir[1];
                    //             } else newAir = player->currentAir - BoostControl_HHOAir[0];

                    //             if (newAir < 0) {
                    //                 newAir = 0;
                    //                 isBoostControlActive = FALSE;
                    //             }
                    //             player->currentAir = newAir;

                    //             if (player->characterArchetype != BoostArchetype){
                    //                 player->gearStats[player->level].boostSpeed = HHOBoostControlSpeeds[3];
                    //             } else player->gearStats[player->level].boostSpeed = HHOBoostControlSpeedsBoostArchetype[3];

                    //             break;
                    //         }
                    //     }
                    //     break;
                }
            }
        }
    }

    if (!isBoostControlActive) {
        player->boostDuration -= 1;
        // if (exLoads.gearExLoadID == HyperHangOnEXLoad && hhoInfo->saturnMegadriveStatus != 2)
        // {
        //     player->gearStats[player->level].baseAccel = player->gearptr->acceleration;
        //     player->gearStats[0].boostSpeed = player->gearptr->levelStats[0].boostSpeed;
        //     player->gearStats[1].boostSpeed = player->gearptr->levelStats[1].boostSpeed;
        //     player->gearStats[2].boostSpeed = player->gearptr->levelStats[2].boostSpeed;
        // }
    }
}