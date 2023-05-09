#include "autopilot.hpp"
#include "handlers/menu/debugmenu/debugmenu.hpp"

#define TRANSPARENCY_FLAG 0x2000

void Player_Autopilot(struct Player *player) {
    if (!DebugMenu_CheckOption(Autopilot)) return;
    if (player->playerType != 0) return;
    if (!(player->input->toggleFaceButtons & DPadDown)) return;

    player->aiControl ^= 1;
    player->unkBAC ^= TRANSPARENCY_FLAG;
}