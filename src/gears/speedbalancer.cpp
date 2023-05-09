#include "speedbalancer.hpp"

USED void Player_SpeedBalancer(Player *player) {
	if (player->extremeGear != SpeedBalancer) return;
	if (player->movementFlags & boosting && player->flags & 0x20) {
		player->gearStats[player->level].boostSpeed += pSpeed(2.0f);
	}
}

USED void Player_SpeedBalancerResetBoostSpeed(Player *player) {
	if (player->extremeGear != SpeedBalancer) return;
	if (!(player->movementFlags & boosting) && !(player->flags & 0x20)) {
		player->gearStats[player->level].boostSpeed = player->gearptr->levelStats[player->level].boostSpeed;
	}
}