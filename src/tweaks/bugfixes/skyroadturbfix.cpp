#include "skyroadturbfix.hpp"
// unused
void Player_SkyRoadTurbFix(Player *player)
{
    u32 turbulenceBool, playerCount;
    playerCount = InGamePlayerCount;
    if (CurrentStage == SkyRoad)
    {
        turbulenceBool = (player->state == TurbulenceRide) ? TRUE : FALSE;
        if (turbulenceBool)
        {
            // compare turbulence index to player count to figure out if player is riding on big turb
            if (player->closestTurbulenceIndex < playerCount) return;
            // player->speed = pSpeed(200.0f);
            if (player->unkFD0 >= pSpeed(200.0f)) player->unkFD0 = pSpeed(200.0f);
        }
    }
}