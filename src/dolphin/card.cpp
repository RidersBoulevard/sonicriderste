/** \file card.cpp
 * Contains wrappers around memory card functions for backwards compatibility.
 */

#include "card.hpp"

#include <cstring>
#include <ogc/card.h>

#include "globalDefs.hpp"

card_stat newCardStat{};

inline void SetCardStats(card_stat &newStat, const CARDStat* stat) {
    memset(&newStat, 0, sizeof(newStat));

    memcpy(newStat.filename, stat->fileName, CARD_FILENAME_MAX);
    newStat.len = stat->length;
    newStat.time = stat->time;
    memcpy(newStat.gamecode, stat->gameName, 4);
    memcpy(newStat.company, stat->company, 2);

    newStat.banner_fmt = stat->bannerFormat;
    newStat.icon_addr = stat->iconAddr;
    newStat.icon_fmt = stat->iconFormat;
    newStat.iconfmt[0] = stat->iconFormat;
    newStat.icon_speed = stat->iconSpeed;
    newStat.iconspeed[0] = stat->iconSpeed;
    newStat.comment_addr = stat->commentAddr;
}

ASMUsed s32 CARD_SetStatusAsyncWrap(const s32 chan, const s32 fileNo, const CARDStat* stat,
                                    const cardcallback callback) {
    SetCardStats(newCardStat, stat);
    return CARD_SetStatusAsync(chan, fileNo, &newCardStat, callback);
}

ASMUsed s32 CARD_SetStatusWrap(const s32 chan, const s32 fileNo, const CARDStat* stat) {
    SetCardStats(newCardStat, stat);
    return CARD_SetStatus(chan, fileNo, &newCardStat);
}
