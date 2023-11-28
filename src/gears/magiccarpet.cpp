#include "magiccarpet.hpp"

constexpr std::array<f32, 3> MagicCarpet_BoostSpeeds = {pSpeed(202.0F), pSpeed(232.0F), pSpeed(252.0F)};
constexpr std::array<f32, 3> MagicCarpet_BoostSpeeds2 = {pSpeed(200.0F), pSpeed(230.0F), pSpeed(250.0F)};

void Player_MagicCarpet(Player *player) {
	if(player->extremeGear != ExtremeGear::MagicCarpet) { return; }

	if(player->characterArchetype == BoostArchetype) {
		if((player->state & Jump) != 0) { // attempts to fix lightboard effect for boost archetype, TODO: try a different solution, this works temporarily
			player->specialFlags &= ~lightBoardEffect;
		} else {
			player->specialFlags |= lightBoardEffect;
		}
		player->specialFlags &= ~berserkerEffect;
		if(player->movementFlags.hasAny(static_cast<MovementFlags>(0x0400))) { return; }
		player->gearStats[player->level].boostSpeed = MagicCarpet_BoostSpeeds[player->level];
		return;
	}

	if (player->characterArchetype == LateBooster) {
        player->gearStats[player->level].boostSpeed = MagicCarpet_BoostSpeeds2[player->level];
    }

	player->specialFlags |= (legendEffect | alwaysOnIce | tornadoBoost | noSpeedLossChargingJump | noSpeedLossTurning | lightBoardEffect);
	player->specialFlags &= ~lowBoost;

	if(player->currentAir >= (player->gearStats[player->level].maxAir * 50) / 100) {
		player->specialFlags |= berserkerEffect;
	} else {
		if(((player->specialFlags & berserkerEffect) != 0u) && ((player->movementFlags & boosting) == 0u) && player->state != AttackingPlayer) {
			player->unkF40 = 0;
		}
		player->specialFlags &= ~berserkerEffect;
	}
}