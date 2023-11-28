// THIS FUNCTION IS ALMOST MATCHED
// it's not a perfect match but the functionality is exactly the same

#include "prolog.hpp"
#include "customcodehandler.hpp"
#include "menu/debugmenu/debugmenu.hpp"
#include "menu/debugmenu/debugmenu_handlers.hpp"

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
	lbl_00175C7C();

	InitCamPalette(0);

	InitCamera(4);
	InitCamera(3);
	InitCamera(2);
	InitCamera(1);
	InitCamera(0);

	nnInitLight();
	InitLight(0);

	gaNnViewRotInvertMtx.fill(0);

	gaNnViewRotInvertMtx[10] = 0x3F800000;
	gaNnViewRotInvertMtx[5] = 0x3F800000;
	gaNnViewRotInvertMtx[0] = 0x3F800000;

	gaNnViewRotInvertMtx[22] = 0x3F800000;
	gaNnViewRotInvertMtx[17] = 0x3F800000;
	gaNnViewRotInvertMtx[12] = 0x3F800000;

	gaNnViewRotInvertMtx[34] = 0x3F800000;
	gaNnViewRotInvertMtx[29] = 0x3F800000;
	gaNnViewRotInvertMtx[24] = 0x3F800000;

	gaNnViewRotInvertMtx[46] = 0x3F800000;
	gaNnViewRotInvertMtx[41] = 0x3F800000;
	gaNnViewRotInvertMtx[36] = 0x3F800000;

	std::array<u32,4> struct3{
		(&lbl_001D78C8)->int1,
		(&lbl_001D78C8)->int2,
		(&lbl_001D78C8)->int3,
		(&lbl_001D78C8)->int4
	};

	nnSetClipScreenCoordinates(struct3.data());

	lbl_000B7BFC();
	lbl_001514A8();
	lbl_800645A8(&lbl_100066C0, (&lbl_001D78C8)->f1, (&lbl_001D78C8)->f2, (&lbl_001D78C8)->f3, (&lbl_001D78C8)->f1,
				 (&lbl_001D78C8)->f5, (&lbl_001D78C8)->f6);
	lbl_80006778();
	SetTask(&TitleSequence_Task, ObjectGroups::TitleSequence, 2);

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
		func_800303B0(boob);
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