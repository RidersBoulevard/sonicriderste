#include "mechanics/magneticimpulse.hpp"

s32 Player_SignGrinding_MI(Player *player, s32 airGain) {
    if (MI::impulseData[player->index].magneticImpulse) {
        return static_cast<s32>((((player->magneticImpulse_timer / MI::MaximumCap) * 0.25f) + 1.0f) * static_cast<f32>(airGain));
    }
	return airGain;

}

ASMUsed void Player_AirGain_SignGrinding(Player *player) {
    s32 currentAir = player->currentAir;
    if (player->specialFlags.hasAny(ringGear)) {
        currentAir += Player_SignGrinding_MI(player, (player->gearStats[0].maxAir / 100) / 2);
    } else {
        currentAir += Player_SignGrinding_MI(player, 1750);
        if (player->getTypeCount() == 0) {
            // no type
            currentAir += Player_SignGrinding_MI(player, 350);
        } else if (player->characterptr->type == 0) {
            // natural speed type
            currentAir += Player_SignGrinding_MI(player, 250);
        }
    }

    player->currentAir = currentAir;
}