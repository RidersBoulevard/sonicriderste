#pragma once
#include "main.hpp"
#include "vector3.hpp"
#include "rgba.hpp"

struct Sys_2dGraphicalDataHeader;

/// Used in multiple 2D menus of the game. Places like the main menu, CSS screen, etc use this struct and the accompanied system.
struct GraphicalObject {
	// size 0xC0
	Matrix3x4F transform;
	void (*updateFunction)(GraphicalObject &);

	union {
		u32 id;
		struct {
			u16 graphicalDataID;
			u8 otherID;
			u8 idIndex;
		} idStruct;
	};

    union {
        u32 user1_U32;

        struct {
            u16 user1_U16_1;
            u16 user1_U16_2;
        };

        struct {
            u8 user1_U8_1;
            u8 user1_U8_2;
            u8 user1_U8_3;
            u8 user1_U8_4;
        };
    };

    union {
        u32 user2_U32;

        struct {
            u16 user2_U16_1;
            u16 user2_U16_2;
        };

        struct {
            u8 user2_U8_1;
            u8 user2_U8_2;
            u8 user2_U8_3;
            u8 user2_U8_4;
        };
    };

	Sys_2dGraphicalDataHeader *graphicalData;
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
    u32 extraTransformAnimationFrame;
    u16 unk5C; // 2 if at the end of animation?
    s16 extraTransformObjectID; // -1 if none
    u8 unk60; // always 5 if extra transform object id exists?
    s8 extraTransformIndex; // which index layer (ignores first layer) to use from the extra transform object to transform current object, otherwise -1
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

	RGBA32 tint;

    u32 unkB0;
    u32 unkB4;
    u32 unkB8;
    u32 unkBC;

	void setExtraTransform(const s16 objectID, const s8 transformIndex = 0) {
		extraTransformObjectID = objectID;
		unk60 = 5;
		extraTransformIndex = transformIndex;
	}
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

struct Sys2dObjectInfo {
	void (*updateFunc)(GraphicalObject &);
	u16 objectID;
	u8 extraData = 0xFF;
	u8 index = 0xFF;
};

struct Sys2dHeap {
	std::array<GraphicalObject, 128> objects;
	m2darray<void*, 50, 2> objectGroupPtrs;
	Task* mainSys2dTask;
	void* unknown1;
	void* unknown2;
};
static_assert(sizeof(Sys2dHeap) == 0x619C);

ASMDefined Sys2dHeap* gp2DSys;

ASMDefined {
    // initializes a new 2d object table
    void Sys2d_SetObjectTbl(Sys_2dObjectTableData *data, void *objectTable, u32 objectCount, u32 objectTableEntrySize, void *objectAssociatedPtr, u8 groupIndex, bool defaultObjectState);

	// initializes a singular 2d object
	void Sys2d_SetObjectBuff(Sys_2dObjectTableData *data, GraphicalObject *savedNewObject, const void *objectInfo, u32 objectInfoEntrySize, void *objectAssociatedPtr, u8 groupIndex, bool defaultObjectState);

    // disables/enables all 2d objects in a specific group
    void Sys2d_RunChgObjectGROUP_IN(u8 groupIndex, bool objectState);

    // clears/frees 2d objects
    void Sys2d_ClrObjectALL();
    void Sys2d_ClrObjectGROUP_IN(u8 groupIndex);

    void Sys2d_DrawObject(GraphicalObject *object);
}