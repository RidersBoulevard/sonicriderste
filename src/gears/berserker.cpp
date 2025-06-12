#include "berserker.hpp"

#include <handlers/player/SetPlayerState.hpp>
#include <riders/stage.hpp>

#include "lib/stdlib.hpp"

std::array<u8, MaxPlayerCount> Player_BerserkerStatIncreaseMultipliers;

void Player_BerserkerStatGain(Player &attackedPlayer, Player &attackingPlayer) {
	if(attackedPlayer.extremeGear == ExtremeGear::Berserker && !attackedPlayer.playerType) {
		// berserker gets attacked, stat reset

		const f32 multiplier = Player_BerserkerStatIncreaseMultipliers[attackedPlayer.index];

		for(u32 i = 0; i < 3; i++) {
			attackedPlayer.gearStats[i].boostCost = attackedPlayer.gearptr->levelStats[i].boostCost;
			attackedPlayer.gearStats[i].boostSpeed = attackedPlayer.gearptr->levelStats[i].boostSpeed;

			attackedPlayer.gearStats[i].topSpeed -= multiplier * pSpeed(3.0f);// +3 top speed
		}

		Player_BerserkerStatIncreaseMultipliers[attackedPlayer.index] = 0;
	}

	if(attackingPlayer.extremeGear == ExtremeGear::Berserker && !attackingPlayer.playerType) {
		// berserker attacks, stat increase

		const u32 multiplier = Player_BerserkerStatIncreaseMultipliers[attackingPlayer.index] += 1;

		for(u32 i = 0; i < 3; i++) {
			attackingPlayer.gearStats[i].boostSpeed += pSpeed(1.0f);
			attackingPlayer.gearStats[i].topSpeed += pSpeed(3.0f);

			if(100 - multiplier * 5 > 0) {
				attackingPlayer.gearStats[i].boostCost =
						static_cast<s32>((static_cast<u32>(attackingPlayer.gearptr->levelStats[i].boostCost / 100)) * (100 - multiplier * 5));
			}
		}
	}
}

void Player_BerserkerStatResetTornado(Player &player) {
	if(player.extremeGear == ExtremeGear::Berserker && player.playerType == 0) {
		// berserker gets attacked, stat reset

		const f32 multiplier = Player_BerserkerStatIncreaseMultipliers[player.index];

		for(u32 i = 0; i < 3; i++) {
			player.gearStats[i].boostCost = player.gearptr->levelStats[i].boostCost;
			if(player.characterArchetype != CharacterArchetype::Boost) {
				player.gearStats[i].boostSpeed = player.gearptr->levelStats[i].boostSpeed;
			}

			player.gearStats[i].topSpeed -= multiplier * pSpeed(3.0f);// +3 top speed
		}

		if(player.characterArchetype == CharacterArchetype::Boost) {
			player.gearStats[0].boostSpeed = pSpeed(203.0f);
			player.gearStats[1].boostSpeed = pSpeed(234.0f);
			player.gearStats[2].boostSpeed = pSpeed(255.0f);
		}

		if(player.characterArchetype == CharacterArchetype::Boost) {
			player.gearStats[0].boostSpeed = pSpeed(203.0f);
			player.gearStats[1].boostSpeed = pSpeed(234.0f);
			player.gearStats[2].boostSpeed = pSpeed(255.0f);
		}

		Player_BerserkerStatIncreaseMultipliers[player.index] = 0;
	}
}

void Player_BerserkerSpeedCheck(Player *player) {
	if (player->extremeGear != ExtremeGear::Berserker) return;
	if (player->state == PlayerState::AttackedByPlayer) return;

	if (player->state != PlayerState::AttackingPlayer
		&& player->speed < pSpeed(120.0f)
		&& player->specialFlags.hasAny(SpecialFlags::berserkerEffect)
		&& !player->movementFlags.hasAny(MovementFlags::boosting))
		{
			player->attackProperties = nullptr;
			player->specialFlags &= ~SpecialFlags::berserkerEffect;
		}
		else if (player->state == PlayerState::Cruise
				&& player->speed > pSpeed(120.0f)
				&& !player->specialFlags.hasAny(SpecialFlags::berserkerEffect)
				&& !(player->input->holdFaceButtons.hasAny(Buttons::A) && player->jumpCharge > 0 && player->speed < pSpeed(120.0f))
				|| player->movementFlags.hasAny(MovementFlags::boosting)) {
			player->specialFlags |= SpecialFlags::berserkerEffect;
		}

}

// NOTE: MOVE TO ANOTHER FILE LATER
void Player_cancelAttackHack(Player &player) {
    if (player.state != PlayerState::AttackingPlayer) return;
    // if attacking, end the attack by setting the state to cruise to disconnect when in this area near the QTE
    switch (CurrentStage) {
        case SandRuins:
            if (player.currPathFindingPoint >= 46 && player.currPathFindingPoint <= 48) {
                func_SetPlayerActionType(player, static_cast<u32>(PlayerState::Cruise));
            }
        break;
        default: break;
    }
}