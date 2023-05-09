#include "customobjects.hpp"

void* CopyGameObjectStruct(const GameObject *object, void* dest) {
    memcpy(dest, object, sizeof(GameObject));
    return static_cast<u32*>(dest) + (sizeof(GameObject)/4);
}

USED void CustomGameObjectSpawner() {
    void* startOfGameObjects = lbl_10008200[4];
    void* gameObjectStructStart = static_cast<u32*>(startOfGameObjects) + 0x2;
    u32 objectAmount = *static_cast<u32*>(startOfGameObjects);
    void* gameObjectStructEnd = static_cast<u32*>(gameObjectStructStart) + objectAmount*(sizeof(GameObject)/4);

    if (CurrentGameMode != TimeTrial && CurrentGameMode != FreeRace && CurrentGameMode != WorldGrandPrix)
    { return; }

    if (RaceExitMethod == 2) { return; } // if player hit the retry button instead of quit button

    switch (CurrentStage) {
        case SegaCarnival:
        {
            *static_cast<u32*>(startOfGameObjects) = objectAmount + 2;
            gameObjectStructEnd = CopyGameObjectStruct(&segaCarnival_flyhoop1, gameObjectStructEnd);
            gameObjectStructEnd = CopyGameObjectStruct(&segaCarnival_flyhoop2, gameObjectStructEnd);
            break;
        }
        case EggFactory:
        {
            *static_cast<u32*>(startOfGameObjects) = objectAmount + 3;
            gameObjectStructEnd = CopyGameObjectStruct(&eggFactory_flyhoop1, gameObjectStructEnd);
            gameObjectStructEnd = CopyGameObjectStruct(&eggFactory_flyhoop2, gameObjectStructEnd);
            gameObjectStructEnd = CopyGameObjectStruct(&eggFactory_flyhoop3, gameObjectStructEnd);
            break;
        }
        case NightChase:
        {
            *static_cast<u32*>(startOfGameObjects) = objectAmount + 1;
            gameObjectStructEnd = CopyGameObjectStruct(&nightChase_car1, gameObjectStructEnd);
            break;
        }
        case DarkDesert:
        {
            *static_cast<u32*>(startOfGameObjects) = objectAmount + 5;
            gameObjectStructEnd = CopyGameObjectStruct(&darkDesert_flyhoop1, gameObjectStructEnd);
            gameObjectStructEnd = CopyGameObjectStruct(&darkDesert_flyhoop2, gameObjectStructEnd);
            gameObjectStructEnd = CopyGameObjectStruct(&darkDesert_flyhoop3, gameObjectStructEnd);

            gameObjectStructEnd = CopyGameObjectStruct(&darkDesert_itembox1, gameObjectStructEnd);
            gameObjectStructEnd = CopyGameObjectStruct(&darkDesert_itembox2, gameObjectStructEnd);
            break;
        }
        case BabylonGarden:
        {
            *static_cast<u32*>(startOfGameObjects) = objectAmount + 9;
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
        case DigitalDimension:
        {
            *static_cast<u32*>(startOfGameObjects) = objectAmount + 2;
            gameObjectStructEnd = CopyGameObjectStruct(&digitalDimension_itembox1, gameObjectStructEnd);
            gameObjectStructEnd = CopyGameObjectStruct(&digitalDimension_itembox2, gameObjectStructEnd);
            break;
        }
		default:
			break;
    }
}

USED BOOL GameObjectDespawner(ObjectInfo *objectInfo, u32 object_id) {
    // returns true if object should be despawned, returns false if not
    
    if (objectInfo->objectType == ItemBox && CurrentStage != MetalCity) {
        if (objectInfo->itemID == HundredRings || objectInfo->itemID == MaxAir)
        return TRUE;
    }

    switch (CurrentStage) {
        case IceFactory:
            if (object_id >= 0x135 && object_id <= 0x136 && objectInfo->objectType == DashPanel)
            return TRUE;

            break;

        case SandRuins:
            if (object_id >= 0x11F && object_id <= 0x121 && objectInfo->objectType == DashPanel)
            return TRUE;
            
            break;
		default:
			break;
    }

    return FALSE;
}