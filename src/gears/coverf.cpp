#include "coverf.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"
#include "riders/object.hpp"

inline u32 Player_CoverF_ChangeArchetype(Player *player, const u8 archetype) {
	u32 archetypeCase;
	if(player->coverF_archetype == archetype && player->rings >= 25) {
		archetypeCase = 2; // same mode toggled
	} else if(player->rings < 30) {
		archetypeCase = 0;
	} else {
		archetypeCase = 1;
	}

	return archetypeCase;
}

inline void Player_CoverF_UpdateStats(Player *player, const u8 archetype) {
	player->coverF_archetype = archetype;

    player->gearStats[1].topSpeed = CoverF_Speeds[archetype-1][0] + Archetype_TopSpeedBonus[std::to_underlying(player->characterArchetype)];
    if (player->state == PlayerState::Cruise) player->speedCap = CoverF_Speeds[archetype-1][0] + Archetype_TopSpeedBonus[std::to_underlying(player->characterArchetype)];
    if (player->characterArchetype == CharacterArchetype::Boost){ // boost character support
        player->gearStats[1].boostSpeed = CoverF_SpeedsBoostArchetype[archetype-1][1];
    } else {
		player->gearStats[1].boostSpeed = CoverF_Speeds[archetype-1][1];
	}
}

void Player_CreateCoverFParticles(Player *player) {
	auto *particles = static_cast<ParticleTaskObject1 *>(SetTask(func_Particle_Task, ObjectGroups::Particle, 2)->object);
	particles->unk72 = player->index;
	particles->x = 0.0f;
	particles->z = 0.0f;
	particles->unk10 = 0.0f;
	particles->unk18 = 0.0f;
	particles->y = 0.3f;
	particles->unk14 = 0.8f;
	particles->texList = gpsTexList_Particle;
	particles->particleParams = &lbl_001D8F58;
	particles->unk73 = static_cast<u8>(~0);
	particles->baseModelMatrix = nullptr;
	particles->unk74 = 0;
	particles->unk48 = &player->x;
}

void Player_CoverF(Player *player) {
	if(player->extremeGear == ExtremeGear::CoverF && player->state == PlayerState::StartLine) {
		player->level = 1;
		player->gearStats[1].boostSpeed = player->gearStats[0].boostSpeed;
	}
	if(player->extremeGear != ExtremeGear::CoverF || player->rings < 25 || player->state == PlayerState::AttackedByPlayer) { return; }

	s32 airToAdd = player->gearStats[1].maxAir;
	if(player->input->toggleFaceButtons.hasAny(Buttons::DPadLeft)) {
		// Solution Turbo (ST)
		const u8 archetype = 2;
		switch(Player_CoverF_ChangeArchetype(player, archetype)) {
			default:
				return;
			case 1:
				player->changeInAir_gain += airToAdd;
				player->rings -= 30;
				break;
			case 2:
				airToAdd /= 2;
				player->changeInAir_gain += airToAdd;
				player->rings -= 25;
				break;
		}
		Player_CoverF_UpdateStats(player, archetype);

		player->gearStats[0].boostCost = 45000;
		player->gearStats[1].boostCost = player->gearStats[1].maxAir / 2;
		player->specialFlags &= ~SpecialFlags::berserkerEffect;

		Player_CreateCoverFParticles(player);
		PlayAudioFromDAT(Sound::SFX::CovFModeSwitch);

	} else if(player->input->toggleFaceButtons.hasAny(Buttons::DPadRight)) {
		// Solution Cruise (SC)
		const u8 archetype = 1;
		switch(Player_CoverF_ChangeArchetype(player, archetype)) {
			default:
				return;
			case 1:
				player->changeInAir_gain += airToAdd;
				player->rings -= 30;
				break;
			case 2:
				airToAdd /= 2;
				player->changeInAir_gain += airToAdd;
				player->rings -= 25;
				break;
		}
		Player_CoverF_UpdateStats(player, archetype);

		player->gearStats[0].boostCost = player->gearptr->levelStats[0].boostCost;
		player->gearStats[1].boostCost = player->gearptr->levelStats[1].boostCost;
		player->specialFlags &= ~SpecialFlags::berserkerEffect;

		Player_CreateCoverFParticles(player);
		PlayAudioFromDAT(Sound::SFX::CovFModeSwitch);

	} else if(player->input->toggleFaceButtons.hasAny(Buttons::DPadUp)) {
		// Solution Battle (SB)
		const u8 archetype = 3;
		if(player->characterArchetype == CharacterArchetype::Boost) {
			if(player->state != PlayerState::RailGrind && !player->movementFlags.hasAny(MovementFlags::boosting) && player->state != PlayerState::AttackingPlayer) {
				player->attackProperties = nullptr;
			}
		} else if(!player->movementFlags.hasAny(MovementFlags::boosting) && player->state != PlayerState::AttackingPlayer) {
			player->attackProperties = nullptr;
		}

		switch(Player_CoverF_ChangeArchetype(player, archetype)) {
			default:
				return;
			case 1:
				player->changeInAir_gain += airToAdd;
				player->rings -= 30;
				break;
			case 2:
				airToAdd /= 2;
				player->changeInAir_gain += airToAdd;
				player->rings -= 25;
				break;
		}
		Player_CoverF_UpdateStats(player, archetype);

		player->gearStats[0].boostCost = 20000;
		player->gearStats[1].boostCost = 25000;
		player->specialFlags |= SpecialFlags::berserkerEffect;

		Player_CreateCoverFParticles(player);
		PlayAudioFromDAT(Sound::SFX::CovFModeSwitch);
	}
}