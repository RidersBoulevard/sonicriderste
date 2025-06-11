#pragma once

#include "types.hpp"
#include "globalDefs.hpp"

#include "nn/struct/nns.hpp"
#include "nn/struct/object.hpp"
#include "nn/struct/node.hpp"

enum NNF_COPYOBJ : u32 {
    NND_COPYOBJ_CACHEFLUSH = (1 << 0),    /* for GC */
    NND_COPYOBJ_COMPILE_DISPLAYLIST_GC = (1 << 1)    /* for GC  (No support) */
};

ASMDefined {

/**
* Draws a NNS_Object. Use this function if you're rendering the object in "mirrored mode" using the drawFlags parameter,
* otherwise the object won't draw correctly.
*
* @param object The object to draw.
* @param mtxPal The matrix palette to use.
* @param nodeStat The node status to use.
* @param alwaysMSBSet Uncertain, but this argument always has the 31st bit set.
* @param drawFlags Additional flags for drawing. This is where you supply the "mirrored mode" flag.
*/
void nnDrawObjectExt(NNS_Object *object, NNS_Matrix *mtxPal, NNS_NodeStatus *nodeStat, u32 alwaysMSBSet, u32 drawFlags);

/**
 * Draws a NNS_Object. This should be the go-to generic draw function to use.
 * Refer to nnDrawObjectExt() if you wish to draw this object in "mirrored mode".
 *
 * @param object The object to draw.
 * @param mtxPal The matrix palette to use.
 * @param nodeStat The node status to use.
 * @param alwaysMSBSet Uncertain, but this argument always has the 31st bit set.
 * @param drawFlags Additional flags for drawing.
 */
void nnDrawObjectLtd(NNS_Object *object, NNS_Matrix *mtxPal, NNS_NodeStatus *nodeStat, u32 alwaysMSBSet, u32 drawFlags);

u32 nnCalcObjectSize(const NNS_Object *obj);
u32 nnCopyObject(NNS_Object *dstobj, const NNS_Object *srcobj, NNF_COPYOBJ flag);

}
