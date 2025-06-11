#include "customobjects.hpp"
#include "riders/gamemode.hpp"
#include "riders/stage.hpp"
#include "riders/player.hpp"

#include <cstring>

void *CopyGameObjectStruct(const GameObject *object, void *dest) {
	std::memcpy(dest, object, sizeof(GameObject));
	return static_cast<u32 *>(dest) + (sizeof(GameObject) / 4);
}

USED void CustomGameObjectSpawner() {
	void *startOfGameObjects = gu32ObjMtxPalMaxSize[4];
	void *gameObjectStructStart = static_cast<u32 *>(startOfGameObjects) + 0x2;
	const u32 objectAmount = *static_cast<u32 *>(startOfGameObjects);
	void *gameObjectStructEnd = static_cast<u32 *>(gameObjectStructStart) + objectAmount * (sizeof(GameObject) / 4);

	if(CurrentGameMode != TimeTrial && CurrentGameMode != FreeRace && CurrentGameMode != WorldGrandPrix) { return; }

	if(gu32EndOfGameFlag == std::to_underlying(ExitMethod::Retry)) { return; }// if player hit the retry button instead of quit button

	switch(CurrentStage) {
		case SegaCarnival: {
			*static_cast<u32 *>(startOfGameObjects) = objectAmount + 3;
			gameObjectStructEnd = CopyGameObjectStruct(&segaCarnival_flyhoop1, gameObjectStructEnd);
			gameObjectStructEnd = CopyGameObjectStruct(&segaCarnival_flyhoop2, gameObjectStructEnd);
		    gameObjectStructEnd = CopyGameObjectStruct(&segaCarnival_flyhoop3, gameObjectStructEnd);
			break;
		}
		case EggFactory: {
			*static_cast<u32 *>(startOfGameObjects) = objectAmount + 3;
			gameObjectStructEnd = CopyGameObjectStruct(&eggFactory_flyhoop1, gameObjectStructEnd);
			gameObjectStructEnd = CopyGameObjectStruct(&eggFactory_flyhoop2, gameObjectStructEnd);
			gameObjectStructEnd = CopyGameObjectStruct(&eggFactory_flyhoop3, gameObjectStructEnd);
			break;
		}
		case NightChase: {
			*static_cast<u32 *>(startOfGameObjects) = objectAmount + 1;
			gameObjectStructEnd = CopyGameObjectStruct(&nightChase_car1, gameObjectStructEnd);
			break;
		}
		case DarkDesert: {
			*static_cast<u32 *>(startOfGameObjects) = objectAmount + 5;
			// gameObjectStructEnd = CopyGameObjectStruct(&darkDesert_flyhoop1, gameObjectStructEnd);
			// gameObjectStructEnd = CopyGameObjectStruct(&darkDesert_flyhoop2, gameObjectStructEnd);
			// gameObjectStructEnd = CopyGameObjectStruct(&darkDesert_flyhoop3, gameObjectStructEnd);

			gameObjectStructEnd = CopyGameObjectStruct(&darkDesert_itembox1, gameObjectStructEnd);
			gameObjectStructEnd = CopyGameObjectStruct(&darkDesert_itembox2, gameObjectStructEnd);
			break;
		}
		case BabylonGarden: {
			*static_cast<u32 *>(startOfGameObjects) = objectAmount + 9;
			gameObjectStructEnd = CopyGameObjectStruct(&babylonGarden_flyhoop1, gameObjectStructEnd);

			gameObjectStructEnd = CopyGameObjectStruct(&babylonGarden_itembox1, gameObjectStructEnd);
			gameObjectStructEnd = CopyGameObjectStruct(&babylonGarden_itembox2, gameObjectStructEnd);
			gameObjectStructEnd = CopyGameObjectStruct(&babylonGarden_itembox3, gameObjectStructEnd);
			gameObjectStructEnd = CopyGameObjectStruct(&babylonGarden_itembox4, gameObjectStructEnd);
			gameObjectStructEnd = CopyGameObjectStruct(&babylonGarden_itembox5, gameObjectStructEnd);
			gameObjectStructEnd = CopyGameObjectStruct(&babylonGarden_itembox6, gameObjectStructEnd);
			gameObjectStructEnd = CopyGameObjectStruct(&babylonGarden_itembox7, gameObjectStructEnd);
			gameObjectStructEnd = CopyGameObjectStruct(&babylonGarden_itembox8, gameObjectStructEnd);
			break;
		}
		case DigitalDimension: {
			*static_cast<u32 *>(startOfGameObjects) = objectAmount + 2;
			gameObjectStructEnd = CopyGameObjectStruct(&digitalDimension_itembox1, gameObjectStructEnd);
			gameObjectStructEnd = CopyGameObjectStruct(&digitalDimension_itembox2, gameObjectStructEnd);
			break;
		}
		default:
			break;
	}
}

ASMUsed bool GameObjectDespawner(ObjectInfo *objectInfo, u32 object_id) {
	// returns true if object should be despawned, returns false if not

	if(objectInfo->objectType == ObjectTypes::ItemBox && CurrentStage != MetalCity) {
		if(objectInfo->itemID == std::to_underlying(ItemID::HundredRings) || objectInfo->itemID == std::to_underlying(ItemID::MaxAir))
			return true;
	}

    if (objectInfo->objectType == ObjectTypes::MetalCityCar && CurrentStage == MetalCity) {
        switch (object_id) {
            case 0x146:
            case 0x149:
                return true;
                break;
            default: break;
        }
    }

	if ((objectInfo->objectType == ObjectTypes::NightChaseCar) && (CurrentStage == NightChase)) {
		// Check for total power types and then spawn a variable amount of cars
		u32 totalPowerTypes = 0;
		for (const auto &player : getCurrentPlayerList()) {
			if (player.typeAttributes.hasAny(Type::Power)) ++totalPowerTypes;
		}
		if (object_id >= (0x29 + (totalPowerTypes * 0x7))) return true;
		return false;
	}

	switch(CurrentStage) {
		case IceFactory:
			if(object_id >= 0x135 && object_id <= 0x136 && objectInfo->objectType == ObjectTypes::DashPanel)
				return true;

			break;

		case SandRuins:
			if(object_id >= 0x11F && object_id <= 0x121 && objectInfo->objectType == ObjectTypes::DashPanel)
				return true;

			break;

        case SegaCarnival:
            if (object_id >= 0x3D && object_id <= 0x3F && objectInfo->objectType == ObjectTypes::PowerObject)
                return true;

            break;

	    case BabylonGuardian:
	        if (objectInfo->objectType == ObjectTypes::GuardianBoss) return true; // guardian

		default:
			break;
	}

	return false;
}