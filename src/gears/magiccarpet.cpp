#include "magiccarpet.hpp"

constexpr std::array<f32, 3> MagicCarpet_BoostSpeeds = {pSpeed(202.0F), pSpeed(232.0F), pSpeed(252.0F)};
constexpr std::array<f32, 3> MagicCarpet_BoostSpeeds2 = {pSpeed(200.0F), pSpeed(230.0F), pSpeed(250.0F)};

void Player_MagicCarpet(Player *player) {
	if(player->extremeGear != ExtremeGear::MagicCarpet) { return; }

	if(player->characterArchetype == CharacterArchetype::Boost) {
		if((std::to_underlying(player->state) & std::to_underlying(PlayerState::Jump)) != 0) { // attempts to fix lightboard effect for boost archetype, TODO: try a different solution, this works temporarily
			player->specialFlags &= ~SpecialFlags::lightBoardEffect;
		} else {
			player->specialFlags |= SpecialFlags::lightBoardEffect;
		}
		player->specialFlags &= ~SpecialFlags::berserkerEffect;
		if(player->movementFlags.hasAny(static_cast<MovementFlags>(0x0400))) { return; }
		player->gearStats[player->level].boostSpeed = MagicCarpet_BoostSpeeds[player->level];
		return;
	}

	if (player->characterArchetype == CharacterArchetype::LateBooster) {
        player->gearStats[player->level].boostSpeed = MagicCarpet_BoostSpeeds2[player->level];
    }

	player->specialFlags |= (SpecialFlags::legendEffect | SpecialFlags::alwaysOnIce | SpecialFlags::tornadoBoost | SpecialFlags::noSpeedLossChargingJump | SpecialFlags::noSpeedLossTurning | SpecialFlags::lightBoardEffect);
	player->specialFlags &= ~SpecialFlags::lowBoost;

	if(player->currentAir >= (player->gearStats[player->level].maxAir * 50) / 100) {
		player->specialFlags |= SpecialFlags::berserkerEffect;
	} else {
		if(player->specialFlags.hasAny(SpecialFlags::berserkerEffect) && !player->movementFlags.hasAny(MovementFlags::boosting) && player->state != PlayerState::AttackingPlayer) {
			player->attackProperties = nullptr;
		}
		player->specialFlags &= ~SpecialFlags::berserkerEffect;
	}
}