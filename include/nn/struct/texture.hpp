#pragma once

#include "gvr_texture.hpp"

enum class NNF_TexFlag : u32 {
    ALLOCATE = 1 << 0
};

struct NNS_TexInfo {
    GVRTexture gvrObj; ///< GVR Object
    void *mainMemory; ///< Texture address
    u32	globalIndex; ///< GlobalIndex number
    u32	bank; ///< bank number
    NNF_TexFlag flag; ///< Load Flag
};

struct NNS_TexList {
    s32 texCount; ///< Number of textures
    NNS_TexInfo *texInfoList; ///< Texture information list
};