#include "boostarchetypejcbc.hpp"

//global void lbl_Player_BoostEndFunction(struct Player*);

// DriftInfo PlayerDriftInfo[8];

ASMUsed void Player_BoostArchetypeJCBC(Player *player) {
    if(player->characterArchetype != BoostArchetype) return;
    if (player->input->holdFaceButtons & AButton && player->movementFlags & drifting && player->input->toggleFaceButtons & (BButton | XButton)){
        lbl_Player_BoostEndFunction(player);
    }
	//Controller *readInput = player->input;
	//DriftInfo *driftInfo = &PlayerDriftInfo[player->index];
    // if ((player->last_movementFlags & drifting) && player->input->holdFaceButtons & AButton && player->input->toggleFaceButtons & (XButton | BButton)) // && player->input->toggleFaceButtons & (BButton | XButton)
    // {
    //     driftInfo->lastDirectionDrifted = player->unkB4C;
    //     lbl_Player_BoostEndFunction(player);
    //     switch (player->unkB4C)
    //     {
    //         case 1:
    //             if (driftInfo->lastDirectionDrifted != player->unkB4C) 
    //             {
    //                 lbl_Player_BoostEndFunction(player);
    //                 driftInfo->lastDirectionDrifted = player->unkB4C;
    //             }
    //         break;

    //         case -1:
    //             if (driftInfo->lastDirectionDrifted != player->unkB4C) 
    //             {
    //                 lbl_Player_BoostEndFunction(player);
    //                 driftInfo->lastDirectionDrifted = player->unkB4C;
    //             }
    //         break;

    //         case 0:
    //         default:
    //             driftInfo->lastDirectionDrifted = 0;
    //         break;
    //     }
    // }
    // if ((player->last_movementFlags & drifting))
    // {
    //     if (player->input->holdFaceButtons & (XButton | BButton)) return;
    //     if (readInput->leftstick.leftStickHorizontal == 0 && player->state == Cruise && player->input->holdFaceButtons & AButton) return; // player->input->holdFaceButtons & (AButton) && 
    //     lbl_Player_BoostEndFunction(player);
    // }
}