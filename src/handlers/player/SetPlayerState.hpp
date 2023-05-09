#pragma once

#include "context.hpp"

/**
 * SetPlayerState's asm declared things
 */
global void func_ClearPlayerMotionTable(Player *);
global void func_GetDirectTrickBonus(Player *, u32);
global void func_StartPlayerMotionTable(Player *);
global void func__RegistPlayerMotionTable(Player *, u32, u32, f32, f32);
global void lbl_0009195C(void *);
global void lbl_000FC114(Player *);
global void lbl_00187ED0(Player *);
global void lbl_Player_BoostEndFunction(void *);
global void func_Particle_Task();

global void* gcosNnSystemVecZeroFast;
global f32 gf32_SystemHzAdd1;
global s32 gpsTexList_Particle;
global struct MagicCarpetParticle gsParFullParam_MagicCarpet;
// global void* lbl_001CCE20; // rodata
global s32 lbl_10087C40;
	// End of ASM stuff


struct ParticleObject {
	char filler[0x10];
	struct ParticleObject1 *object1;
};

struct MagicCarpetParticle {
	u16 unk0;
	u16 unk2;
	u16 unk4;
	u16 unk6;
	/* 0x8 */ u32 unk8;
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
	/* 0x38 */ u32 unk38;
	/* 0x3C */ u32 unk3C;
	/* 0x40 */ u32 unk40;
	/* 0x44 */ u32 unk44;
	/* 0x48 */ u32 unk48;
	/* 0x4C */ u32 unk4C;
	/* 0x50 */ u32 unk50;
	/* 0x54 */ u32 unk54;
	/* 0x58 */ u32 unk58;
	/* 0x5C */ u32 unk5C;
	/* 0x60 */ u32 unk60;
	/* 0x64 */ u32 unk64;
	/* 0x68 */ u32 unk68;
	/* 0x6C */ u32 unk6C;
	/* 0x70 */ u32 unk70;
	/* 0x74 */ u32 unk74;
	/* 0x78 */ u32 unk78;
	/* 0x7C */ u32 unk7C;
	/* 0x80 */ u32 unk80;
	/* 0x84 */ u32 unk84;
	/* 0x88 */ u32 unk88;
	/* 0x8C */ u32 unk8C;
	/* 0x90 */ u32 unk90;
	/* 0x94 */ u32 unk94;
	/* 0x98 */ u32 unk98;
	/* 0x9C */ u32 unk9C;
	/* 0xA0 */ u32 unkA0;
};

struct ParticleObject1 {
	/* 0x0 */ u32 unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 unk8;
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
	/* 0x38 */ u32 unk38;
	/* 0x3C */ u32 unk3C;
	/* 0x40 */ u32 unk40;
	/* 0x44 */ u32 unk44;
	/* 0x48 */ void *unk48;
	/* 0x4C */ u32 unk4C;
	/* 0x50 */ u32 unk50;
	/* 0x54 */ u32 unk54;
	/* 0x58 */ u32 unk58;
	/* 0x5C */ u32 unk5C;
	/* 0x60 */ struct MagicCarpetParticle *unk60;
	/* 0x64 */ u32 unk64;
	/* 0x68 */ u32 unk68;
	/* 0x6C */ void *unk6C;
	/* 0x70 */ u8 unk70;
	/* 0x70 */ u8 unk71;
	/* 0x70 */ u8 unk72;
	/* 0x70 */ s8 unk73;
	/* 0x74 */ u32 unk74;
	/* 0x78 */ u32 unk78;
	/* 0x7C */ u32 unk7C;
};

ASMUsed void func_SetPlayerActionType(Player *player, u32 state);