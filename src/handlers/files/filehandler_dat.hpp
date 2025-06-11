#pragma once

#include "globalDefs.hpp"
#include "types.hpp"
#include "lib/files.hpp"

#include <memory>
#include <string_view>

struct DATSpecs {
	fillerData<0x10> filler;
    u32 spec1;
    u32 unknown;
    u32 spec2;
};

/**
 * A representation of the DAT file
 * 	Used to load the full dat file for use
 */
struct DATFile {
	static constexpr auto maxFileNameSize = 0x10;

	std::array<char, maxFileNameSize> fileName{};
	u32 spec1 = 0; // offset 0x10 in DAT file
	u32 spec2 = 0; // offset 0x18 in DAT file
	[[maybe_unused]] const u32 *soundFileID = nullptr; // pointer to IDs that play sounds like 0xA9300100

	constexpr DATFile() = default;

	/**
	 * Constructor for generating a DATFile at runtime.
	 * Primarily used for dynamic DATFile loads
	 * @param _fileName The name of the datfile to open.
	 * @param _spec1 First magic number needed to load the file. offset 0x10 in DAT file
	 * @param _spec2 Second magic number needed to load the file. offset 0x18 in DAT file
	 * @param _soundFileID pointer to IDs that play sounds like 0xA9300100. We never use it so its always null
	 */
	constexpr explicit DATFile(const std::string_view _fileName, const u32 &_spec1 = 0, const u32 &_spec2 = 0, const u32 *_soundFileID = nullptr) : spec1(_spec1), spec2(_spec2), soundFileID(_soundFileID){
		std::copy(_fileName.begin(), _fileName.end(), this->fileName.begin());
	}
};

ASMUsed void LoadMainMenuDatFiles();
ASMUsed void LoadStageDatFiles();

constexpr auto LongStandingDatFileCount = DATFILE_LONG_STANDING_FILE_COUNT;
ASMDefined u8 LoadedDynamicDATFileCount;

void FreeDynamicDatFileByIndex(u8 index = LoadedDynamicDATFileCount);

ASMDefined DATFile& LoadCustomDATFile(DATFile& customDat); // Returns first argument
ASMDefined void FreeDATFiles(u8 indexToStartFreeing = LongStandingDatFileCount);
ASMDefined void FreeDATFileByIndex(u8 index);
ASMUsed void LoadDATFileDynamic(const char filename[]);
ASMUsed void DATFileHandler();
