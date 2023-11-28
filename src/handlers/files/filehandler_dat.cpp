#include "filehandler_dat.hpp"

ASMUsed void LoadDATFileDynamic(const char filename[]) {
	DATSpecs *datfile = DumpFile(filename, 1);
	DATFile dat{filename, datfile->spec1, datfile->spec2};
	delete datfile;

	LoadCustomDATFile(dat);
}

ASMUsed void DATFileHandler() {
	LoadDATFileDynamic("SFX.DAT");
}