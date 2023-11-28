#include "riders/player.hpp"

ASMDefined void PlayerCtrl_JumpAndTrick(Player *);

ASMDefined void PlayerAttackSearch(Player *);

ASMDefined void PlayerCtrl_AirAction(Player *);

ASMDefined void PlayerCtrl_Rotate(Player *, f32);

ASMDefined f32 lbl_001C57A0[]; // rodata

ASMUsed void Player_EnableJumpingInRun(Player *player) {
	PlayerCtrl_JumpAndTrick(player);
	if (player->extremeGear == ExtremeGear::AdvantageS) {
		PlayerAttackSearch(player);
		PlayerCtrl_AirAction(player);
		PlayerCtrl_Rotate(player, lbl_001C57A0[22]);
	}
}
