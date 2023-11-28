#pragma once

#include "customcodehandler_func.hpp"
#include "riders/player.hpp"

#include <array>
#include <cstddef>

using InjectorPtr = void (*)(void *, void *);
using InjectorPlayerPtr = void (*)(void *, void *, Player *);

enum CustomCodehandlerTypes {
	EveryFrameInjectedCode, InGameInjectedCode
};

// these numbers are in hex
enum CustomCodehandlerBSSTypes {
	NoBSS, TenBytesBSS, TwentyBytesBSS, FortyBytesBSS, EightyBytesBSS
};

namespace CustomCodehandler {
	void InitializeInjectedCode(InjectorPtr function, const u32 &bssType, const u32 &codetype);

	u32 FetchInjectedCode(InjectorPtr function, const u32 &codetype);

	u32 FetchInjectedCode(InjectorPtr function);

	u32 FetchInjectedCode(InjectorPlayerPtr function);

	void InvokeInjectedCodes();

	void InvokeInjectedCodes(Player *player);
}

void CustomCodehandler_Player(Player *player);