#pragma once

#include "types.hpp"
#include <ogc/gx.h>
#include "nns.hpp"

enum class NNE_BlendMode : u32 {
    NONE = GX_BM_NONE,
    BLEND = GX_BM_BLEND,
    LOGIC = GX_BM_LOGIC,
    SUBTRACT = GX_BM_SUBTRACT,
};

enum class NNE_BlendFactor : u32 {
    ZERO = GX_BL_ZERO,
    ONE = GX_BL_ONE,
    SRCCLR = GX_BL_SRCCLR,
    INVSRCCLR = GX_BL_INVSRCCLR,
    SRCALPHA = GX_BL_SRCALPHA,
    INVSRCALPHA = GX_BL_INVSRCALPHA,
    DSTALPHA = GX_BL_DSTALPHA,
    INVDSTALPHA = GX_BL_INVDSTALPHA,

    DSTCLR = GX_BL_SRCCLR,
    INVDSTCLR = GX_BL_INVSRCCLR,
};

enum class NNE_LogicOp : u32 {
    CLEAR = GX_LO_CLEAR,
    AND = GX_LO_AND,
    REVAND = GX_LO_REVAND,
    COPY = GX_LO_COPY,
    INVAND = GX_LO_INVAND,
    NOOP = GX_LO_NOOP,
    XOR = GX_LO_XOR,
    OR = GX_LO_OR,
    NOR = GX_LO_NOR,
    EQUIV = GX_LO_EQUIV,
    INV = GX_LO_INV,
    REVOR = GX_LO_REVOR,
    INVCOPY = GX_LO_INVCOPY,
    INVOR = GX_LO_INVOR,
    NAND = GX_LO_NAND,
    SET = GX_LO_SET,
};

enum class NNE_CmpFunc : u32 {
    // it's in a different order cuz sega
    NEVER = GX_NEVER,
    LESS = GX_LESS,
    LEQUAL = GX_LEQUAL,
    EQUAL = GX_EQUAL,
    NEQUAL = GX_NEQUAL,
    GEQUAL = GX_GEQUAL,
    GREATER = GX_GREATER,
    ALWAYS = GX_ALWAYS,
};

enum class NNE_AlphaOp : u32 {
    AND = GX_AOP_AND,
    OR = GX_AOP_OR,
    XOR = GX_AOP_XOR,
    XNOR = GX_AOP_XNOR,
};

struct NNS_Material_Color {
    NNS_RGB diffuse;
    f32 alpha;
    NNS_RGB ambient;
    NNS_RGB specular;
    f32 specularLevel;
    f32 specularGloss;
};
static_assert(sizeof(NNS_Material_Color) == 0x30);

struct NNS_Material_Logic {
    struct {
        NNE_BlendMode type;
        NNE_BlendFactor srcFactor;
        NNE_BlendFactor dstFactor;
        NNE_LogicOp op;
    } blendMode; // gets passed to GX_SetBlendMode

    struct {
        NNE_CmpFunc func;
    } zMode; // gets passed to GX_SetZMode

    struct {
        u8 ref0;
        u8 ref1;
        // 2 bytes of padding should be automatically added here by the compiler
        NNE_CmpFunc comp0;
        NNE_CmpFunc comp1;
        NNE_AlphaOp aop;
    } alphaCompare; // gets passed to GX_SetAlphaCompare

    fillerData<4> _;
};
static_assert(sizeof(NNS_Material_Logic) == 0x28);

struct NNS_Material_Desc {
    u32 flags;
    NNS_Material_Color color;
    NNS_Material_Logic logic;
    // ...texmap descriptors would follow in an array...
    // see NNS_Material_TexMap_Desc
};
static_assert(sizeof(NNS_Material_Desc) == 0x5C);

struct NNS_Material {
    u16 textureCount; // in a weird format
    u16 unknown;
    NNS_Material_Desc *materialData;
};
static_assert(sizeof(NNS_Material) == 0x8);

struct [[maybe_unused]] NNS_Material_TexMap_Desc {
    u32 texMapType;
    u32 texIdx;
    NNS_TexCoord offset;
    f32 blend; ///< Texture blend ratio
};
static_assert(sizeof(NNS_Material_TexMap_Desc) == 0x14);
