#pragma once

#include "types.hpp"
#include "containers/vector3.hpp"
#include "material.hpp"
#include <span>

using NNS_Node = void;
using NNS_SubObj = void;

struct NNS_Object {
    Vector3F 		center;					///< Center position
    f32				radius;					///< Radius
    s32				nMaterial;				///< Number of material
    NNS_Material    *matPtrList;			///< Material pointer list
    s32				nVtxList;				///< Number of vertex list
    void		    *vtxListPtrList;		///< Vertexlist pointer list
    s32				nPrimList;				///< Number of primitive list
    void 	        *primListPtrList;		///< Primitivelist pointer list
    s32				nNode;					///< Number of nodes
    s32				maxNodeDepth;			///< Maximum node depth
    NNS_Node		*nodeList;				///< Node list
    s32				nMtxPal;				///< Number of matrix pallette
    s32				nSubobj;				///< Number of subobjects
    NNS_SubObj		*subobjList;			///< Subobject list
    s32				nTex;					///< Number of textures

    template <class Self>
    [[nodiscard]] auto materials(this Self&& obj) {
        return std::span<NNS_Material>(obj.matPtrList, obj.nMaterial);
    }
    /*template <class Self>
    [[nodiscard]] auto vertices(this Self&& obj) {
        using T = decltype(*obj.vtxListPtrList);
        return std::span<T>(obj.vtxListPtrList, obj.nVtxList);
    }
    template <class Self>
    [[nodiscard]] auto primitives(this Self&& obj) {
        using T = decltype(*obj.primListPtrList);
        return std::span<T>(obj.primListPtrList, obj.nPrimList);
    }
    template <class Self>
    [[nodiscard]] auto subobjects(this Self&& obj) {
        using T = decltype(*obj.subobjList);
        return std::span<T>(obj.subobjList, obj.nSubobj);
    }*/
};