#pragma once

#include "types.hpp"
#include "globalDefs.hpp"

#include "nn/struct/nns.hpp"
#include "nn/struct/object.hpp"
#include "nn/struct/motion.hpp"
#include "nn/struct/node.hpp"

using NNS_MatMotObj = NNS_Object;
using NNS_MatMotion = NNS_Motion;

ASMDefined {

/**
* Equivalent to both nnDrawObjectExt() and nnDrawObjectLtd(), although you can use this function to also draw in "mirrored mode".
* Instead of passing in a NNS_Object, you pass in the NNS_MatMotObj equivalent (all material motion calculations are done on that object).
*
* @param matMotObj The material motion object to draw.
* @param mtxPal The matrix palette to use.
* @param nodeStat The node status to use.
* @param alwaysMSBSet Uncertain, but this argument always has the 31st bit set.
* @param drawFlags Additional flags for drawing. Can also specify "mirrored mode".
*/
void nnDrawMaterialMotionObject(NNS_MatMotObj *matMotObj, NNS_Matrix *mtxPal, NNS_NodeStatus *nodeStat, u32 alwaysMSBSet, u32 drawFlags);

/**
 * Sets the proper animation values of a NNS_MatMotObj on the given frame.
 * Call this function before drawing your NNS_MatMotObj.
 * Usually, a frame in the game advances by 0.5f.
 *
 * @param matMotObj The material motion object.
 * @param object The object.
 * @param matMotion The material motion.
 * @param currentFrame The current frame in the animation playback.
 */
void nnCalcMaterialMotion(NNS_MatMotObj *matMotObj, NNS_Object *object, NNS_MatMotion *matMotion, f32 currentFrame);

/**
 * Instantiates a copy of the specified NNS_Object in the form of a NNS_MatMotObj.
 * This is used for objects that have material animations applied to them, so that it doesn't alter the underlying object.
 *
 * @param matMotObj The material motion object to instantiate.
 * @param object The object that'll be using the material motions.
 * @param matMotion The material motion.
 */
void nnInitMaterialMotionObject(NNS_MatMotObj *matMotObj, NNS_Object *object, NNS_MatMotion *matMotion);

/**
 * Calculates the required size in bytes to instantiate a NNS_MatMotObj of the given NNS_Object.
 * This function is used to allocate memory for a NNS_MatMotObj,
 * where you may set it up with nnInitMaterialMotionObject() afterwards.
 *
 * @param object The object that'll be using the material motions.
 * @param matMotion The material motion.
 * @return The size required for the NNS_MatMotObj in bytes.
 */
u32 nnCalcMaterialMotionObjectBufferSize(NNS_Object *object, NNS_MatMotion *matMotion);

}