#pragma once

#include "types.hpp"
#include "globalDefs.hpp"

#include "nn/struct/texture.hpp"
#include "gvr_texture.hpp"

ASMDefined {

/**
* Sets the texture list for a NNS_Object to refer to. Call this function before drawing your object.
*
* @param texList The texture list to use.
* @return 1 if successful, -1 if unsuccessful.
* Usually this return value is never used.
*/
s32 nnSetTextureList(NNS_TexList *texList);

/**
 * Sets up a texture list to use for any objects requiring one (3D models, drawing 2D elements, etc).
 *
 * After set up, you can use utilTexLoadFileOne() to initialize all the textures into this texture list
 * (this function doesn't know what textures you want to include!).
 *
 * @param savePtr Where it should save a pointer to the initialized texture list.
 * @param texNum The total texture count.
 * @param texStructs A pointer to somewhere on the heap that is large enough in size to initialize this as a texture list.
 * You can get this by malloc'ing with the size value from nnEstimateTexlistSize().
 */
void nnSetUpTexlist(void *savePtr, u32 texNum, TexStructHeader *texStructs);

/**
 * Estimates the size of a texture list based on the given total texture count.
 * Use this in tandem with malloc to get an uninitialized texture list set up.
 * You can set it up fully using nnSetUpTexlist().
 *
 * @param texNum The total texture count.
 * @return The size of the texture list in bytes.
 */
u32 nnEstimateTexlistSize(u32 texNum);

}