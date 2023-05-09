#include "context.hpp"
#include "cosmetics/player/exloads.hpp"

ASMUsed u32 Player_NonLevelupGears(Player *player, u32 specialFlags) {
    //u8 controllerPort = player->input->port;
    EnabledEXLoads exLoads{};
    FetchEnabledEXLoadIDs(player, exLoads);
    if (isSuperCharacter(*player, Tails) || // super tails
		isSuperCharacter(*player, MetalSonic) || // super metal
        player->extremeGear == CoverF ||
        player->extremeGear == CoverS ||
        player->extremeGear == CoverP ||
        exLoads.gearExLoadID == StardustSpeederEXLoad
        // exLoads.gearExLoadID == E99EXLoad ||
        // exLoads.gearExLoadID == TheBeastEXLoad
        )
        specialFlags |= ringGear;
    return specialFlags;
}