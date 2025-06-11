#pragma once

#include "types.hpp"

constexpr auto CARD_FILENAME_MAX = 32;
constexpr auto CARD_ICON_MAX = 8;

struct CARDStat {
    char fileName[CARD_FILENAME_MAX];
    u32 length;
    u32 time;
    u8 gameName[4];
    u8 company[2];

    u8 bannerFormat;
    u8 __padding;
    u32 iconAddr;
    u16 iconFormat;
    u16 iconSpeed;
    u32 commentAddr;

    u32 offsetBanner;
    u32 offsetBannerTlut;
    u32 offsetIcon[CARD_ICON_MAX];
    u32 offsetIconTlut;
    u32 offsetData;
};
