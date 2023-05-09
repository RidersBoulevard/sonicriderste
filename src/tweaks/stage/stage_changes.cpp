#include "context.hpp"
#include "containers/vector3.hpp"
#include "lib/sound.hpp"
#include "mechanics/magneticimpulse.hpp"

global u32 data_stageChanges[];
global f32 data_TornadoSlingshot[];
global u8 bss_StageChanges[];
//global f32 bss_TornadoTest[];

// this structure for object1 will be specifically used for this code
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct TornadoObject {
	u32 origin_x; // floats
	u32 origin_y;
	u32 origin_z;
	struct unknown *unknown;
	f32 current_x;
	f32 current_y;
	f32 current_z;
	char filler[0x20];
	u8 playerIndex; // only for tornadoes, determines which player's tornado it is via index
	char filler2[0x3];
	u8 tornadoLevel;
	char filler3[0x8];
	u32 item_icon;
};
#pragma GCC diagnostic pop

inline void lbl_update_item(Object *object, u32 item) {
	auto *object1 = static_cast<Object1 *>(object->object);
	object->item_id = item;
	object1->item_icon = item;
}

std::array<f32, 3> TornadoSlingshotDistances = {
        5.0f, // level 1 tornado
        6.0f, // level 2
        14.0f // level 3
};

static void func_TornadoSlingshot(Player *player, Object *object, PlayerCameraStruct *camera) {
	// should be compiled using CW 1.2.5
	auto *object1 = reinterpret_cast<TornadoObject *>(object->object);
	// it is assumed that the player pointer passed in points to the first player
	// it is assumed that the passed in player camera struct points to the first player's camera
	Vector3 rotation, forward, directionToTornado, delta;
	u32 i;
	f32 angle, distance, deltax, deltay, deltaz;
	u8 *bss = bss_StageChanges;
	u32 playercount = InGamePlayerCount;

	for (i = 0; i < playercount; i++) {
		if (player[i].ignoreTurbulence == FALSE && player[i].playerType == FALSE) {
			u8 index = player[i].index;
			u8 timer = bss[index];
			if (timer != 0) {
				bss[index] = timer - 1;
				continue;
			}

			if (index == object1->playerIndex) {
				continue;
			}

			deltax = player[i].x - object1->current_x;
			deltay = player[i].y - object1->current_y;
			deltaz = player[i].z - object1->current_z;

			distance = deltax * deltax + deltay * deltay + deltaz * deltaz;

			rotation.x = player[i].verticalRotation;
			rotation.y = player[i].horizontalRotation;
			rotation.z = player[i].rotationRoll;

			forward = Vector3_GetForwardVectorForRidersRotation(rotation);

			delta.x = deltax;
			delta.y = deltay;
			delta.z = deltaz;

			directionToTornado = Vector3_Normalize(delta);

			angle = Vector3_CalculateAngle(forward, directionToTornado);

			if (TornadoSlingshotDistances[object1->tornadoLevel] < distance) {
				continue;
			}

			bss[index] = 30; // 30 frames

			// angle at which you're "looking" at the tornado and won't slingshot
            if (0.7f > angle) {
				continue;
			}

			if (player[i].state == Stun) {
				continue;
			}

			player[i].speed += pSpeed(60) + ScaleUsingCurrentMI(*player, pSpeed(60));
            player[i].reciproExtendTimer = 0;

			//camera = PlayerCameraStruct[index];
			camera = &camera[index];
			camera->cameraPreset = 130; // boost state camera preset
			camera->cameraPresetProperty = 0x0;

			//bss[index] = 10; // 10 frames
			player[i].dreamTrail_timer = 30;
			PlayAudioFromDAT(Sound::SFX::TornadoSlingshot);

			//tornadobss[index*2] = distance;
			//tornadobss[index*2+1] = angle;
		}
	}
}

