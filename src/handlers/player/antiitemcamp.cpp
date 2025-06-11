#include "cosmetics/player/exloads.hpp"
#include "gears/airtank.hpp"
#include "gears/hangon.hpp"
#include "lib/stdlib.hpp"
#include "riders/gamemode.hpp"
#include "riders/object.hpp"

ASMUsed u32 AntiItemCampHandler(Player *player, u32 item) {
    if (CurrentGameMode != TimeTrial) {
        if (player->state == PlayerState::StartLine) {
            item = SpeedShoes;
        } else {
            if ((player->extremeGear == ExtremeGear::SuperHangOn && player->gearSpecificFlags[SuperHangOn::Level4]) && player->last_itemBox_random) {
                item = SpeedShoes;
            }

        	const auto &exloadID = player->gearExload().exLoadID;

            if (exloadID == EXLoad::TheBeast && player->state == PlayerState::Run && (item == ThirtyAir || item == FiftyAir || item == HundredAir))
            {
               switch (lbl_RNG_Number(3)) {
                    case 1:
                        item = TenRings;
                        break;
                    case 2:
                        item = TwentyRings;
                        break;
                    case 3:
                        item = ThirtyRings;
                        break;
                    default:
                        item = SpeedShoes;
                        break; 
                }
            }
            
            // AirTankInfo *ATInfo = &PlayerAirTankInfo[player->index];
            // if (player->extremeGear == AirTank && ATInfo->itemUsed == 0) {
                
            //     // experimental, air tank item storage
            //     ATInfo->itemStorage[0] = item;
            //     // if (ATInfo->stoSize < 3)
            //     // {ATInfo->itemStorage[ATInfo->stoSize++] = item;} 
            //     return -1;
            // } else if (player->extremeGear == AirTank && ATInfo->itemUsed == 1) 
            // {
            //     ATInfo->itemUsed = 0;
            //     return item;
            // }

            // Fix for new speed shoe RNG, items will go OOB if the chance is zero so this covers that case
            if (item > FiveRings) {
                switch (lbl_RNG_Number(1)) {
                    case 0:
                        item = TenRings;
                        break;
                    case 1:
                        item = ThirtyAir;
                        break;
                    default:
                        item = FiveRings;
                        break; 
                }
            }
             

            if (player->last_itemID_lap != player->currentLap) {
                player->current_itemID = player->last_itemID;
            } else if (player->last_itemID == player->current_itemID) {
                item = SpeedShoes;
            } else {
                player->current_itemID = player->last_itemID;
            }
        }
    }

    player->last_itemID_lap = player->currentLap;
    return item;
}