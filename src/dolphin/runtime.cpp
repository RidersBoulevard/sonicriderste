/** \file runtime.cpp
 * Contains runtime functions from MetroWerks runtime for backwards compatibility.
 */

#include "globalDefs.hpp"
#include "types.hpp"

ASMUsed u32 __cvt_fp2unsigned(f32 f) {
    return static_cast<u32>(f);
}