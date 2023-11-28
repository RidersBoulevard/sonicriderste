#pragma once
#include "main.hpp"
#include "vector3.hpp"
#include "rgba.hpp"

/// Used in multiple 2D menus of the game. Places like the main menu, CSS screen, etc use this struct and the accompanied system.
struct GraphicalObject {
	// size 0xC0
	Matrix3x3F transform;
	void *updateFunction;

	union {
		u32 id;
		struct {
			u16 graphicalDataID;
			u8 otherID;
			u8 idIndex;
		} idStruct;
	};

	u32 unk38;
	u32 unk3C;
	void *graphicalData;
	void *textureData;
	u32 activeTimer;
	u16 index;
	u8 cullingGroup;
	u8 unk4F;
	u32 currentAnimationFrame;
	u16 unk54;
	u8 unk56;
    u8 unk57_1 : 4;
    bool active : 1;
    u8 unk57_2 : 1;
    u8 unk57_3 : 1;
    u8 unk57_4 : 1;
    u32 currentAnimationFrame2;
    u16 unk5C; // 2 if at the end of animation?
    s16 extraTransformObjectID; // -1 if none
    u8 unk60; // always 5 if extra transform object id exists?
    s8 extraTransformIndex; // which index to use from the extra transform object to transform current object, otherwise -1
	fillerData<0x26> filler;

	struct TextureSlots {
		u16 textureSlot1;
		u16 textureSlot2;
		u16 textureSlot3;
		u16 textureSlot4;
		u16 textureSlot5;
		u16 textureSlot6;
	};
	std::array<TextureSlots, 2> textureIDs;

	Vector3F textureTransform;

	RGBA tint;

    u32 unkB0;
    u32 unkB4;
    u32 unkB8;
    u32 unkBC;
};
static_assert(sizeof(GraphicalObject) == 0xC0);

struct Sys_2dGraphicalDataObject {
    u32 objectCount;
    u32 size;
    void* objects[];
};

struct Sys_2dGraphicalDataHeader {
    u16 unk0;
    u16 unk2;
    u32 unk4;
    void *objectTableStart;
    void *textureSlots;
    u32 objectCount;
    u32 unk14;
    Sys_2dGraphicalDataObject* objects[];
};

struct Sys_2dObjectTableData {
    Sys_2dGraphicalDataHeader *graphicalData;
	void *textureData;
};

ASMDefined void* const gp2DSys;

ASMDefined {
    // initializes a new 2d object table
    void Sys2d_SetObjectTbl(Sys_2dObjectTableData *data, void *objectTable, u32 objectCount, u32 objectTableEntrySize, void *objectAssociatedPtr, u8 groupIndex, bool defaultObjectState);

    // disables/enables all 2d objects in a specific group
    void Sys2d_RunChgObjectGROUP_IN(u8 groupIndex, bool objectState);

    // clears/frees 2d objects
    void Sys2d_ClrObjectALL();
    void Sys2d_ClrObjectGROUP_IN(u8 groupIndex);

    void Sys2d_DrawObject(GraphicalObject *object);
}