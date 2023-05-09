#include "context.hpp"

ASMUsed void Player_TypeAmount(Player *player) {
    u8 speed = (player->typeAttributes & SpeedType);
    u8 fly = (player->typeAttributes & FlyType);
    u8 power = (player->typeAttributes & PowerType);
    player->typeAmount = speed + fly + power;
}