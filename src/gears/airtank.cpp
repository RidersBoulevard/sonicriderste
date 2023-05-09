#include "airtank.hpp"

struct AirTankInfo PlayerAirTankInfo[8];

void Player_AirTank(struct Player *player) {
    if (player->extremeGear != AirTank) return;

    struct AirTankInfo *ATInfo = &PlayerAirTankInfo[player->index];

    if (player->state == StartLine)
    {
        ATInfo->stoSize = 0;
        u32 itemStorage[] = {-1, -1, -1};
        ATInfo->itemUsed = 0;
    }

    if (player->input->toggleFaceButtons & XButton && player->state != StartLine)
    {
        for (int i = 0; i < 3; i++)
        {
            if (ATInfo->itemStorage[i] == -1) {
                break;
            } else { // if (itemStorage[i] == 0)
                ATInfo->itemUsed = 1;
                AntiItemCampHandler(player, ATInfo->itemStorage[i]);
                // player->current_itemID = ATInfo->itemStorage[i];
                // player->last_itemID = ATInfo->itemStorage[i];
                ATInfo->itemStorage[i] = -1;
            } 
        }
    }
}