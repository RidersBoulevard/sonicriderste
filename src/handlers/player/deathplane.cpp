#include "deathplane.hpp"
#include "handlers/player/SetPlayerState.hpp"

constexpr auto deathPlaneYLevel = -2000.0f;

void Player_DeathPlane(Player &player) {
	if(player.y >= deathPlaneYLevel) { return; }
	if(player.state == PlayerState::Death) { return; }
	func_SetPlayerActionType(player, static_cast<u32>(PlayerState::Death));
}