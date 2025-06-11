#pragma once

#include "containers/vector3.hpp"

// acronym "TRS" in this case stands for translation, rotation, scale
struct NNS_TRS {
    Vector3F translation;
    Quaternion rotation;
    Vector3F scale;
};
static_assert(sizeof(NNS_TRS) == 0x28);

using NNS_NodeStatus = u32;