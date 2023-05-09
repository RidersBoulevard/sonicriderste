#include "olliekinggear.hpp"
#include "cosmetics/player/exloads.hpp"

const f32 OllieKingGear_BoostSpeeds[7] = {pSpeed(200.0f), pSpeed(210.0f), pSpeed(220.0f), pSpeed(225.0f), pSpeed(230.0f), pSpeed(235.0f), pSpeed(240.0f)};

const f32 OllieKingGear_KickdashSpeeds[3] = {pSpeed(50.0f), pSpeed(65.0f), pSpeed(80.0f)};
const f32 OllieKingGear_KickdashCap[3] = {pSpeed(310.0f), pSpeed(320.0f), pSpeed(330.0f)};

struct OKGInfo PlayerOKGInfo[8];

void Player_OllieKingGear(struct Player *player) {
    u8 controllerPort = player->input->port;
    struct EnabledEXLoads exLoads; 
    FetchEnabledEXLoadIDs(player, &exLoads);
    struct OKGInfo *okgInfo = &PlayerOKGInfo[player->index];
    if (exLoads.gearExLoadID != OllieKingGearEXLoad) return;

    if (player->state == StartLine) 
    {
        okgInfo->OllieBurstStatus = 0;
        // okgInfo->flyStateEntry = FALSE;
    }
    

    player->specialFlags |= noSpeedLossChargingJump;
    player->specialFlags |= lowerDecel;
    player->specialFlags |= tornadoBoost;

    // if (player->state == Fly && player->previousState != Jump && okgInfo->flyStateEntry == FALSE)
    // {
    //     okgInfo->currentSpeed = player->speed;
    //     okgInfo->flyStateEntry = TRUE;
    // } else if (player->state == Cruise) okgInfo->flyStateEntry = FALSE;

    if (player->movementFlags & boosting) okgInfo->currentSpeed = player->speed;

    switch (okgInfo->OllieBurstStatus)
        {
            case 0:
                player->specialFlags |= noBoostChain;
                break;
            case 1:
                player->specialFlags &= ~noBoostChain;
                break;
        }


    if (player->input->toggleFaceButtons & (XButton | BButton) && !(player->movementFlags & boosting)) 
    {
        switch (okgInfo->OllieBurstStatus)
        {
            case 0:
            f32 kickdashSpeedCalc;
                kickdashSpeedCalc = player->speed + OllieKingGear_KickdashSpeeds[player->level];
                if (kickdashSpeedCalc > OllieKingGear_KickdashCap[player->level] && !(player->speed > OllieKingGear_KickdashCap[player->level])) kickdashSpeedCalc = OllieKingGear_KickdashCap[player->level];
                player->gearStats[player->level].boostSpeed = kickdashSpeedCalc;
                break;
            case 1:
                player->gearStats[player->level].boostSpeed = OllieKingGear_BoostSpeeds[player->trickRank];
                break;
        }
        
    }
    
}