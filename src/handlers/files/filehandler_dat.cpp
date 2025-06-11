#include "filehandler_dat.hpp"
#include "lib/sound.hpp"
//#include <stdexcept>

void FreeDynamicDatFileByIndex(u8 index){
	if(index >= LoadedDynamicDATFileCount){
		//throw std::runtime_error("Attempting to free unallocated index");
		return;
	}
	FreeDATFileByIndex(index);
	LoadedDynamicDATFileCount--;
}

ASMUsed void LoadDATFileDynamic(const char filename[]) {
	auto datfile = DumpFile<DATSpecs>(filename);
	if(datfile){
		DATFile dat{filename, datfile->spec1, datfile->spec2};

		LoadCustomDATFile(dat);
	}
}

/*
 * This function loads all of our long-standing dat files.
 * If you want to add more, change the macro in `shared-definitions.h`
 */
ASMUsed void LoadAlwaysLoadedDatFiles(){
	LoadDATFileDynamic("MSFX.DAT"); // 3rd
}

ASMUsed void LoadStageDatFiles() {
	LoadDATFileDynamic("SFX.DAT");
}

ASMUsed void LoadMainMenuDatFiles(){
	LoadDATFileDynamic(
			GameLanguage == Language::Japanese
			? "10JCSSV.DAT"
			: "10CSSV.DAT");
}

/**
 * This file handler will keep these files in memory for the entire runtime of the game.
 */
void FileHandler_LongLasting() {
	//SetArchiveBinary(DumpPackManFile("TSGM"), 0, 0);
}