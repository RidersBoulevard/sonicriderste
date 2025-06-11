#pragma once

#include "types.hpp"
#include "globalDefs.hpp"

#include "nn/struct/nns.hpp"
#include "nn/struct/object.hpp"
#include "nn/struct/node.hpp"

ASMDefined {

void nnCalcMatrixListTRSList(NNS_Matrix *, NNS_Object *, NNS_TRS *, NNS_Matrix *transform);
void nnCalcMatrixPaletteMatrixList(NNS_Matrix *mtxPal, NNS_Object *, NNS_Matrix *, u32);

/**
 * Calculates a NNS_Object's bone matrices depending on the given motion and the given frame.
 * Use this function before drawing the model, so that upon drawing, it can use the animated matrices.
 *
 * @param mtxpal The matrix palette to apply the node motions to.
 * @param nodestatlist The node status list to apply the node motions to.
 * @param obj The object to apply the node motions to.
 * @param mot The node motion to apply.
 * @param frame The frame to calculate in the node motion.
 * @param basemtx Base transform, where the motion calculations get applied on top of.
 * @param mstk Matrix stack. The game has one of these already set up, use gsNnMtxStack.
 * @param flag Set node status flags. Leave at 0 usually.
 */
void nnCalcMatrixPaletteMotion(NNS_Matrix *mtxpal, NNS_NodeStatus *nodestatlist, const NNS_Object *obj, const NNS_Motion *mot, f32 frame, const Matrix3x4F *basemtx, void *mstk, u32 flag);

/**
 * Sets up the given node status list.
 * Always call this function before drawing an object (and before using it in any other consequential functions).
 *
 * @param nodestatlist The node status list to set up.
 * @param num The number of nodes.
 * @param flag Node status flags. Leave at 0 usually.
 */
void nnSetUpNodeStatusList(NNS_NodeStatus *nodestatlist, s32 num, u32 flag);

}
