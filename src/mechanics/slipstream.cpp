#include "slipstream.hpp"
#include "characters/gizoidreplication.hpp"
#include "containers/vector3.hpp"
#include "riders/gamemode.hpp"
#include "riders/object.hpp"

/**
 * Enables slipstream particles for a given player.
 *
 * @param index The player index to enable particles for.
 */
void lbl_SlipstreamParticles(u8 index) {
	auto &[data] = playerWindParticles[index];
	data[0x10] = 0x3F800000;
	data[0x11] = 0x3F800000;
	data[0x12] = 0x2;

	ObjectNode *selectedObj = nullptr;
	for(auto &object : getObjectList()) {
		if(object.object_group == DashPadParticle) {
			selectedObj = &object;
			break;
		}
	}
	if(selectedObj == nullptr) { return; }

	for(; index > 0; index--) {
		selectedObj = selectedObj->next;
	}

	if(selectedObj->object_group != DashPadParticle || selectedObj->state == 0x5 || selectedObj->state == 0x6) {
		return;
	}
	selectedObj->state = 0x4;
}

/**
 * Handles all of slipstream's mechanics for a given player.
 *
 * @param player The player to apply slipstream mechanics to.
 */
void lbl_Slipstream(Player &player) { // NOLINT(readability-function-cognitive-complexity)
	const auto gamemode = CurrentGameMode;
	if(gamemode == FreeRace || gamemode == WorldGrandPrix || gamemode == StoryMode) {
		player.slipstream = false;
		if(player.placement == 0 || player.state != PlayerState::Cruise || player.statusEffectFlags.hasAny(PlayerStatus::BallAndChainStatus)) { return; }

		for(auto &player2: players) {
			if(player.index == player2.index) { continue; }
			if(player.stageProgress > player2.stageProgress) { continue; }
			if(player.otherPlayerDistance[player2.index] > 60000.0F) { continue; }

			Vector3F rotation = {
			        player2.verticalRotation,
			        player2.horizontalRotation,
			        player2.rotationRoll,
			};

			const Vector3F player2Forward = rotation.getForwardVectorForRidersRotation();

			const Vector3F delta = {
			        player.x - player2.x,
			        player.y - player2.y,
			        player.z - player2.z,
			};

			const Vector3F directionToSecond = delta.normalized();

			const f32 angle = player2Forward.calculateAngle(directionToSecond);

			rotation = {
			        player.verticalRotation,
			        player.horizontalRotation,
			        player.rotationRoll,
			};

			const Vector3F playerForward = rotation.getForwardVectorForRidersRotation();

			const f32 alignment = playerForward.calculateAngle(player2Forward);

			if(angle > 0.3F || alignment > 0.35F) { continue; } // NOLINT(readability-magic-numbers)
			player.slipstream = true;

			if(player.character == Character::Emerl) {
				GizoidReplicationInfo &grInfo = PlayerGizoidReplication[player.index];
				if(grInfo.slipstreamPlayer != nullptr) {
					if(player2.placement < grInfo.slipstreamPlayer->placement) {
						grInfo.slipstreamPlayer = &player2;
					}
				} else {
					grInfo.slipstreamPlayer = &player2;
				}
			}

			lbl_SlipstreamParticles(player.index);

			if(player.speed == 0.0F) { continue; }// if player's speed is 0
			if(!player.movementFlags.hasAny(MovementFlags::drifting) && player.movementFlags.hasAny(MovementFlags::braking)) { continue; }
			// if (player.speed > data[4]) // 200 speed
			vf32 speedGainAbove200 = 0.00262963F;   // Todo: Are these two supposed to be set by something?
			vf32 speedGainUnder200 = 0.001314815F;
			if(player.speed > player.gearStats[player.level].topSpeed) {
				if(speedGainAbove200 <= 0.0F) { continue; } // zero // Always false
				//if (player.characterArchetype == Windcatcher) {
				//	player.speed += speedGainAbove200 * 1.1F;
				//} else {
					player.speed += speedGainAbove200;
				//}
				//compound assignment with 'volatile'-qualified left operand is deprecated
				speedGainAbove200 = speedGainAbove200 - 0.0006574075F;
			} else {
				if(speedGainUnder200 <= 0.0F) { continue; } // zero // Always false
				//if (player.characterArchetype == Windcatcher) {
				//	player.speed += speedGainUnder200 * 1.1F;
				//} else {
					player.speed += speedGainUnder200;
				//}
				speedGainUnder200 = speedGainUnder200 - 0.0006574075F;
			}
		}
	}
}