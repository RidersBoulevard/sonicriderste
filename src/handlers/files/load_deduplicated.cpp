#include "load_deduplicated.hpp"
#include "riders/stage.hpp"
#include "riders/gamemode.hpp"
#include "lib/files.hpp"

#include <format>

#include "lib/calculate_date.hpp"

extern const vu32 gu32ViewportNum;

inline void LoadPackManFileGeneral(const char* fileName) {
    SetArchiveBinary(DumpPackManFile(fileName, 0), 0, 0);
}

ASMUsed void LoadDeduplicatedStage() {
    // load common files for every stage
    LoadPackManFileGeneral("CMN");

    if (CurrentGameMode == MissionMode) {
        LoadPackManFileGeneral("MCMN");
    }
    else if (CurrentGameMode == TagMode) {
        LoadPackManFileGeneral("TCMN");
    }

    // load specific assets depending on player count ("CMN1", "CMN2", "CMN4")
    auto playerCount = gu32ViewportNum;
    if (playerCount > 2) { playerCount = 4; }

    const auto filename = std::format("CMN{}", playerCount);
    LoadPackManFileGeneral(filename.c_str());

    // get specific stage's common file name, like "1CMN"
    auto cmnStageFilename = std::format("{}CMN", CurrentStage);

    std::string stageFilename;

    // get stage file name ("1" or "1V" or "1M", or "M1" for mission mode)
    if (CurrentGameMode == MissionMode) {
        stageFilename = std::format("M{}", CurrentStage);
    }
    else {
        stageFilename = std::format("{}", CurrentStage);
        if (playerCount > 2) {
            stageFilename += "M";
        }
        else if (playerCount == 2) {
            stageFilename += "V";
        }
    }

    const auto isApril = Date::GetCurrentDate().CheckAprilFools();
    if (isApril && CurrentGameMode != MissionMode) {
        switch (CurrentStage) {
            case MetalCity:
                cmnStageFilename += "A";
                stageFilename += "A";
                break;
            case GreenCave:
            case SkyRoad:
                stageFilename += "A";
                break;
            case BabylonGarden:
            case DarkDesert:
                cmnStageFilename += "A";
                break;
            case EggFactory:
                cmnStageFilename += "A";
                if (playerCount <= 2) {
                    stageFilename += "A";
                }
                break;
            default:
                break;
        }
    }

    // load the files
    LoadPackManFileGeneral(cmnStageFilename.c_str());
    LoadPackManFileGeneral(stageFilename.c_str());
}
