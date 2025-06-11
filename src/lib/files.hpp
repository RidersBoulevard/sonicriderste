#pragma once

#include "globalDefs.hpp"
#include "types.hpp"
#include <memory>

ASMUsed void* DumpFile(const char* path, const bool unknownAlwaysTrue = true);

template<typename T>
[[nodiscard]] inline std::unique_ptr<T> DumpFile(const char *filename, bool unknownAlwaysTrue = true){
    T* ptr = static_cast<T*>(DumpFile(filename, unknownAlwaysTrue));
    return std::unique_ptr<T>(ptr);
}

extern bool lbl_8021BB62;
extern bool lbl_8021BB61;
extern bool lbl_8021BB9C;
extern f32 lbl_8021BB98;

ASMDefined void func_800090D8(u32 state, u32, u32);
ASMDefined void SNDF_SendSystemControl(u32, u32, u32);
ASMDefined void SNDFGC_Se3dSendSystemControl(u32, u32, u32);

ASMDefined char* DumpPackManFile(const char* filename, bool lowHeap = false);
ASMDefined void SetArchiveBinary(const char*, u32, u32);
ASMDefined void GetSet_Tex(void *packmanStart, u32 *offsetFromStart, void *saveptr, void *mallocHi);
ASMDefined void PackMan_SetupRenderedText(void *packmanStart, u32 *offsetFromStart, void *saveptr);
