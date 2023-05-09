// THIS FUNCTION IS ALMOST MATCHED
// it's not a perfect match but the functionality is exactly the same

#include "prolog.hpp"
#include "customcodehandler.hpp"
#include "menu/debugmenu/debugmenu.hpp"
#include "menu/debugmenu/debugmenu_handlers.hpp"
#include "macros.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-magic-numbers"
#pragma ide diagnostic ignored "readability-implicit-bool-conversion"
global [[noreturn]] void _prolog() {
	//struct lbl_001D78C8 *struct1;
	//struct1 = &lbl_001D78C8;

    /*
	u32 *customCodehandlerHookAddress = reinterpret_cast<u32 *>(0x80001f88);
	u32 offset = reinterpret_cast<u32>(&CustomCodehandlerHook) - reinterpret_cast<u32>(customCodehandlerHookAddress);
	*customCodehandlerHookAddress = (offset & 0x03FFFFFC) | 0x48000001;

	asm(" clrlwi r4, %[HookAddr], 0x10\n"
		" rlwinm r3, %[HookAddr], 0, 0, 0xF\n"
		" dcbf r3, r4\n"
		" sync\n"
		" icbi r3, r4\n"
		" isync"
			:
			: [HookAddr] "r"(customCodehandlerHookAddress)
			: "r3", "r4");
     */

	DebugMenu_Data.toggledPageOptions = DEBUGMENU_DEFAULTPAGE1OPTIONS;

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

	gaNnViewRotInvertMtx[10] = 0x3F800000;
	gaNnViewRotInvertMtx[5] = 0x3F800000;
	gaNnViewRotInvertMtx[0] = 0x3F800000;

	gaNnViewRotInvertMtx[11] = 0;
	gaNnViewRotInvertMtx[9] = 0;
	gaNnViewRotInvertMtx[8] = 0;
	gaNnViewRotInvertMtx[7] = 0;
	gaNnViewRotInvertMtx[6] = 0;
	gaNnViewRotInvertMtx[4] = 0;
	gaNnViewRotInvertMtx[3] = 0;
	gaNnViewRotInvertMtx[2] = 0;
	gaNnViewRotInvertMtx[1] = 0;

	gaNnViewRotInvertMtx[22] = 0x3F800000;
	gaNnViewRotInvertMtx[17] = 0x3F800000;
	gaNnViewRotInvertMtx[12] = 0x3F800000;

	gaNnViewRotInvertMtx[23] = 0;
	gaNnViewRotInvertMtx[21] = 0;
	gaNnViewRotInvertMtx[20] = 0;
	gaNnViewRotInvertMtx[19] = 0;
	gaNnViewRotInvertMtx[18] = 0;
	gaNnViewRotInvertMtx[16] = 0;
	gaNnViewRotInvertMtx[15] = 0;
	gaNnViewRotInvertMtx[14] = 0;
	gaNnViewRotInvertMtx[13] = 0;

	gaNnViewRotInvertMtx[34] = 0x3F800000;
	gaNnViewRotInvertMtx[29] = 0x3F800000;
	gaNnViewRotInvertMtx[24] = 0x3F800000;

	gaNnViewRotInvertMtx[35] = 0;
	gaNnViewRotInvertMtx[33] = 0;
	gaNnViewRotInvertMtx[32] = 0;
	gaNnViewRotInvertMtx[31] = 0;
	gaNnViewRotInvertMtx[30] = 0;
	gaNnViewRotInvertMtx[28] = 0;
	gaNnViewRotInvertMtx[27] = 0;
	gaNnViewRotInvertMtx[26] = 0;
	gaNnViewRotInvertMtx[25] = 0;

	gaNnViewRotInvertMtx[46] = 0x3F800000;
	gaNnViewRotInvertMtx[41] = 0x3F800000;
	gaNnViewRotInvertMtx[36] = 0x3F800000;

	gaNnViewRotInvertMtx[47] = 0;
	gaNnViewRotInvertMtx[45] = 0;
	gaNnViewRotInvertMtx[44] = 0;
	gaNnViewRotInvertMtx[43] = 0;
	gaNnViewRotInvertMtx[42] = 0;
	gaNnViewRotInvertMtx[40] = 0;
	gaNnViewRotInvertMtx[39] = 0;
	gaNnViewRotInvertMtx[38] = 0;
	gaNnViewRotInvertMtx[37] = 0;

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
	SetTask(&TitleSequence_Task, 2, 2);

	const u32 boob = 0xB00B;
	for(;;) {
		lbl_80006778();
		lbl_0004F404();
		gNp_TaskDone(CurrentStage, players.data(), playerCameraStruct.data());
		if (!lbl_8021BB84) {
			lbl_0014C080();
		}
		func_800303B0(boob);
		gNp_VWaitTrans();
		lbl_000B766C();
		lbl_000375B4();
		ReadAsyncReadFileData(1);
		CustomCodehandler::InvokeInjectedCodes();
		DebugMenuHandler_DisableMusic();lbl_8021BB68 += 1;
	}
}
#pragma clang diagnostic pop