ASMUsed void func_StageChanges(u32 currentStage, Player *player, Object *object, PlayerCameraStruct *camera) {
	if (!object->object) {
		return;
	}

	auto *object1 = static_cast<Object1 *>(object->object);

	u16 object_type = object->object_type;
	if (object_type == 0x1E || object_type == 0x1F) {
		func_TornadoSlingshot(player, object, camera);
	}

	u16 object_id = object->object_id;
	u32 i;
	u32 *data = data_stageChanges;
	switch (currentStage) {
		case GreenCave: {
			if (object_id == 0xE8 && object_type == FlightRing) {
				object1->x_rotation = s32(0xFFFFF06D);
			} else if (object_type == ItemBox) {
				switch (object_id) {
					case 0xBB:
						lbl_update_item(object, HundredAir);
						break;
					case 0xBC:
					case 0xBD:
						lbl_update_item(object, SpeedShoes);
						break;
					default:
						break;
				}
			}
			break;
		}
		case WhiteCave: {
			if (object_id == 0xC4 && object_type == WhiteCaveWeb) {
				if (object->state != 1) {
					object->state = 1;
				}
			} else if (object_id >= 0x1C && object_id <= 0x1E && object_type == DashPanel) {
				i = object_id - 0x1C; // generate index
				i *= 6;
				i += 11; // index into correct portion of data section
				auto *obj1 = static_cast<Object1 *>(object->object);
				obj1->x = data[i];
				obj1->y = data[i + 1];
				obj1->z = data[i + 2];

				obj1->x_rotation = static_cast<s32>(data[i + 3]);
				obj1->y_rotation = static_cast<s32>(data[i + 4]);
				obj1->z_rotation = static_cast<s32>(data[i + 5]);
			}
			break;
		}
		case EggFactory: {
			if (object_id >= 0x104 && object_id <= 0x106 && object_type == DashPanel) {
				i = object_id - 0x104; // generate index
				i += 29; // index into correct portion
				auto *obj1 = static_cast<Object1 *>(object->object);
				obj1->z = data[i];
				obj1->x = data[32];
			} else if (object_id >= 0x125 && object_id <= 0x126 && object_type == FlightRing) {
				i = object_id - 0x125;
				i *= 6;
				i += 33;
				auto *obj1 = static_cast<Object1 *>(object->object);
				obj1->x = data[i];
				obj1->y = data[i + 1];
				obj1->z = data[i + 2];

				obj1->x_rotation = static_cast<s32>(data[i + 3]);
				obj1->y_rotation = static_cast<s32>(data[i + 4]);
				obj1->z_rotation = static_cast<s32>(data[i + 5]);
			}
			break;
		}

		case RedCanyon: {
			if (object_id >= 0xA4 && object_id <= 0xA7 && object_type == DashPanel) {
				i = 0xA7 - object_id;
				i *= 2;
				i += 45;
				auto *obj1 = static_cast<Object1 *>(object->object);
				obj1->x = data[i];
				obj1->y = data[53];
				obj1->z = data[i + 1];

				obj1->y_rotation = static_cast<s32>(0xFFFFF5A0);
			}
			break;
		}
			/*
			case IceFactory:
			{
				if (object_id >= 0x135 && object_id <= 0x136 && object_type == DashPanel) {
					if (object->state != 1)
					{
						object->state = 1;
					}
				}
				break;
			}

			case SandRuins:
			{
				if (object_id >= 0x11F && object_id <= 0x121 && object_type == DashPanel) {
					if (object->state != 1)
					{
						object->state = 1;
					}
				}
				break;
			}
			*/
		default:
			break;
	}

	if (object_type != ItemBox) { return; }

	switch (currentStage) {
		case MetalCity: {
			if (object_id == 0x2A) {
				lbl_update_item(object, TwentyRings);
			}
			break;
		}
		case NightChase: {
			if (object_id >= 0x67 && object_id <= 0x6E) {
				lbl_update_item(object, TenRings);
			} else if (object_id >= 0xFC && object_id <= 0x100) {
				lbl_update_item(object, RNG);
			}
			break;
		}
		case SegaIllusion: {
			switch (object_id) {
				case 0xDD:
					lbl_update_item(object, FiftyAir);
					break;
				case 0xDC:
					lbl_update_item(object, ThirtyAir);
					break;
				default:
					break;
			}
			if (object_id >= 0xB6 && object_id <= 0xB9) {
				lbl_update_item(object, TenRings);
			} else if (object_id >= 0xBA && object_id <= 0xBD) {
				lbl_update_item(object, FiftyAir);
			}
			break;
		}
		case SandRuins: {
			if (object_id >= 0x1A1 && object_id <= 0x1A4) {
				lbl_update_item(object, TenRings);
			}
			switch (object_id) {
				case 0x18C:
					lbl_update_item(object, ThirtyAir);
					break;
				case 0x18D:
					lbl_update_item(object, FiftyAir);
					break;
				case 0x18E:
					lbl_update_item(object, HundredAir);
					break;
				default:
					break;
			}
			break;
		}
		case SplashCanyon: {
			if (object_id == 0xCC) {
				lbl_update_item(object, ThirtyRings);
			}
			break;
		}
		case DigitalDimension: {
			if (object_id >= 0xC && object_id <= 0xF) {
				object1->z = data[54];
			} else if (object_id >= 0x10 && object_id <= 0x13) {
				lbl_update_item(object, TenRings);
				i = object_id - 0x10;
				i *= 3;
				i += 55;

				auto *obj1 = static_cast<Object1 *>(object->object);
				obj1->x = data[i];
				obj1->y = data[i + 1];
				obj1->z = data[i + 2];
			}
			break;
		}
		case SkyRoad: {
			if (object_id >= 0xD8 && object_id <= 0xDF) {
				lbl_update_item(object, TenRings);
			}
			break;
		}
		default:
			break;
	}
}