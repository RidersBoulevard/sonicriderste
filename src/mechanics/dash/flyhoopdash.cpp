#include "flyhoopdash.hpp"
#include "grindraildash.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"
#include "lib/stdlib.hpp"

std::array<PlayerFlyHoopDashProperties, MaxPlayerCount> playerFlyHoopDashProperties;

ASMUsed void Player_SetFlyHoopProperties(Player &player) {
	PlayerFlyHoopDashProperties &properties = playerFlyHoopDashProperties[player.index];

	properties.canUse = true;
	properties.duration = 0;
}

void Player_FlyHoopDashHandler(Player &player) {
	PlayerFlyHoopDashProperties &properties = playerFlyHoopDashProperties[player.index];

	// player.state == StartLine is redundant here, should this be `&&`?
	if(player.state == PlayerState::StartLine || player.state != PlayerState::Fly) {
		properties.duration = 0;
		properties.gainDelay = 0.0f;
		properties.hasUsed = false;
	}

	// Everything below is the old code.

	// if(player.state == Fly && properties.canUse){
	// 	if(player.flyHoopDash != 2){
	// 		if(player.input->toggleFaceButtons & (BButton | XButton | LButton)){
	// 			// initial cost
	// 			player.flyHoopDash = true;
	// 			PlayAudioFromDAT(Sound::SFX::RailHoopDash); // dash sfx
	// 			Player_InitBoostParticles(player);
	// 			if(player.specialFlags & ringGear){
	// 				if(player.character == SuperSonic
	// 				   && player.shortcutAirGainMultiplier > 0
	// 				   && exLoads.gearExLoadID != HyperSonic){
	// 					player.shortcutAirGainMultiplier -= FlyHoopDashInitialCosts[player.level];
	// 					if(player.shortcutAirGainMultiplier < 0) player.shortcutAirGainMultiplier = 0;
	// 				}else{
	// 					s32 newAir = player.currentAir - (player.gearStats[player.level].maxAir * 5) / 100;
	// 					player.currentAir = clamp(newAir, 0);
	// 				}
	// 			}else{
	// 				player.changeInAir_gain -= FlyHoopDashInitialCosts[player.level];
	// 			}
	// 		}else if(player.currentAir > 0){
	// 			if(player.input->holdFaceButtons & (BButton | XButton | LButton) && player.flyHoopDash == 1){
	// 				player.speed += pSpeed(2);
	// 				f32 newMI;
	// 				if(player.character == SuperSonic
	// 				   && player.shortcutAirGainMultiplier > 0
	// 				   && exLoads.gearExLoadID != HyperSonic){
	// 					newMI = player.magneticImpulse_timer; // essentially does nothing with blast gauge
	// 				}else{
	// 					newMI = player.magneticImpulse_timer - 3.0f;
	// 				}
	// 				player.magneticImpulse_timer = clamp(newMI, 0.f);

	// 				properties.duration += 1;

	// 				if(properties.duration >= 120){
	// 					func_SetPlayerActionType(player, Fall);
	// 					properties.canUse = false;
	// 					properties.duration = 0;
	// 					player.flyHoopDash = 2;
	// 				}
	// 			}else if(player.flyHoopDash == 1){
	// 				properties.canUse = false;
	// 				properties.duration = 0;
	// 				player.flyHoopDash = 2;
	// 			}

	// 		}else if(player.flyHoopDash == 1){
	// 			func_SetPlayerActionType(player, Fall);
	// 			properties.canUse = false;
	// 			properties.duration = 0;
	// 			player.flyHoopDash = 2;
	// 		}
	// 	}

	// }else if(player.state == Fly && player.flyHoopDash == 2){
	// 	player.speed -= pSpeed(0.5);
	// }else{
	// 	player.flyHoopDash = false;
	// 	properties.canUse = false;
	// 	properties.duration = 0;
	// }
}

ASMUsed void Player_FlyHoopDashAttack(Player &attackingPlayer, Player &attackedPlayer) {
	if(!attackedPlayer.specialFlags.hasAny(SpecialFlags::ringGear) && attackedPlayer.state == PlayerState::Fly && attackingPlayer.state == PlayerState::Fly && attackingPlayer.flyHoopDash != 0) {
		Player_RingLossVisualsSFX(attackedPlayer);
		RingLoss_OnAttack(attackedPlayer);
		attackedPlayer.changeInAir_gain -= 30000;
	}
}