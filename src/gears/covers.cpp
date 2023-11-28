#include "covers.hpp"
#include "lib/sound.hpp"
#include "riders/object.hpp"

inline u32 Player_CoverS_ChangeArchetype(Player *player, const Type &shortcutType) {
    u32 archetypeCase{};
    if (player->typeAttributes == shortcutType) { archetypeCase = 2; } // same mode toggled, does nothing
    else if (player->rings < 30) { archetypeCase = 0; } // rings less than requirement does nothing
    else { archetypeCase = 1; } // different mode toggled

    return archetypeCase;
}

void Player_CreateCoverSParticles(Player *player) {
    auto *particles = static_cast<ParticleTaskObject1 *>(SetTask(func_Particle_Task, ObjectGroups::Particle, 2)->object);
    particles->unk72 = player->index;
    particles->x = 0.0F;
	particles->y = 0.3F;
    particles->z = 0.0F;
    particles->unk10 = 0.0F;
    particles->unk18 = 0.0F;
    particles->unk14 = 0.8F;
    particles->unk68 = gpsTexList_Particle;
    particles->unk60 = &lbl_001D8F58;
    particles->unk73 = static_cast<u8>(~0);
    particles->unk6C = nullptr;
    particles->unk74 = 0;
    particles->unk48 = &player->x;
}

void Player_CoverS(Player *player) {
    if (player->extremeGear != ExtremeGear::CoverS) { return; }
    player->level = 1;
    // player->gearStats[2].airDrain = 20;
    // player->gearStats[1].boostCost = static_cast<s32>(static_cast<f32>(player->gearStats[2].boostCost) * 0.75F);
    // player->gearStats[2].driftCost = player->gearStats[1].driftCost;
    // player->gearStats[2].tornadoCost = player->gearStats[1].tornadoCost;
    // player->gearStats[1].boostSpeed = player->gearStats[2].boostSpeed - pSpeed(10.0F);

    // if (player->state == StartLine) { player->currentAir = player->gearStats[player->level].maxAir; }
    if (player->rings < 20) { return; }
    if (player->input->toggleFaceButtons.hasAny(DPadLeft)) {
        // Speed Type
        switch (Player_CoverS_ChangeArchetype(player, Type::Speed)) {
            default:
                return;
            case 1:
                player->rings -= 30;
                player->typeAttributes = Type::Speed;
                break;
            case 2:
                return;
        }
        Player_CreateCoverSParticles(player);
        PlayAudioFromDAT(Sound::SFX::CovFModeSwitch);
    } else if (player->input->toggleFaceButtons.hasAny(DPadUp)) {
        // Fly Type
        switch (Player_CoverS_ChangeArchetype(player, Type::Fly)) {
            default:
                return;
            case 1:
                player->rings -= 30;
                player->typeAttributes = Type::Fly;
                break;
            case 2:
                return;
        }
        Player_CreateCoverSParticles(player);
        PlayAudioFromDAT(Sound::SFX::CovFModeSwitch);
    } else if (player->input->toggleFaceButtons.hasAny(DPadRight)) {
        // Power Type
        switch (Player_CoverS_ChangeArchetype(player, Type::Power)) {
            default:
                return;
            case 1:
                player->rings -= 30;
                player->typeAttributes = Type::Power;
                break;
            case 2:
                return;
        }
        Player_CreateCoverSParticles(player);
        PlayAudioFromDAT(Sound::SFX::CovFModeSwitch);
    }
}