#include "customcodehandler.hpp"

void CustomCodehandlerUpdate(const u32 codetype, const u32 bssType, InjectorPtr function) {
	if(codetype > 1) {
		return;
	}

	if(bssType > 4) {
		return;
	}

	const u32 codeIndex = CustomCodehandler::FetchInjectedCode(function, codetype);
	if(codeIndex == ~0U) {
		CustomCodehandler::InitializeInjectedCode(function, bssType, codetype);
	}
}

void CustomCodehandler_Player(Player *player) {
	CustomCodehandler::InvokeInjectedCodes(player);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-non-const-global-variables"
namespace CustomCodehandler {
	constexpr auto bssMaxSize = 0x400;
	static std::array<u32, 2> injectedCodeAmount{};
	static m2darray<char, 4, bssMaxSize> bss{};
	static std::array<u16, 4> bssFreeSpace{bssMaxSize, bssMaxSize, bssMaxSize, bssMaxSize};
	static std::array<u16, 4> bssSlotAmount{0, 0, 0, 0};

	template<typename Injector>
	struct InjectInfo {
		Injector function;
		void *bssPointer;
	};

	// Maybe this would be better using std::variant?
	std::array<InjectInfo<InjectorPtr>, 64> injectedCodes;            // NOLINT(readability-magic-numbers)
	std::array<InjectInfo<InjectorPlayerPtr>, 64> injectedPlayerCodes;// NOLINT(readability-magic-numbers)

	template<typename Injector>
	void InitializeInjectedCode(InjectInfo<Injector> &info, Injector function, const u32 &bssType) {
		//InjectInfo *info = &(injectedCodes[codetype][injectedCodeAmount[codetype]]);

		info.function = function;
		info.bssPointer = nullptr;

		if(bssType == 0) {
			// no bss allocation
			return;
		}

		const u32 bssSlot = bssSlotAmount[bssType - 1];
		s32 bssSpace;// NOLINT(cppcoreguidelines-init-variables)
		switch(bssType) {
			case TenBytesBSS:
				bssSpace = bssFreeSpace[bssType - 1] - 0x10;// NOLINT(readability-magic-numbers)
				if(bssSpace >= 0) {
					// if there's free bss space left, allocate it
					info.bssPointer = &bss[bssType - 1][0x10 * bssSlot];// NOLINT(readability-magic-numbers)

					bssSlotAmount[bssType - 1] += 1;
					bssFreeSpace[bssType - 1] = bssSpace;
				}

				break;

			case TwentyBytesBSS:
				bssSpace = bssFreeSpace[bssType - 1] - 0x20;// NOLINT(readability-magic-numbers)
				if(bssSpace >= 0) {
					// if there's free bss space left, allocate it
					info.bssPointer = &bss[bssType - 1][0x20 * bssSlot];// NOLINT(readability-magic-numbers)

					bssSlotAmount[bssType - 1] += 1;
					bssFreeSpace[bssType - 1] = bssSpace;
				}

				break;

			case FortyBytesBSS:
				bssSpace = bssFreeSpace[bssType - 1] - 0x40;// NOLINT(readability-magic-numbers)
				if(bssSpace >= 0) {
					// if there's free bss space left, allocate it
					info.bssPointer = &bss[bssType - 1][0x40 * bssSlot];// NOLINT(readability-magic-numbers)

					bssSlotAmount[bssType - 1] += 1;
					bssFreeSpace[bssType - 1] = bssSpace;
				}

				break;

			case EightyBytesBSS:
				bssSpace = bssFreeSpace[bssType - 1] - 0x80;// NOLINT(readability-magic-numbers)
				if(bssSpace >= 0) {
					// if there's free bss space left, allocate it
					info.bssPointer = &bss[bssType - 1][0x80 * bssSlot];// NOLINT(readability-magic-numbers)

					bssSlotAmount[bssType - 1] += 1;
					bssFreeSpace[bssType - 1] = bssSpace;
				}
				break;
			default:
				break;
		}
	}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
	void InitializeInjectedCode(InjectorPtr function, const u32 &bssType, const u32 &codetype) {
		injectedCodeAmount[codetype]++;
		if(codetype == EveryFrameInjectedCode) {
			InitializeInjectedCode(injectedCodes[injectedCodeAmount[codetype]], function, bssType);
		} else {
			InitializeInjectedCode(injectedPlayerCodes[injectedCodeAmount[codetype]], reinterpret_cast<InjectorPlayerPtr>(function), bssType);
		}
	}

	u32 FetchInjectedCode(InjectorPtr function, const u32 &codetype) {
		return codetype == EveryFrameInjectedCode
		               ? FetchInjectedCode(function)
		               : FetchInjectedCode(reinterpret_cast<InjectorPlayerPtr>(function));
	}
#pragma GCC diagnostic pop

	u32 FetchInjectedCode(InjectorPtr function) {
		for(u32 i = 0; i < injectedCodeAmount[EveryFrameInjectedCode]; i++) {
			if(injectedCodes[i].function == function) {
				return i;
			}
		}

		return ~0U;
	}

	u32 FetchInjectedCode(InjectorPlayerPtr function) {
		for(u32 i = 0; i < injectedCodeAmount[InGameInjectedCode]; i++) {
			if(injectedPlayerCodes[i].function == function) {
				return i;
			}
		}

		return ~0U;
	}

	void InvokeInjectedCodes() {
		const std::span injectedCodeList = std::span(injectedCodes).subspan(0, injectedCodeAmount[EveryFrameInjectedCode]);
		for(const auto &injectedCode: injectedCodeList) {
			reinterpret_cast<InjectorPtr>(injectedCode.function)(
			        injectedCode.bssPointer,
			        reinterpret_cast<void *>(&CustomCodehandler_Retrieve));
		}
	}

	void InvokeInjectedCodes(Player *player) {
		const std::span injectedCodeList = std::span(injectedPlayerCodes).subspan(0, injectedCodeAmount[InGameInjectedCode]);
		for(const auto &injectedCode: injectedCodeList) {
			reinterpret_cast<InjectorPlayerPtr>(injectedCode.function)(
			        injectedCode.bssPointer,
			        reinterpret_cast<void *>(&CustomCodehandler_Retrieve),
			        player);
		}
	}
}// namespace CustomCodehandler
#pragma clang diagnostic pop