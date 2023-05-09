#include "context.hpp"

ASMUsed u32 HUD_GearsCanHoldMoreRings(Player *player) {
    switch (player->extremeGear) {
        case Gambler:
            return 150;
        case ChaosEmerald:
            if (player->character != Tails) break;
			[[fallthrough]]; // todo: Is this supposed to fall through?
        case CoverF:
            return 200;
        case CoverS:
            return 130;
        case CoverP:
            return 300;
		default:
			break;
    }
	return 100;
}