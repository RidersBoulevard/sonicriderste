#include "flyhoopdash.hpp"
#include "lib/sound.hpp"
#include "cosmetics/player/exloads.hpp"

PlayerFlyHoopDashProperties playerFlyHoopDashProperties[8];

constexpr s32 FlyHoopDashInitialCosts[3] = {
		// 40000, 45000, 50000
		30000, 35000, 40000
};

constexpr f32 FlyHoopDashAirLoss[3] = {
		-333.3f, -416.6f, -500.0f
};

constexpr s32 FlyHoopDashAirLossRingGear = 30;

ASMUsed void Player_SetFlyHoopProperties(Player *player){
	PlayerFlyHoopDashProperties *properties = &playerFlyHoopDashProperties[player->index];

	properties->canUse = TRUE;
	properties->duration = 0;
}

void Player_FlyHoopDashHandler(Player *player){
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
	PlayerFlyHoopDashProperties *properties = &playerFlyHoopDashProperties[player->index];

	if (player->state == StartLine || player->state != Fly) {
		properties->duration = 0;
		properties->gainDelay = 0.0f;
		properties->hasUsed = FALSE;
	}

	// Everything below is the old code.

	// if(player->state == Fly && properties->canUse){
	// 	if(player->flyHoopDash != 2){
	// 		if(player->input->toggleFaceButtons & (BButton | XButton | LButton)){
	// 			// initial cost
	// 			player->flyHoopDash = TRUE;
	// 			PlayAudioFromDAT(Sound::SFX::RailHoopDash); // dash sfx
	// 			Player_InitBoostParticles(player);
	// 			if(player->specialFlags & ringGear){
	// 				if(player->character == SuperSonic
	// 				   && player->shortcutAirGainMultiplier > 0
	// 				   && exLoads.gearExLoadID != HyperSonicEXLoad){
	// 					player->shortcutAirGainMultiplier -= FlyHoopDashInitialCosts[player->level];
	// 					if(player->shortcutAirGainMultiplier < 0) player->shortcutAirGainMultiplier = 0;
	// 				}else{
	// 					s32 newAir = player->currentAir - (player->gearStats[player->level].maxAir * 5) / 100;
	// 					player->currentAir = clamp(newAir, 0);
	// 				}
	// 			}else{
	// 				player->changeInAir_gain -= FlyHoopDashInitialCosts[player->level];
	// 			}
	// 		}else if(player->currentAir > 0){
	// 			if(player->input->holdFaceButtons & (BButton | XButton | LButton) && player->flyHoopDash == 1){
	// 				player->speed += pSpeed(2);
	// 				f32 newMI;
	// 				if(player->character == SuperSonic
	// 				   && player->shortcutAirGainMultiplier > 0
	// 				   && exLoads.gearExLoadID != HyperSonicEXLoad){
	// 					newMI = player->magneticImpulse_timer; // essentially does nothing with blast gauge
	// 				}else{
	// 					newMI = player->magneticImpulse_timer - 3.0f;
	// 				}
	// 				player->magneticImpulse_timer = clamp(newMI, 0.f);

	// 				properties->duration += 1;

	// 				if(properties->duration >= 120){
	// 					func_SetPlayerActionType(player, Fall);
	// 					properties->canUse = FALSE;
	// 					properties->duration = 0;
	// 					player->flyHoopDash = 2;
	// 				}
	// 			}else if(player->flyHoopDash == 1){
	// 				properties->canUse = FALSE;
	// 				properties->duration = 0;
	// 				player->flyHoopDash = 2;
	// 			}

	// 		}else if(player->flyHoopDash == 1){
	// 			func_SetPlayerActionType(player, Fall);
	// 			properties->canUse = FALSE;
	// 			properties->duration = 0;
	// 			player->flyHoopDash = 2;
	// 		}
	// 	}

	// }else if(player->state == Fly && player->flyHoopDash == 2){
	// 	player->speed -= pSpeed(0.5);
	// }else{
	// 	player->flyHoopDash = FALSE;
	// 	properties->canUse = FALSE;
	// 	properties->duration = 0;
	// }
}

ASMUsed void Player_FlyHoopDashAttack(Player *attackingPlayer, Player *attackedPlayer){
	if(!(attackedPlayer->specialFlags & ringGear) &&
	   ((attackedPlayer->state == Fly) & (attackingPlayer->state == Fly) & (attackingPlayer->flyHoopDash))){
		Player_RingLossVisualsSFX(attackedPlayer);
		RingLoss_OnAttack(attackedPlayer);
		attackedPlayer->changeInAir_gain -= 30000;
	}
}

ASMUsed void ClearData_HoopDash(){
	TRK_memset(&playerFlyHoopDashProperties, 0, sizeof(PlayerFlyHoopDashProperties));
}