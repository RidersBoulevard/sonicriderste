#include "context.hpp"
#include "cosmetics/player/exloads.hpp"
#include <cmath>

struct BoneData {
	char filler[0x1C4];
	u32 unk1;
	char filler2[0x24];
	u32 unk2;
	char filler3[0x24];
	u32 unk3;
	char filler4[0x7B4];
	f32 f6;
	f32 f5;
	f32 f4;
	char filler5[0xBC];
	f32 f3;
	f32 f2;
	f32 f1;
};

struct UnkTest {
	u32 filler[4];
};

struct UnkTest2 {
	f32 filler[4];
};

global s8 lbl_10087CF4;

global void nnMultiplyQuaternion(void *, void *, void *);
global void nnMakeRotateZXYQuaternion(void *, s32, s32, s32);

ASMUsed void Silver_QuillMovement(Player *player, BoneData *bonedata) {
	//f32 value;
	f32 boneWeight;
	f64 unkn;
	u32 gameCnt = gu32GameCnt;
	//u32 testing,testing1;
	s32 unkn2, unkn3;
	UnkTest test{};
	//UnkTest2 test2{};

	if (player->speed > pSpeed(250)) {
		boneWeight = 1102.55f;
	} else {
		boneWeight = player->speed * 927.375f;
	}

	if (!gu32Game_PauseFlag) lbl_10087CF4 = static_cast<s8>(player->unknownVal);
	unkn = std::sin(gameCnt * 0.54f);
	unkn2 = lbl_10087CF4 + 15;
	unkn3 = unkn2 * (6.0f * player->speed);
	unkn2 = boneWeight * (f32) unkn;

	nnMakeRotateZXYQuaternion(&test, 0, -unkn3, unkn2);
	nnMultiplyQuaternion(&bonedata->unk1, &bonedata->unk1, &test);

	unkn = std::sin((gameCnt - 15) * 0.54f);

	unkn2 = boneWeight * (f32) unkn;
	nnMakeRotateZXYQuaternion(&test, 0, 0, unkn2);
	nnMultiplyQuaternion(&bonedata->unk2, &bonedata->unk2, &test);

	unkn = std::sin((gameCnt - 30) * 0.54f);

	unkn2 = boneWeight * (f32) unkn;
	nnMakeRotateZXYQuaternion(&test, 0, 0, unkn2);
	nnMultiplyQuaternion(&bonedata->unk3, &bonedata->unk3, &test);

	/*

	// this breaks the model for some reason

	if (player->gearptr->model == 0)
	{
		testing = 0x3f800000;
		testing1 = 0x3f4ccccd;
		test2.filler[0] = testing;
		test2.filler[1] = testing1;
		value = test2.filler[player->flags & 0x100 ? TRUE : FALSE];
		bonedata->f1 = value;
		bonedata->f2 = value;
		bonedata->f3 = value;
		bonedata->f4 = value;
		bonedata->f5 = value;
		bonedata->f6 = value;
	}
	*/
}

ASMUsed void Blaze_QuillMovement(Player *player, BoneData *bonedata) {
	//f32 value;
	f32 boneWeight;
	f64 unkn;
	u32 gameCnt = gu32GameCnt;
	//u32 testing,testing1;
	s32 unkn2, unkn3;
	UnkTest test{};
	//UnkTest2 test2;

	if (player->speed > pSpeed(250)) {
		boneWeight = 2941.5f;
	} else {
		boneWeight = player->speed * 2685.5f;
	}

	if (!gu32Game_PauseFlag) lbl_10087CF4 = static_cast<s8>(player->unknownVal);
	unkn = std::sin(gameCnt * 0.54f);
	unkn2 = lbl_10087CF4 + 15;
	unkn3 = unkn2 * (6.0f * player->speed);
	unkn2 = boneWeight * (f32) unkn;

	nnMakeRotateZXYQuaternion(&test, 0, -unkn3, unkn2);
	nnMultiplyQuaternion(&bonedata->unk1, &bonedata->unk1, &test);

	unkn = std::sin((gameCnt - 15) * 0.54f);

	unkn2 = boneWeight * (f32) unkn;
	nnMakeRotateZXYQuaternion(&test, 0, 0, unkn2);
	nnMultiplyQuaternion(&bonedata->unk2, &bonedata->unk2, &test);

	unkn = std::sin((gameCnt - 30) * 0.54f);

	unkn2 = boneWeight * (f32) unkn;
	nnMakeRotateZXYQuaternion(&test, 0, 0, unkn2);
	nnMultiplyQuaternion(&bonedata->unk3, &bonedata->unk3, &test);

	/*

	// this breaks the model for some reason

	if (player->gearptr->model == 0)
	{
		testing = 0x3f800000;
		testing1 = 0x3f4ccccd;
		test2.filler[0] = testing;
		test2.filler[1] = testing1;
		value = test2.filler[player->flags & 0x100 ? TRUE : FALSE];
		bonedata->f1 = value;
		bonedata->f2 = value;
		bonedata->f3 = value;
		bonedata->f4 = value;
		bonedata->f5 = value;
		bonedata->f6 = value;
	}
	*/
}

ASMUsed void MikuCharacterFeatures(Player *player, BoneData *bonedata) {
    EnabledEXLoads exLoads{};
    FetchEnabledEXLoadIDs(player, exLoads);

    if (exLoads.characterExLoadID == HatsuneMikuEXLoad)
        Blaze_QuillMovement(player, bonedata);
}