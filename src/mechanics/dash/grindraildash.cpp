#include "grindraildash.hpp"
#include "cosmetics/player/exloads.hpp"
#include "gears/blastGaugeGears.hpp"
#include "lib/sound.hpp"
#include "lib/stdlib.hpp"
#include "riders/object.hpp"

ASMDefined u32 lbl_10017B18;
ASMDefined f32 lbl_001C40C0;
ASMDefined u32 lbl_10017F34;// boost particle visibility

ASMDefined void lbl_00048604();
ASMDefined void lbl_000CD4C8();                       // boost particle func
ASMDefined void InitBoostParticleTask(Player *player);// another boost particle func

ASMDefined struct FillerAttackStructArray {
	std::array<FillerAttackStruct, 3> filler;
} lbl_001E73E4;

ASMDefined struct Thing {
	f32 someFloat;
	fillerData<0x14> filler;
} lbl_001E739C;

ASMDefined struct GsAttackObjReadManager {
	u8 filler;
	u8 character;
} gsAttackObjReadManager;

struct RingLossObject1 {
	/* 0x0 */ f32 unk0;
	/* 0x4 */ f32 unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ u32 unkC;
	/* 0x10 */ u32 unk10;
	/* 0x14 */ u32 unk14;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ u32 unk1C;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ u32 unk28;
	/* 0x2C */ u32 unk2C;
	/* 0x30 */ u32 unk30;
	/* 0x34 */ u32 unk34;
	/* 0x38 */ void *unk38;
	/* 0x3C */ u32 unk3C;
	/* 0x40 */ u32 unk40;
};

void Player_InitBoostParticles(Player *player) {
	GsAttackObjReadManager *attackPreset = &gsAttackObjReadManager + player->index;
	Thing *thing = &lbl_001E739C + player->level;
	FillerAttackStructArray *attackStruct = &lbl_001E73E4 + attackPreset->character;
	FillerAttackStruct *attackStructLevel = &(attackStruct->filler[player->level]);

	BoostParticleObject1 *object1;

	lbl_10017F34 &= ~(1 << player->index);// make boost particle visible
	InitBoostParticleTask(player);
	object1 = (BoostParticleObject1 *) SetTask(&lbl_000CD4C8, ObjectGroups::BoostParticle, 2)->object;
	object1->player = player;
	object1->attackStruct = attackStructLevel;
	object1->unkfloat = thing->someFloat;
	object1->unknown = 0;
}

void Player_GrindRailDashHandler(Player *player) {
	EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);
	BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];

	if(player->state == RailGrind) {
		if(player->grindRailDash != 2) {
			if(player->input->toggleFaceButtons.hasAny(LButton)) {
				// initial cost
				player->grindRailDash = 1;
				PlayAudioFromDAT(Sound::SFX::RailHoopDash);// dash sfx
				Player_InitBoostParticles(player);
				if(player->specialFlags.hasAny(ringGear)) {
					if(player->character == SuperSonic
					   && bgInfo->currentGauge > 0
					   && exLoads.gearExLoadID != HyperSonicEXLoad){
						bgInfo->currentGauge -= GrindRailDashInitialCosts[player->level];
						if(bgInfo->currentGauge < 0) bgInfo->currentGauge = 0;
					} else {
						s32 newAir = player->currentAir - (player->gearStats[player->level].maxAir * 5) / 100;
						player->currentAir = clamp(newAir, 0);
					}
				}else{
					if (player->characterArchetype == Mechanic) {
						player->changeInAir_gain -= GrindRailDashInitialCostsMechanic[player->level];
					} else player->changeInAir_gain -= GrindRailDashInitialCosts[player->level];
				}
			} else if(player->input->holdFaceButtons.hasAny(LButton) && player->grindRailDash == 1) {
				if(player->currentAir > 0) {
					player->speed += pSpeed(2);
					f32 newMI;
					if(player->character == SuperSonic
					   && bgInfo->currentGauge > 0
					   && exLoads.gearExLoadID != HyperSonicEXLoad){
						newMI = player->magneticImpulse_timer;// essentially does nothing with blast gauge
					} else {
						newMI = player->magneticImpulse_timer - 6.0f;
					}
					player->magneticImpulse_timer = clamp(newMI, 0.f);
				} else {
					player->grindRailDash = 2;
				}
			} else if(player->grindRailDash == 1) {
				player->grindRailDash = 2;
			}
		}
	} else if(player->grindRailDash != 0) {
		player->grindRailDash = 0;
	}
}

void Player_RingLossVisualsSFX(Player *player) {
	lbl_10017B18 = gu32GameCnt;
	Sound::PlaySound(Sound::ID::IDKSFX, 0x39);

	u32 rings = player->rings;
	if(rings == 0) { return; }
	if(rings > 8) { rings = 8; }
	for(u32 i = 0; i < rings; i++) {
		auto *object = SetTask(lbl_00048604, ObjectGroups::RingLoss, 2);
		auto *object1 = static_cast<RingLossObject1 *>(object->object);

		object1->unk0 = player->x;
		object1->unk4 = player->y + lbl_001C40C0;
		object1->unk8 = player->z;

		object1->unk20 = 0;
		object1->unk1C = 0;
		object1->unk18 = 0;
		object1->unk40 = 0xF0;

		object->index = i;
		object->object_type = 0x834;
		object1->unk38 = &player->unk1FC;
		object->culling_group = 0x3F;
	}
}

ASMUsed void RingLoss_OnAttack(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	// if (exLoads.gearExLoadID == E99EXLoad)
	// {
	//     u32 ringRetain = player->rings;
	//     player->rings = ringRetain;
	//     return;
	// }
	if(player->specialFlags.hasAny(ringGear)) {
		u32 lossPercentage = 0;
		switch(player->rings){
			case 0 ... 15:
				lossPercentage = 1;
				break;
			case 16 ... 40:
				lossPercentage = 25;
				break;
			case 41 ... 70:
				lossPercentage = 35;
				break;
			case 71 ... 100:
				lossPercentage = 40;
				break;
			case 101 ... 130:
				lossPercentage = 50;
				break;
			case 131 ... 150:
				lossPercentage = 55;
				break;
			case 151 ... 170:
				lossPercentage = 60;
				break;
			default:
			case 171 ... ~0U:
				lossPercentage = 65;
				break;
		}
		//if(player->extremeGear != ExtremeGear::CoverP) {
			const s32 newRings = (player->currentAir * (100 - static_cast<s32>(lossPercentage))) / 100;
			player->currentAir = newRings;
		//} else {
		//	const u32 newCoverPRings = (player->rings * (100 - lossPercentage)) / 100;
		//	player->rings = newCoverPRings;
		//}
	} else {
		const auto newRings = static_cast<s32>(((player->rings * 85) / 100) - 10);
		player->rings = static_cast<u32>(clamp(newRings, 0));
	}
}

ASMUsed void Player_GrindRailAttack(Player *attackingPlayer, Player *attackedPlayer) {
	if((!attackedPlayer->specialFlags.hasAny(ringGear)) &&
	   (attackedPlayer->state == RailGrind) &&
	   (attackingPlayer->state == RailGrind) &&
	   (attackingPlayer->grindRailDash != 0)
	   ) {
		Player_RingLossVisualsSFX(attackedPlayer);
		RingLoss_OnAttack(attackedPlayer);
		attackedPlayer->changeInAir_gain -= 30000;
	}
}