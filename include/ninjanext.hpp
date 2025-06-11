#pragma once

#include "types.hpp"
#include "globalDefs.hpp"
#include "containers/vector3.hpp"
#include <cmath>

using NNS_SubMotion = void;
struct NNS_Motion {
    u32 type;
    f32 startFrame;
    f32 endFrame;
    u32 subMotionCount;
    NNS_SubMotion *subMotions;
    f32 framerate;
};

struct NNS_Object {
    Vector3F 		center;					///< Center position
    f32				radius;					///< Radius
    s32				nMaterial;				///< Number of material
    void		    *matPtrList;			///< Material pointer list
    s32				nVtxList;				///< Number of vertex list
    void		    *vtxListPtrList;		///< Vertexlist pointer list
    s32				nPrimList;				///< Number of primitive list
    void 	        *primListPtrList;		///< Primitivelist pointer list
    s32				nNode;					///< Number of nodes
    s32				maxNodeDepth;			///< Maximum node depth
    void    		*nodeList;				///< Node list
    s32				nMtxPal;				///< Number of matrix pallette
    s32				nSubobj;				///< Number of subobjects
    void    		*subobjList;			///< Subobject list
    s32				nTex;					///< Number of textures
};

// TODO: Map out these structs at some point
using NNS_MatMotObj = NNS_Object;
using NNS_MatMotion = NNS_Motion;
using NNS_TexList = void;
using NNS_MtxList = void;
using NNS_ObjectTrsList = void;
using NNS_MtxPal = void;
using NNS_UnitMtxPal = void;
using NNS_NodeStat = void;

using Angle32 = s32;
using Angle16 = s16;

namespace nn {
    [[nodiscard]] constexpr f32 BAMToFloat(const Angle32 bam) { return static_cast<f32>(bam) * (180.f / 32767.f); }

    [[nodiscard]] constexpr Angle32 FloatToBAM(const f32 val) {
        return static_cast<Angle32>(roundf(val * (32767.f / 180.f)));
    }
}

ASMDefined {

/**
 * Sets the texture list for a NNS_Object to refer to. Call this function before drawing your object.
 *
 * @param texList The texture list to use.
 * @return 1 if successful, -1 if unsuccessful. Usually this return value is never used.
 */
s32 nnSetTextureList(NNS_TexList *texList);

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
void nnDrawObjectExt(NNS_Object *object, NNS_MtxPal *mtxPal, NNS_NodeStat *nodeStat, u32 alwaysMSBSet, u32 drawFlags);

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
void nnDrawObjectLtd(NNS_Object *object, NNS_MtxPal *mtxPal, NNS_NodeStat *nodeStat, u32 alwaysMSBSet, u32 drawFlags);

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
void nnDrawMaterialMotionObject(NNS_MatMotObj *matMotObj, NNS_MtxPal *mtxPal, NNS_NodeStat *nodeStat, u32 alwaysMSBSet, u32 drawFlags);

void nnCalcMatrixListTRSList(NNS_MtxList *, NNS_Object *, NNS_ObjectTrsList *, void *);
void nnCalcMatrixPaletteMatrixList(NNS_UnitMtxPal *, NNS_Object *, NNS_MtxList *, u32);

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
void nnCalcMatrixPaletteMotion(NNS_MtxPal *mtxpal, NNS_NodeStat *nodestatlist, const NNS_Object *obj, const NNS_Motion *mot, f32 frame, const Matrix3x3F *basemtx, void *mstk, u32 flag);

/**
 * Sets up the given node status list.
 * Always call this function before drawing an object (and before using it in any other consequential functions).
 *
 * @param nodestatlist The node status list to set up.
 * @param num The number of nodes.
 * @param flag Node status flags. Leave at 0 usually.
 */
void nnSetUpNodeStatusList(NNS_NodeStat *nodestatlist, s32 num, u32 flag);

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

void nnMultiplyQuaternion(void *, void *, void *);
void nnMakeRotateZXYQuaternion(void *, Angle32, Angle32, Angle32);
void nnMultiplyMatrix(Matrix3x3F *A, Matrix3x3F *B, Matrix3x3F &out);
void nnMakeTranslateMatrix(Matrix3x3F *M, f32 x, f32 y, f32 z);
void nnMakeRotateXYZMatrix(Matrix3x3F *dst, Angle32 ax, Angle32 ay, Angle32 az);
void nnTranslateMatrix(Matrix3x3F *dst, Matrix3x3F *src, f32 x, f32 y, f32 z);
void nnRotateXMatrix(Matrix3x3F *dst, const Matrix3x3F *src, Angle32 ax);

void nnInitLight();
void nnSetClipScreenCoordinates(u32 []);

/**
 * Instantiates a copy of the specified NNS_Object in the form of a NNS_MatMotObj.
 * This is used for objects that have material animations applied to them, so that it doesn't alter the underlying object.
 *
 * @param matMotObj The material motion object to instantiate.
 * @param object The object that'll be using the material motions.
 * @param matMotion The material motion.
 */
void nnInitMaterialMotionObject(NNS_MatMotObj *matMotObj, NNS_Object *object, NNS_MatMotion *matMotion);

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

ASMDefined u32 utilTexLoadFileOne(TexStruct *tex, const TexInfo *info = &defaultTexInfo, const void *gvrTexture = nullptr);

/**
 * Calculates the required size in bytes to instantiate a NNS_MatMotObj of the given NNS_Object.
 * This function is used to allocate memory for a NNS_MatMotObj, where you may set it up with nnInitMaterialMotionObject() afterwards.
 *
 * @param object The object that'll be using the material motions.
 * @param matMotion The material motion.
 * @return The size required for the NNS_MatMotObj in bytes.
 */
u32 nnCalcMaterialMotionObjectBufferSize(NNS_Object *object, NNS_MatMotion *matMotion);

/// sets up tex structs and saves a pointer to it
void nnSetUpTexlist(void *savePtr, u32 texNum, TexStructHeader *texStructs);

/// estimates tex list size
u32 nnEstimateTexlistSize(u32 texNu);
}