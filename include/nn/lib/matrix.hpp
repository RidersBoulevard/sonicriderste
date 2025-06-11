#pragma once

#include "types.hpp"
#include "globalDefs.hpp"

#include "nn/struct/nns.hpp"

ASMDefined {

void nnMultiplyQuaternion(void *, void *, void *);
void nnMakeRotateZXYQuaternion(void *, Angle32, Angle32, Angle32);
void nnMultiplyMatrix(NNS_Matrix *A, NNS_Matrix *B, NNS_Matrix &out);
void nnMakeTranslateMatrix(NNS_Matrix *M, f32 x, f32 y, f32 z);
void nnMakePerspectiveMatrix(NNS_Matrix44 *dst, Angle32 fovy, f32 aspect, f32 znear, f32 zfar);
void nnMakeRotateXYZMatrix(Matrix3x4F *dst, Angle32 ax, Angle32 ay, Angle32 az);
void nnTranslateMatrix(Matrix3x4F *dst, Matrix3x4F *src, f32 x, f32 y, f32 z);
void nnRotateXMatrix(Matrix3x4F *dst, const Matrix3x4F *src, Angle32 ax);

}

