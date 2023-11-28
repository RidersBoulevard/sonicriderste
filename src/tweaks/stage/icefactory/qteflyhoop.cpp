#include "riders/player.hpp"
#include "riders/stage.hpp"

ASMUsed f32 IceFactory_QTEFlyHoopVerticalSpeedCap(Player *player, f32 currentSpeed, f32 multiplier) {
    if (CurrentStage == IceFactory) {
        if (player->x > 268.272003f && player->x < 278.272003f &&
            player->z > -31.5f && player->z < -16.5f) {

            player->verticalSpeed = -1e-05f;
            multiplier = 0.95f;
        }
    }
    return currentSpeed * multiplier;
}