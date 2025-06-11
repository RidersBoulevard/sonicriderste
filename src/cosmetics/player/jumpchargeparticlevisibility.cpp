#include "riders/object.hpp"
#include "riders/player.hpp"
#include "tweaks/stage/stage_changes.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#pragma GCC diagnostic pop

struct JumpChargeObject {
	Task update;
	ObjectNode *back;
	ObjectNode *next;
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

ASMDefined JumpChargeObject *gpsCurrentTask;

ASMDefined void Player_JumpChargeParticlesTask();

ASMUsed void Player_JumpChargeParticlesVisibility() {
	auto &obj = *gpsCurrentTask;
	const Player &player = players[obj.player_index];
	if (player.extremeGear != ExtremeGear::AdvantageS && player.state == PlayerState::Run) {
		return;
	}
	Player_JumpChargeParticlesTask();
}
