#include "lcanceling.hpp"
#include "riders/object.hpp"

std::array<PlayerLCancelInfo, MaxPlayerCount> playerLCancelInfo;

void Player_LCancelHandler(Player *player) {
    PlayerLCancelInfo &info = playerLCancelInfo[player->index];

    // deduct timers
    if (info.frameWindow > 0) { info.frameWindow -= 1; }
    if (info.lockoutTimer > 0) { info.lockoutTimer -= 1; }

	if(player->state < PlayerState::FrontflipRamp || player->state > PlayerState::TurbulenceTrick) { return; }
	if(!player->input->toggleFaceButtons.hasAny(Buttons::L, Buttons::R) || info.lockoutTimer != 0) { return; }
	if (player->characterArchetype == CharacterArchetype::Trickster) {
		info.frameWindow = 7;
	} else {
		info.frameWindow = 5;
	}

	info.lockoutTimer = 30;
}

ASMUsed u32 Player_CheckLCancel(Player *player, u32 currentTrickStatus) {
	const PlayerLCancelInfo &info = playerLCancelInfo[player->index];
    OffroadParticleObject1 *object;
    s32 newAir;

    if (info.frameWindow > 0) {
        if (currentTrickStatus == 2 && !player->specialFlags.hasAny(SpecialFlags::ringGear)) {
            player->changeInAir_loss += (player->gearStats[player->level].maxAir / 20); // removes 5 percent of air for a successful L-cancel, don't remove air otherwise
        }
            object = static_cast<OffroadParticleObject1 *>(SetTask(&OffroadJumpParticleTask, ObjectGroups::OffroadJumpParticle, 2)->object);
            object->playerIndex = player->index;
            currentTrickStatus = 0; // make sure not to fail the player's trick
    }
    return currentTrickStatus;
}