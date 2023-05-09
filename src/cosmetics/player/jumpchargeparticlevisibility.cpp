#include "context.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct JumpChargeObject {
	void *update;
	Object *back;
	Object *next;
	u16 flags;
	u16 object_group;
	Object1 *object;
	u8 state;
	u8 unused;
	u8 unused2;
	u8 player_index;
	u8 culling_group;
	u16 item_id;
	u16 object_id;
	u16 index;
};
#pragma GCC diagnostic pop

global JumpChargeObject *gpsCurrentTask;
global void Player_JumpChargeParticlesTask();

ASMUsed void Player_JumpChargeParticlesVisibility() {
	auto &obj = *gpsCurrentTask;
	const Player &player = players[obj.player_index];
	if (player.extremeGear != AdvantageS && player.state == Run) {
		return;
	}
	Player_JumpChargeParticlesTask();
}
