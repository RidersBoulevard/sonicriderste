#include "coverp.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"
#include "cosmetics/player/exloads.hpp"

inline u32 Player_Coverp_ChangeWeight(Player *player, u32 weightState) {
    u32 weightCase;
    if (player->coverP_weightState == weightState) weightCase = 2; // same mode toggled, does nothing
    else if (player->rings < 15) weightCase = 0; // rings less than requirement does nothing
    else weightCase = 1; // different mode toggled

    return weightCase;
}

void Player_CreateCoverPParticles(Player *player) {
	auto *particles = (ParticleTaskObject1 *) SetTask(func_Particle_Task, 0xB3B0, 2)->object;
	particles->unk72 = player->index;
	particles->unk0 = 0.0f;
	particles->unk8 = 0.0f;
	particles->unk10 = 0.0f;
	particles->unk18 = 0.0f;
	particles->unk4 = 0.3f;
	particles->unk14 = 0.8f;
	particles->unk68 = gpsTexList_Particle;
	particles->unk60 = &lbl_001D8F58;
	particles->unk73 = -1;
	particles->unk6C = 0;
	particles->unk74 = 0;
	particles->unk48 = &player->x;
}

void Player_CoverP(Player *player) {
    f32 originalWeight;
    f32 calculatedWeight;
    f32 boostSpeed;
    f32 topSpeed;
    f32 acceleration;
    u8 weightState;
    EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
    
    if (player->extremeGear != CoverP) return;
    player->level = 1;
    // player->gearStats[player->level].topSpeed = player->gearStats[0].topSpeed;
    originalWeight = CharacterWeights[player->character] + 0.4f;
    // TODO: change below to when we fix exload unloading
    if (exLoads.characterExLoadID == RealaEXLoad || exLoads.characterExLoadID == GonGonEXLoad) originalWeight = EXLoadWeights[exLoads.characterExLoadID] + 0.4f;

    // player->gearStats[0].airDrain = 10;
    // player->gearStats[0].boostCost = 0x61A8 * 0.80f;
    // player->gearStats[0].driftCost = 0xA6;
    // player->gearStats[0].tornadoCost = 0x61A8* 0.80f;

    if (player->state == StartLine){
        player->currentAir = player->gearStats[player->level].maxAir;
        player->coverP_weightState = 3;
        return;
    }

    if (player->input->toggleFaceButtons & DPadLeft) {
        weightState = 1;
        switch (Player_Coverp_ChangeWeight(player, weightState)) {
            default:
                return;
            case 1:
                player->rings -= 15;
                calculatedWeight = originalWeight - (originalWeight * 0.75f);
                if (calculatedWeight < 0.1f) calculatedWeight = 0.1f;
                player->weight = calculatedWeight;
                // weightState = 1;
                break;
            case 2:
                return;
        }
        player->coverP_weightState = weightState;
        Player_CreateCoverPParticles(player);
        PlayAudioFromDAT(Sound::SFX::CovFModeSwitch);


    } else if (player->input->toggleFaceButtons & DPadUp) {
        weightState = 3;
        switch (Player_Coverp_ChangeWeight(player, weightState)) {
            default:
                return;
            case 1:
                player->rings -= 15;
                player->weight = originalWeight;
                // weightState = 3;
                break;
            case 2:
                return;
        }
        player->coverP_weightState = weightState;
        Player_CreateCoverPParticles(player);
        PlayAudioFromDAT(Sound::SFX::CovFModeSwitch);

    } else if (player->input->toggleFaceButtons & DPadRight) {
        weightState = 2;
        switch (Player_Coverp_ChangeWeight(player, weightState)) {
            default:
                return;
            case 1:
                player->rings -= 15;
                calculatedWeight = originalWeight + (originalWeight * 0.75f);
                if (calculatedWeight > 2.92500f) calculatedWeight = 2.92500f;
                player->weight = calculatedWeight;
                // weightState = 2;
                break;
            case 2:
                return;
        }
        player->coverP_weightState = weightState;
        Player_CreateCoverPParticles(player);
        PlayAudioFromDAT(Sound::SFX::CovFModeSwitch);
    }

    // topSpeed = pSpeed(150.0f) + (pSpeed(0.35f) * player->rings);
    // player->gearStats[player->level].topSpeed = topSpeed;
    acceleration = (f32(player->rings) / 5.0f);
    if (player->characterArchetype != BoostArchetype) player->gearStats[player->level].baseAccel = acceleration;
    else if (player->characterArchetype == BoostArchetype) {
        if (player->movementFlags & boosting) player->gearStats[player->level].baseAccel = player->gearptr->acceleration;
        else player->gearStats[player->level].baseAccel = acceleration;
    }
    boostSpeed = pSpeed(185.0f) + pSpeed(0.3f * (player->rings));
    if (player->characterArchetype == BoostArchetype) 
    {
        boostSpeed += BoostArchetypeBoostSpeeds[player->level];
    }
    player->gearStats[player->level].boostSpeed = boostSpeed;

    if (player->movementFlags & boosting) player->gearStats[player->level].topSpeed = boostSpeed;
    else if (player->state == Cruise || player->state == Fly)
    {
        topSpeed = player->gearStats[0].topSpeed + pSpeed(0.234f*(player->rings));
        player->gearStats[player->level].topSpeed = topSpeed;
        player->speedCap = topSpeed;
    }
}