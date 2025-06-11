#include "autopilot.hpp"
#include "handlers/menu/debugmenu/debugmenu.hpp"
#include "riders/gamemode.hpp"

constexpr auto TRANSPARENCY_FLAG = 0x2000;

void Player_Autopilot(Player &player) {
    if (!DebugMenu_CheckOption(DebugMenuOptions::Autopilot)) { return; }
    if (player.playerType != 0) { return; }

    player.unkBAC &= ~TRANSPARENCY_FLAG;
    player.unkBAC |= TRANSPARENCY_FLAG * player.aiControl;

    // Hold Z+C-stick down, then press DPadDown to turn it on or off
    if (!player.input->toggleFaceButtons.hasAny(Buttons::DPadDown) || !player.input->holdFaceButtons.hasAny(Buttons::Z)
        || player.input->rightStickVertical >= -80) return;

    // if (!player.input->toggleFaceButtons.hasAny(Buttons::DPadDown)) { return; }

    if ((geGame_ModeDetail - CurrentGameMode) < 2) { return; }

    player.aiControl ^= 1;

}