#pragma once
#include "main.hpp"
#include "vector3.hpp"
#include "rgba.hpp"

struct GraphicalObject {
	// size 0xC0
	Matrix3x3 transform;
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
	u32 unk50;
	u16 unk54;
	u8 unk56;
	u8 active;
	char filler[0x30];

	struct {
		u16 textureSlot1;
		u16 textureSlot2;
		u16 textureSlot3;
		u16 textureSlot4;
		u16 textureSlot5;
		u16 textureSlot6;
	} textureIDs[2];

	Vector3 textureTransform;

	RGBA tint;

	char filler2[0x10];
};

struct Sys_2dObjectTableData {
	void *graphicalData;
	void *textureData;
};

global void *gp2DSys;

global {
    // initializes a new 2d object table
    void Sys2d_SetObjectTbl(Sys_2dObjectTableData *data, void *objectTable, u32 objectCount, u32 objectTableEntrySize, void *objectAssociatedPtr, u8 groupIndex, BOOL defaultObjectState);

    // disables/enables all 2d objects in a specific group
    void Sys2d_RunChgObjectGROUP_IN(u8 groupIndex, BOOL objectState);

    // clears/frees 2d objects
    void Sys2d_ClrObjectALL();
    void Sys2d_ClrObjectGROUP_IN(u8 groupIndex);

    void Sys2d_DrawObject(GraphicalObject *object);
}