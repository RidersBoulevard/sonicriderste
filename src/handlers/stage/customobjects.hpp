#pragma once

#include "riders/object.hpp"

struct GameObject {
	u16 object_id;
	u8 playerAmount;// set this to 04 for singleplayer
	u8 portal;
	u32 flags;
	u32 properties;// item id for item boxes, fly hoop control lock frames
	f32 x;
	f32 y;
	f32 z;
	f32 x_rotation;
	f32 y_rotation;
	f32 z_rotation;
	f32 otherProperties; // fly hoop speed, 0 for item boxes
	f32 _otherProperties;// fly hoop scale, 1 is normal
	f32 unkProperty;
};

constexpr GameObject segaCarnival_flyhoop1 =
		{FlightRing, 0x4, 0x42, 0x8F, 0,
		 -0.5f, 10.3103f, -123.07f,// coordinates
		 -5, 180, -0.395f,         // rotation
		 216, 1, 0};               // other properties

constexpr GameObject segaCarnival_flyhoop2 =
		{FlightRing, 0x4, 0x43, 0x8F, 0,
		 10.9598f, 13.9957f, -264.074f,// coordinates
		 33, 150, -0.395f,             // rotation
		 216, 1, 0};                   // other properties

constexpr GameObject eggFactory_flyhoop1 =
		{FlightRing, 0x4, 0x44, 0x8F, 0x3C,
		 -72.6393f, 21.9509f, -300.476f, // coordinates
		 -18.0029f, 65.8843f, -0.823673f,// rotation
		 170, 1, 0};                     // other properties

constexpr GameObject eggFactory_flyhoop2 =
		{FlightRing, 0x4, 0x44, 0x8F, 0,
		 -5.36687f, 50.4197f, -267.296f,// coordinates
		 1.15171f, 31.0356f, -0.126376f,// rotation
		 170, 1, 0};                    // other properties

constexpr GameObject eggFactory_flyhoop3 =
		{FlightRing, 0x4, 0x44, 0x8F, 0,
		 20.276f, 47.9736f, -219.703f,// coordinates
		 5.68786f, 28.2202f, 3.21242f,// rotation
		 170, 1, 0};                  // other properties

constexpr GameObject nightChase_car1 =
		{Car, 0x4, 0x4F, 0x1DCF, 0x2,
		 -125.595f, -66.9674f, -411.637f,// coordinates
		 -1.05286f, 171.856f, 0.00003f,  // rotation
		 0, 0, 0};                       // other properties

constexpr GameObject darkDesert_flyhoop1 =
		{FlightRing, 0x4, 0x44, 0x8F, 0,
		 -598.214f, -83.5013f, 155.607f,// coordinates
		 7.57298f, -55.5625f, 0,        // rotation
		 200, 1, 0};                    // other properties

constexpr GameObject darkDesert_flyhoop2 =
		{FlightRing, 0x4, 0x44, 0x8F, 0,
		 -636.014f, -89.5946f, 181.526f,// coordinates
		 4.86715f, -28.5514f, 0,        // rotation
		 200, 1, 0};                    // other properties

constexpr GameObject darkDesert_flyhoop3 =
		{FlightRing, 0x4, 0x44, 0x8F, 0,
		 -663.046f, -96.349f, 231.94f,// coordinates
		 8.8207f, 31.0243f, 0,        // rotation
		 200, 1, 0};                  // other properties

constexpr GameObject darkDesert_itembox1 =
		{ItemBox, 0x4, 0x45, 0xF, TenRings,
		 -646.445f, -111.064f, 294.896f,// coordinates
		 -0.178561f, -90, -10.8424f,    // rotation
		 0, 0, 0};                      // other properties

constexpr GameObject darkDesert_itembox2 =
		{ItemBox, 0x4, 0x45, 0xF, TenRings,
		 -644.176f, -109.87f, 297.661f,// coordinates
		 -0.178561f, -90, -10.8424f,   // rotation
		 0, 0, 0};                     // other properties

constexpr GameObject babylonGarden_flyhoop1 =
		{FlightRing, 0x4, 0x41, 0x8F, 0,
		 223.525f, -38.4718f, -8.32182f,// coordinates
		 30.607f, 106.97f, 0,           // rotation
		 190, 1, 0};                    // other properties

constexpr GameObject babylonGarden_itembox1 =
		{ItemBox, 0x4, 0x4F, 0xF, TenRings,
		 95.5558f, -78, -10.0042f,// coordinates
		 0, 0, 0,                 // rotation
		 0, 0, 0};                // other properties

constexpr GameObject babylonGarden_itembox2 =
		{ItemBox, 0x4, 0x4F, 0xF, TenRings,
		 95.5558f, -78, -7.27519f,// coordinates
		 0, 0, 0,                 // rotation
		 0, 0, 0};                // other properties

constexpr GameObject babylonGarden_itembox3 =
		{ItemBox, 0x4, 0x4F, 0xF, TenRings,
		 95.5558f, -78, -4.37523f,// coordinates
		 0, 0, 0,                 // rotation
		 0, 0, 0};                // other properties

constexpr GameObject babylonGarden_itembox4 =
		{ItemBox, 0x4, 0x4F, 0xF, TenRings,
		 95.5558f, -78, -1.53358f,// coordinates
		 0, 0, 0,                 // rotation
		 0, 0, 0};                // other properties

constexpr GameObject babylonGarden_itembox5 =
		{ItemBox, 0x4, 0x4F, 0xF, TenRings,
		 95.5558f, -78, 1.33585f,// coordinates
		 0, 0, 0,                // rotation
		 0, 0, 0};               // other properties

constexpr GameObject babylonGarden_itembox6 =
		{ItemBox, 0x4, 0x4F, 0xF, TenRings,
		 95.5558f, -78, 4.21506f,// coordinates
		 0, 0, 0,                // rotation
		 0, 0, 0};               // other properties

constexpr GameObject babylonGarden_itembox7 =
		{ItemBox, 0x4, 0x4F, 0xF, TenRings,
		 95.5558f, -78, 7.17066f,// coordinates
		 0, 0, 0,                // rotation
		 0, 0, 0};               // other properties

constexpr GameObject babylonGarden_itembox8 =
		{ItemBox, 0x4, 0x4F, 0xF, TenRings,
		 95.5558f, -78, 10.0251f,// coordinates
		 0, 0, 0,                // rotation
		 0, 0, 0};               // other properties

constexpr GameObject digitalDimension_itembox1 =
		{ItemBox, 0x4, 0x46, 0xF, HundredAir,
		 658.329f, 0.0307086f, -138.386f,// coordinates
		 0, 0, 0,                        // rotation
		 0, 0, 0};                       // other properties

constexpr GameObject digitalDimension_itembox2 =
		{ItemBox, 0x4, 0x46, 0xF, HundredAir,
		 635.489f, 2.85357e-05f, -238.356f,// coordinates
		 0, 0, 0,                          // rotation
		 0, 0, 0};                         // other properties

ASMDefined void *lbl_10008200[];
ASMUsed void CustomGameObjectSpawner();
ASMUsed bool GameObjectDespawner(ObjectInfo *objectInfo, u32 object_id);