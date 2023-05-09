#include "filehandler_dat.hpp"

void LoadDATFileDynamic(const char filename[]) {
    DATFile dat{};
	DATSpecs* datfile = DumpFile(filename, 1);

    strcpy(dat.file, filename);
    dat.spec1 = datfile->spec1;
    dat.spec2 = datfile->spec2;
    dat.soundFileID = nullptr;
    nnFree(datfile);

    LoadCustomDATFile(&dat);
}

ASMUsed void DATFileHandler() {
    LoadDATFileDynamic("SFX.DAT");
}