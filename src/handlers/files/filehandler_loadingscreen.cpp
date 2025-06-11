#include "filehandler_dat.hpp"
#include "main.hpp"
#include "riders/gamemode.hpp"

#include <span>

std::array<void*, 2> CustomTextArchiveFile;

ASMDefined {
    extern void* lbl_801AFC64; /*asm*/
    void* texList_NormalSuperTails;
    void* texList_NormalSuperNeoMetal;
}

void init_text_table(std::span<u32> table) {
	u32 *textptr = &table[3];
    u32 *startptr = table.data();

	const u32 &textAmount = table[2];
	for(u32 i = 0; i < textAmount; i++){
		*textptr = reinterpret_cast<u32>(startptr) + *textptr;
		textptr++;
	}
}

ASMUsed void LoadingScreenFileHandler() {
    u32 offset = 0;

    SetArchiveBinary(DumpPackManFile("TSGC"), 0, 0); // Custom Archive File

    GetSet_Tex(DumpFile("PTT"), &offset, &texList_NormalSuperTails, 0); // Super Tails Textures

    GetSet_Tex(DumpFile("P4T"), &offset, &texList_NormalSuperNeoMetal, 0); // Neo Metal Textures

    if (CurrentGameMode == FreeRace || CurrentGameMode == WorldGrandPrix){
        SetArchiveBinary(DumpPackManFile("SVB"), 0, 0);
        SetArchiveBinary(DumpPackManFile("SVBE"), 0, 0);
    }
}