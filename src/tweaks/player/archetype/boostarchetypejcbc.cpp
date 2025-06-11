#include "boostarchetypejcbc.hpp"

std::array<DriftInfo, MaxPlayerCount> PlayerDriftInfo;
std::array<NewCombatInfo, MaxPlayerCount> PlayerCombatInfo;

ASMUsed void Player_BoostArchetypeJCBC(Player &player) {
	if(player.characterArchetype != CharacterArchetype::Boost) { return; }
	if(player.state != PlayerState::Cruise && !(player.extremeGear == ExtremeGear::AdvantageS && player.state == PlayerState::Run)) { return; }
	DriftInfo &driftInfo = PlayerDriftInfo[player.index];
	// player.movementFlags.hasAny(MovementFlags::drifting)
	if(player.input->holdFaceButtons.hasAny(Buttons::L, Buttons::R)
		|| player.input->toggleFaceButtons.hasAny(Buttons::L, Buttons::R)) {
		if(!driftInfo.hasHeldDrift &&
		   player.input->toggleFaceButtons.hasAny(Buttons::B, Buttons::X) &&
		   player.input->holdFaceButtons.hasAny(Buttons::A) &&
		   player.jumpCharge > 0 &&
		   player.currentAir > player.gearStats[player.level].boostCost) {
			lbl_Player_BoostEndFunction(player);
			driftInfo.hasHeldDrift = true;
		}
	} else {
		driftInfo.hasHeldDrift = false;
	}
}