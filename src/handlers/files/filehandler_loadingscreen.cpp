#include "context.hpp"
#include "filehandler_dat.hpp"

void* CustomTextArchiveFile[2];

global {
    extern void* lbl_801AFC64; /*asm*/
    void* texList_NormalSuperTails;
    void* texList_NormalSuperNeoMetal;
}

void init_text_table(u32 table[]) {
    u32 textAmount = table[2];
    u32 i=0;
    u32 *textptr = &table[3];
    u32 *startptr = &table[0];

    while (i < textAmount)
    {
        *textptr = reinterpret_cast<u32>(startptr) + *textptr;
        textptr++;
        i++;
    }
}

ASMUsed void LoadingScreenFileHandler() {
    /*
    void *file1,*file2,*file3,*file4,*file5,*file6;
    u32 a = 0;
    u32 *offset = &a;

    file1 = DumpFile("TSIG", 1);
    init_text_table(file1);    
    
    //file2 = DumpFile("TSIGME", 1); // (UNUSED) used to contain a template of text texture struct
    
    file3 = DumpFile("TSIGMT", 1); // text texture archive
    SetupGVRTextureArchive(file3, offset, &lbl_801AFC64, 0);
    
    //file4 = DumpFile("TSIGTEX", 1); // custom textures

    CustomTextArchiveFile[0] = file1;
    CustomTextArchiveFile[1] = (u32 *)file1 + 3; // increment by 0xC
    //CustomTextArchiveFile[2] = file3;
    //CustomTextArchiveFile[3] = file3;
    //CustomTextArchiveFile[4] = file4;
    */

    void *customArchiveFile, *file2, *file3, *superTailsTextures, *neoMetalTextures;
    u32 offset = 0;

    customArchiveFile = DumpPackManFile("TSGC", 0);
    SetArchiveBinary((char *)customArchiveFile, 0, 0);

    superTailsTextures = DumpFile("PTT", 1);
    SetupGVRTextureArchive(superTailsTextures, &offset, &texList_NormalSuperTails, 0);

    neoMetalTextures = DumpFile("P4T", 1);
    SetupGVRTextureArchive(neoMetalTextures, &offset, &texList_NormalSuperNeoMetal, 0);

    if (CurrentGameMode == FreeRace || CurrentGameMode == WorldGrandPrix)
    {
        file2 = DumpPackManFile("SVB", 0);
        SetArchiveBinary((char *)file2, 0, 0);
        file3 = DumpPackManFile("SVBE", 0);
        SetArchiveBinary((char *)file3, 0, 0);
    }
}