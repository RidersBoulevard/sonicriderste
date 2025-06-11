#pragma once

#include "globalDefs.hpp"

struct TexInfo {
    u32 unk0;
    char *filename;
    u16 unk8;
    u16 unkA;
    u32 unkC;
    u32 unk10;
};
static_assert(sizeof(TexInfo) == 0x14);

// TODO: document this struct
struct TexStruct {
    fillerData<0x40> data;
};

struct TexStructHeader {
    u32 texNum;
    TexStruct *texStart;
};

constexpr TexInfo defaultTexInfo = {
        0, nullptr, 1, 1, 0, 0
};

struct GVRTextureArchive {
    u16 texNum;
    u16 flag; // either 0 or 1
    u32 offsetTable[];
};

struct GVRTexture {
    std::array<char, 4> gcixMagic;
    fillerData<0xC> data1;
    std::array<char, 4> gvrtMagic;
    u32 texSize; ///< (little endian) starting from the currently read offset, so this variable is 0x18 bytes less than the entire file
    fillerData<0x4> data2;
    u16 width;
    u16 height;
};