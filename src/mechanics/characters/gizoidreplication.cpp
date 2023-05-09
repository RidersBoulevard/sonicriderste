#include "gizoidreplication.hpp"
#include "lib/sound.hpp"
#include "cosmetics/player/exloads.hpp"

GizoidReplicationInfo PlayerGizoidReplication[8];

constexpr f32 BaseGizoidReplicationTimers[3] = {
		toFrames(5), toFrames(10), toFrames(15)
};

constexpr f32 ExtraGizoidReplicationTimers[4] = {
		// none
		0.0f,

		// bumping into player
		toFrames(10),

		// attacking player
		toFrames(5),

		// turb and slipstream
		toFrames(1),
};

constexpr f32 GR_TypeShortcutSpeedMultiplier = 1.2f;
constexpr f32 GR_TypeShortcutSpeedMultiplierDual = 1.1f;

constexpr f32 GR_SlipstreamDelay = toFrames(1);
constexpr f32 GR_TurbulenceDelay = toFrames(2);

void Player_EnableGizoidReplication(Player *player, Player *grPlayer, u32 extraGRState){
	GizoidReplicationInfo &grInfo = PlayerGizoidReplication[player->index];

	if(player->character != Emerl) return;
	//if (grInfo->isEnabled) return;

	PlayAudioFromDAT(Sound::SFX::EmerlCopyType);

	grInfo.isEnabled = TRUE;
	grInfo.timer += BaseGizoidReplicationTimers[player->level] + ExtraGizoidReplicationTimers[extraGRState];
	player->typeAttributes = grInfo.currentType = grPlayer->typeAttributes;
}

void Player_GRHandler(Player *player){
	if(player->character != Emerl) return;

	if((InGamePlayerCount < 2) && player->state == StartLine){
		if(player->input->toggleFaceButtons
		   & ZButton){ // Press Z button before starting the race to rotate types in single player
			PlayAudioFromDAT(Sound::SFX::EmerlCopyType);
			u32 newType = player->typeAttributes << 1; // rotate to next type
			if(newType > PowerType) newType = SpeedType; // reset to speed type
			player->typeAttributes = newType;
		}
		return;
	}

	GizoidReplicationInfo &grInfo = PlayerGizoidReplication[player->index];
	if(grInfo.timer > 0.0f){
		if((player->state == RailGrind || player->state == Fly ||
		((player->previousState == RailGrind || player->previousState == Fly))) && 
		grInfo.timer <= toFrames(5)) {
			grInfo.timer = toFrames(5);
		} else {
			if (player->state == QTE || player->state == QTE2 || 
			(player->state >= FrontflipRamp && player->state <= TurbulenceTrick2) ||
			player->state == Fall || player->state == StartLine) { 
				grInfo.timer += 0.0f;
				// Emerl's GR timer is not depleted during QTEs, tricks state, falling state, and using pits.
			} else grInfo.timer -= 1.0f;
		}
		grInfo.timer = clamp(grInfo.timer, 0.0f);

	}else{
		if(grInfo.isEnabled){
			grInfo.isEnabled = FALSE;
			grInfo.currentType = 0;
			player->typeAttributes = 0;
			PlayAudioFromDAT(Sound::SFX::EmerlLoseType);
		}
	}

	if(!grInfo.isEnabled){
		Player *grPlayer;
		if(player->state == TurbulenceRide){
			if(grInfo.enableDelayTimer < GR_TurbulenceDelay){
				grInfo.enableDelayTimer += 1.0f;
			}else{
				grInfo.enableDelayTimer = 0.0f;
				grPlayer = &players[player->closestTurbulenceIndex];
				Player_EnableGizoidReplication(player, grPlayer, GR_TurbSlipstreamState);
			}
		}else if(player->slipstream){
			if(grInfo.enableDelayTimer < GR_SlipstreamDelay){
				grInfo.enableDelayTimer += 1.0f;
			}else{
				if(grInfo.slipstreamPlayer){
					grInfo.enableDelayTimer = 0.0f;
					grPlayer = grInfo.slipstreamPlayer;
					Player_EnableGizoidReplication(player, grPlayer, GR_TurbSlipstreamState);
				}
			}
		}else{
			grInfo.enableDelayTimer = 0.0f;
			if(!player->slipstream) grInfo.slipstreamPlayer = nullptr;
		}
	}
}

ASMUsed void Player_GRPlayerBump(Player *player, Player *grPlayer){
	Player_EnableGizoidReplication(player, grPlayer, GR_BumpingIntoPlayerState);
}

ASMUsed void Player_GRAttacking(Player *player, Player *grPlayer){
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
	// struct HHOInfo *hhoInfo = &PlayerHHOInfo[player->index];
	Player_EnableGizoidReplication(player, grPlayer, GR_AttackingPlayerState);
	// if (exLoads.gearExLoadID == HyperHangOnEXLoad)
	// {
	//     if (hhoInfo->saturnMegadriveStatus != 2)
	//     {
	//         hhoInfo->attackCount += 1;
	//         PlayAudioFromDAT(Sound::SFX::TornadoSlingshot);
	//     }
	// }
}

ASMUsed void ClearData_GizoidReplication(){
	TRK_memset(&PlayerGizoidReplication, 0, sizeof(PlayerGizoidReplication));
}