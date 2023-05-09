#pragma once

#include "context.hpp"

global struct lbl_001D78C8 {
	u32 int1;
	u32 int2;
	u32 int3;
	u32 int4;
	f32 unkf;
	f32 unkf1;
	f32 unkf2;
	f32 f1;
	f32 f2;
	f32 f3;
	f32 f5;
	f32 f6;
} lbl_001D78C8;


global void *lbl_100066C0;
global void TitleSequence_Task();
global u32 lbl_8021BB84;
global u32 lbl_8021BB68;
global u32 gaNnViewRotInvertMtx[50];

global u32 OSGetTick();
global void lbl_0004D1A4(u32);
global void lbl_00175C7C();
global void InitCamPalette(u32);
global void InitCamera(u32);
global void nnInitLight();
global void InitLight(u32);
global void nnSetClipScreenCoordinates(u32 []);
global void lbl_000B7BFC();
global void lbl_001514A8();
global void lbl_800645A8(void *, f32, f32, f32, f32, f32, f32);
global void lbl_80006778();
global void lbl_0004F404();
global void gNp_TaskDone(u32 stage, Player *player, PlayerCameraStruct *camera);
global void lbl_0014C080();
global void func_800303B0(u32);
global void gNp_VWaitTrans();
global void lbl_000B766C();
global void lbl_000375B4();
global void ReadAsyncReadFileData(u32);