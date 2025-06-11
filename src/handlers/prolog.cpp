// THIS FUNCTION IS ALMOST MATCHED
// it's not a perfect match but the functionality is exactly the same

#include "prolog.hpp"
#include "customcodehandler.hpp"
#include "menu/debugmenu/debugmenu.hpp"
#include "menu/debugmenu/debugmenu_handlers.hpp"
#include "nn/ninjanext.hpp"
#include "files/filehandler_dat.hpp"
#include <ogc/gx.h>
#include <ogc/card.h>

#define DEBUG_TESTING false && DEBUG

#if DEBUG_TESTING
#include <debug.h>
ASMDefined void __exception_init();
#endif

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-magic-numbers"
#pragma ide diagnostic ignored "readability-implicit-bool-conversion"
ASMUsed [[noreturn]] void _prolog() {

	const u32 tick = OSGetTick();
	srand(tick);
	lbl_0004D1A4(tick);
	CARD_Init("GXSR", "TE");

	InitCamPalette(0);

	InitCamera(4);
	InitCamera(3);
	InitCamera(2);
	InitCamera(1);
	InitCamera(0);

	nnInitLight();
	InitLight(0);

	gaNnViewRotInvertMtx.fill(0);
	constexpr u32 matrixVal = 0x3F800000;

	gaNnViewRotInvertMtx[10] = matrixVal;
	gaNnViewRotInvertMtx[5] = matrixVal;
	gaNnViewRotInvertMtx[0] = matrixVal;

	gaNnViewRotInvertMtx[22] = matrixVal;
	gaNnViewRotInvertMtx[17] = matrixVal;
	gaNnViewRotInvertMtx[12] = matrixVal;

	gaNnViewRotInvertMtx[34] = matrixVal;
	gaNnViewRotInvertMtx[29] = matrixVal;
	gaNnViewRotInvertMtx[24] = matrixVal;

	gaNnViewRotInvertMtx[46] = matrixVal;
	gaNnViewRotInvertMtx[41] = matrixVal;
	gaNnViewRotInvertMtx[36] = matrixVal;

	std::array<u32,4> struct3{
		lbl_001D78C8.int1,
		lbl_001D78C8.int2,
		lbl_001D78C8.int3,
		lbl_001D78C8.int4
	};

	nnSetClipScreenCoordinates(struct3.data());

	lbl_000B7BFC();
    CreateJapaneseVoiceThread();
	lbl_001514A8();
	lbl_800645A8(&lbl_100066C0, lbl_001D78C8.f1, lbl_001D78C8.f2, lbl_001D78C8.f3, lbl_001D78C8.f1,
				 lbl_001D78C8.f5, lbl_001D78C8.f6);
	lbl_80006778();
	SetTask(&TitleSequence_Task, ObjectGroups::TitleSequence, Object1Sizes::x80);

#if DEBUG_TESTING
	__exception_init();
	DEBUG_Init(GDBSTUB_DEVICE_USB, 1); //0x802ca720
	_break();
#endif

	const u32 boob = 0xB00B;
	for(;;) {
		lbl_80006778();
		lbl_0004F404();
		gNp_TaskDone();
		if (!lbl_8021BB84) {
			lbl_0014C080();
		}
		GX_SetDrawSync(boob);
		gNp_VWaitTrans();
		lbl_000B766C();
		lbl_000375B4();
		ReadAsyncReadFileData(1);
		CustomCodehandler::InvokeInjectedCodes();
		DebugMenuHandler_DisableMusic();
		lbl_8021BB68 += 1;
	}
}
#pragma clang diagnostic pop