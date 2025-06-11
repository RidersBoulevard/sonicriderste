#pragma once

#include "types.hpp"
#include "globalDefs.hpp"

#include "nn/struct/nns.hpp"
#include "nn/struct/camera.hpp"

ASMDefined {

void nnMakeTargetUpVectorCameraViewMatrix(NNS_Matrix *mtx, const NNS_CameraTargetUpVector *cam);

}