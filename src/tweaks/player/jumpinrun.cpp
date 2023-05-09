#include "context.hpp"

global void PlayerCtrl_JumpAndTrick(Player *);

global void PlayerAttackSearch(Player *);

global void PlayerCtrl_AirAction(Player *);

global void PlayerCtrl_Rotate(Player *, f32);

global f32 lbl_001C57A0[]; // rodata

ASMUsed void Player_EnableJumpingInRun(Player *player) {
	PlayerCtrl_JumpAndTrick(player);
	if (player->extremeGear == AdvantageS) {
		PlayerAttackSearch(player);
		PlayerCtrl_AirAction(player);
		PlayerCtrl_Rotate(player, lbl_001C57A0[22]);
	}
}
