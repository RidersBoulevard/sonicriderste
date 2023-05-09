#pragma once

#include "main.hpp"
#include <array>

constexpr int MIVoiceLinesPerChar = 3;

constexpr f32 MI_NoTypeMultiplier = 2.0f;
constexpr f32 MI_DualTypeMultiplier = 0.8f;
constexpr f32 MI_AllTypeMultiplier = 0.6f;

constexpr f32 MI_MaximumCap = 1200.0f; // maximum amount of frames before MI reaches maximum state
constexpr f32 MI_MaxMITime = 420.0f; // how long max MI is guaranteed for
constexpr f32 MI_LossTimeCap = 900.0f; // the amount of frames it caps to when MI is lost
constexpr f32 MI_BufferTime = 60.0f; // how many frames before MI turns off/on

constexpr std::array<std::array<u8, MIVoiceLinesPerChar>, TotalCharacterAmount> MI_VoiceLineIDs = {{
        {0x02},    			// Sonic
        {0x02},    			// Tails
        {0x02},    			// Knuckles
        {0x02},    			// Amy
        {0x02},    			// Jet
        {0x02},    			// Storm
        {0x02},    			// Wave
        {0x02},    			// Eggman
        {0x02},    			// Cream
        {0x02},    			// Rouge
        {0x02},    			// Shadow
        {0x02},    			// SuperSonic
        {0},    			// Nights
        {0x02},    			// Aiai
        {0x02},    			// Ulala
        {0x00},    			// E10G
        {0x00},    			// E10R
        {0x08, 0x09, 0x0A}, // Silver
        {0x06, 0x07, 0x08}, // MetalSonic
        {0x02, 0x07},    	// Emerl
        {0x08, 0x09},    	// Blaze
        {0x07},    			// Chaos
        {0}    				// Tikal
}};

// placeholder struct for storing MI data till I allocate stuff over to player data
struct MagneticImpulseData {
	u8 unused;
	BOOL magneticImpulse;
	u16 unused2;
	f32 buffer;
	f32 interrupt;
	f32 maxMItimer;
	f32 ringPowerObjTimer;
	u32 afterburnerTimer;
};

extern MagneticImpulseData magneticImpulseData[8];
extern Matrix3x3 PlayerPositionRotationMatrices[8];

f32 MagneticImpulse_CalculateMultiplier(Player *player, f32 value);

void MagneticImpulse_CreateGuaranteedMaxMIParticles();
void Player_MagneticImpulseTimer(Player &player);
f32 ScaleUsingCurrentMI(Player &player, f32 value);

struct LevelUpStarsObject1 {
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
	/* 0x48 */ const void *unk48;
	/* 0x4C */ u32 unk4C;
	/* 0x50 */ u32 unk50;
	/* 0x54 */ u32 unk54;
	/* 0x58 */ u32 unk58;
	/* 0x5C */ u32 unk5C;
	/* 0x60 */ void *unk60;
	/* 0x64 */ u32 unk64;
	/* 0x68 */ void *unk68;
	/* 0x6C */ u32 unk6C;
	/* 0x70 */ u8 unk70;
	/* 0x71 */ u8 unk71;
	/* 0x72 */ u8 unk72;
	/* 0x73 */ u8 unk73;
	/* 0x74 */ u32 unk74;
	/* 0x78 */ u32 unk78;
	/* 0x7C */ u32 unk7C;
};

struct Particle_LevelUpStars {
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

extern Particle_LevelUpStars Particle_LevelUpStars;

ASMUsed f32 MagneticImpulse_ParticleSpeed(Player *player);
ASMUsed void MagneticImpulse_AttackParticles(Player *player, u8 battleModeData[], u32 offset);
ASMUsed void MagneticImpulse_GuardParticles(Player *player, u8 battleModeData[], u32 offset);
ASMUsed f32 Player_MISpeedAirGain(Player *player, f32 airGain);
ASMUsed f32 Player_MIFlyAirGain(Player *player, f32 airGain);
ASMUsed f32 Player_MIPowerObjectAirGain(Player *player, f32 airGain);
ASMUsed f32 Player_MITrickAirGain(Player *player, f32 airGain);
ASMUsed void Player_MIPitAirGain(Player *player);
ASMUsed f32 Player_MIPitSpeed(Player *player, f32 pitSpeed);
ASMUsed f32 Player_MITrickLandingSpeed(Player *player, f32 currentTrickSpeed);
ASMUsed s16 Player_MIBabylonGardenMiniSpiralQTE(Player *player, void *qte);
ASMUsed s32 Player_MIRingBoxGain(Player *player);
ASMUsed f32 Player_MIRingDrain(Player *player, f32 ringDrain);
ASMUsed void Player_MISpeedShoes(Player *player);
ASMUsed void Player_MIAirBoxGain(Player *player, s32 airToAdd);
ASMUsed void MagneticImpulse_ClearData();
