#pragma once

#include "types.hpp"
#include "globalDefs.hpp"

enum class NNE_PrimTexBlend {
    MODULATE,
    REPLACE
};

enum class NNE_PrimTexcoord {
    UV,
    ENVIRONMENT
};

enum class NNE_PrimTexWrap {
    REPEAT,
    CLAMP,
};

ASMDefined {
void nnSetPrimitiveTexState( NNE_PrimTexBlend blend, NNE_PrimTexcoord coord,
                             NNE_PrimTexWrap uwrap, NNE_PrimTexWrap vwrap );
}