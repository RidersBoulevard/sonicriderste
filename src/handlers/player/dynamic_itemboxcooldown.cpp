#include "riders/player.hpp"
#include "riders/stage.hpp"

ASMUsed void Player_ItemBoxCooldownHandler(Player *player) {
    u8 cooldown = 30;
    bool toSetTimer = TRUE;
    switch (CurrentStage) {
        case SegaIllusion:
            if (player->stage_subState == 2) {toSetTimer = FALSE;}
            break;
        case GreenCave:
            if (player->greenCave_subState == 2) {cooldown = 7;}
            break;
        case IceFactory:
            if (player->death_spawnPoint >= 0x22 && player->death_spawnPoint <= 0x2A) {toSetTimer = FALSE;}
            break;
		default:
			break;
    }

    if (toSetTimer) {
        player->itemBox_cooldown = cooldown;
    }
}