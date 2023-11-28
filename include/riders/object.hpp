/** @file object.hpp
 * Header file for dealing with all objects on a stage in the game.
 */

#pragma once

#include <span>

#include "../flags.hpp"
#include "../globalDefs.hpp"
#include "../types.hpp"

/// Contains the specific IDs assigned to commonly seen stage objects.
enum ObjectTypes {
	ItemBox = 0x7DA, FlightRing = 0x848, DashPanel = 0x83E, WhiteCaveWeb = 0x1F40, Car = 0x13A8
};

/// Contains all the item ID values for an item box object.
enum ItemID {
	TenRings, TwentyRings, ThirtyRings, ThirtyAir, FiftyAir, HundredAir, SpeedShoes, Magnet,
	Invincibility, Bomb, BallAndChain, RNG, HundredRings, MaxAir, FiveRings
};

/// Dictates where an object is allowed to be spawned.
enum ObjectVisibility : u32 {
	GameCubeVisibility = 1 << 0,
	PlayStationVisibility = 1 << 1,
	XboxVisibility = 1 << 2,
	RaceVisibility = 1 << 3,
	TagVisibility = 1 << 4,
	SurvivalVisibility = 1 << 5,

	Mission1Visibility = 1 << 6,
	Mission2Visibility = 1 << 7,
	Mission3Visibility = 1 << 8,
	Mission4Visibility = 1 << 9,
	Mission5Visibility = 1 << 10,
	Mission6Visibility = 1 << 11,
	Mission7Visibility = 1 << 12
};
ENUM_FLAG_OPERATORS(ObjectVisibility); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast, readability-identifier-length, *-type-traits)

enum class ObjectGroups : u16 {
    MotionBlur = 0xE678, Particle = 0xB3B0, TitleSequence = 2, OffroadJumpParticle = 0xB0F4, BoostParticle = 0xB018,
    RingLoss = 0x7D65,

    // Custom TE object groups below

    Object_SuperTailsRenderText = 0xF000,
    Object_EmerlTypeIcon, ///< @note This object group is used directly in assembly! When modifying this, modify it in assembly as well.
    Object_TournamentRacePopup,
	Object_BallAndChain
};

/// The main struct which holds data about all currently spawned in objects. See variable StartOfObjectsPointer for these objects.
struct ObjectNode {
	Task update;
	ObjectNode *back;
	ObjectNode *next;
	u16 flags;
	u16 object_group;
	void* object;
	u8 state;
	u16 object_type;
	u8 culling_group;
	u16 item_id;
	u16 object_id;
	u16 index;
};
static_assert(sizeof(ObjectNode) == 0x20);

/// Contains all required info about an object to be spawned in. These structs are contained within a specific file in every stage's files.
struct ObjectInfo {
	u16 objectType;
	u8 maxPlayerCount; ///< maximum amount of required players in the game for object to spawn
	u8 objectPortal;
    ObjectVisibility spawnableGamemodes;
	union {
		u32 extraAttribute;
		u32 itemID; ///< for item boxes
		u32 controlLockFrames; ///< for fly hoops
	};
	f32 x;
	f32 y;
	f32 z;
	// BAMS rotations? multiplies this value with 182.044
	f32 xRotation;
	f32 yRotation;
	f32 zRotation;
	union {
		f32 extraAttribute2;
		f32 flyHoopSpeed; ///< don't use pSpeed for this
	};
	union {
		f32 extraAttribute3;
		f32 flyHoopScale;
	};
	f32 unknown;
};
static_assert(sizeof(ObjectInfo) == 0x30);

ASMDefined const vu16 CurrentObjectAmount;
ASMDefined const vu32 MaxObjectAmount;
ASMDefined ObjectNode* const StartOfObjectsPointer;

/**
 * Sets a new task.
 * Tasks are simply routines that are ran every frame and you can specify their lifetime.
 * Use function gNp_DeadTask() within your Task to "delete" them and free their memory.
 *
 * @param task The task that handles the object.
 * @param objectGroup The object group to associate with this task.
 * @param object1Size Sets the size of the extra data struct used at ObjectNode::object if necessary for the task. Set this to 0 for no extra data, 1 for 0x20 sized extra data and 2 for 0x80 sized extra data.
 */
ASMDefined ObjectNode* SetTask(Task task, ObjectGroups objectGroup, u32 object1Size);

/// Kills the current task.
ASMDefined void gNp_DeadTask();

[[nodiscard]] inline std::span<ObjectNode> getObjectList() {
	return {StartOfObjectsPointer, CurrentObjectAmount};
}

struct ParticleTaskObject1 {
	/* 0x0 */ f32 x;
	/* 0x4 */ f32 y;
	/* 0x8 */ f32 z;
	/* 0xC */ u32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ u32 unk1C;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ u32 unk28;
	/* 0x2C */ u32 unk2C;
	/* 0x30 */ f32 unk30;
	/* 0x34 */ f32 unk34;
	/* 0x38 */ f32 unk38;
	/* 0x3C */ f32 unk3C;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ f32 unk44;
	/* 0x48 */ void* unk48;
	/* 0x4C */ u32 unk4C;
	/* 0x50 */ u32 unk50;
	/* 0x54 */ u32 unk54;
	/* 0x58 */ u32 unk58;
	/* 0x5C */ u32 unk5C;
	/* 0x60 */ void* unk60;
	/* 0x64 */ u32 unk64;
	/* 0x68 */ void* unk68;
	/* 0x6C */ void* unk6C;
	/* 0x70 */ u8 unk70;
	/* 0x71 */ u8 unk71;
	/* 0x72 */ u8 unk72;
	/* 0x73 */ u8 unk73;
	/* 0x74 */ u32 unk74;
	/* 0x78 */ u32 unk78;
	/* 0x7C */ u32 unk7C;
};
static_assert(sizeof(ParticleTaskObject1) == 0x80); // NOLINT(readability-magic-numbers)

struct ParticleObject {
	fillerData<0x10> filler;
	struct ParticleObject1 *object1;
};

struct MagicCarpetParticle {
	u16 unk0;
	u16 unk2;
	u16 unk4;
    ObjectGroups unk6;
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
	/* 0x60 */ volatile MagicCarpetParticle *unk60;
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