#pragma once

#include "globalDefs.hpp"

ASMDefined struct lbl_001D78C8 {
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


ASMDefined void *lbl_100066C0;
ASMDefined void TitleSequence_Task();
ASMDefined u32 lbl_8021BB84;
ASMDefined u32 lbl_8021BB68;
ASMDefined std::array<u32, 50> gaNnViewRotInvertMtx;

ASMDefined u32 OSGetTick();
ASMDefined void lbl_0004D1A4(u32);
ASMDefined void lbl_00175C7C();
ASMDefined void InitCamPalette(u32);
ASMDefined void InitCamera(u32);
ASMDefined void nnInitLight();
ASMDefined void InitLight(u32);
ASMDefined void nnSetClipScreenCoordinates(u32 []);
ASMDefined void lbl_000B7BFC();
ASMDefined void lbl_001514A8();
ASMDefined void lbl_800645A8(void *, f32, f32, f32, f32, f32, f32);
ASMDefined void lbl_80006778();
ASMDefined void lbl_0004F404();
ASMDefined void gNp_TaskDone();
ASMDefined void lbl_0014C080();
ASMDefined void func_800303B0(u32);
ASMDefined void gNp_VWaitTrans();
ASMDefined void lbl_000B766C();
ASMDefined void lbl_000375B4();
ASMDefined void ReadAsyncReadFileData(u32);