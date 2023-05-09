#pragma once

#include "context.hpp"

struct DATSpecs {
    char filler[0x10];
    u32 spec1;
    u32 unknown;
    u32 spec2;
};

global DATSpecs* DumpFile(const char [], u32);

global void LoadDATFileDynamic(const char filename[]);
ASMUsed void DATFileHandler();

extern void* CustomTextArchiveFile[2];