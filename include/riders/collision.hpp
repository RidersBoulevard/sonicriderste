/** @file collision.hpp
 * Header file for anything regarding collision on stages.
 */

#pragma once

#include "types.hpp"

//#define		Wall					    0x00010000UL
//#define		DeathPlane				    0x00020000UL
//#define		OffRoadCollision		    0x00030000UL
//#define		SpeedBoostCollision		    0x00040000UL
//#define		ForwardFlipRampCollision	0x00050000UL
//#define		BackFlipRampCollision		0x00060000UL
//#define		ManualRampCollision			0x00090000UL
//#define		FlightStateTrigger		    0x00110000UL
//#define		OnIceCollision			    0x00230000UL

struct CollisionProperties{
	enum CollisionType : u16{
		None 				= 0x00,
		Wall				= 0x01,
		DeathPlane			= 0x02,
		OffRoad				= 0x03,
		SpeedBoost			= 0x04,
		ForwardFlipRamp		= 0x05,
		BackFlipRamp		= 0x06,
		HalfpipeRamp		= 0x08,
		ManualRamp 			= 0x09,
		FlightStateTrigger 	= 0x11,
		OnIce 				= 0x23
	};

	CollisionType type = None;
	u16 someOtherProperty = 0;


	constexpr explicit CollisionProperties(CollisionType collisionType, u16 otherProperty = 0) : someOtherProperty(otherProperty), type(collisionType){}
	// boilerplate
	constexpr CollisionProperties() = default;
	constexpr CollisionProperties(const CollisionProperties &) = default;
	constexpr CollisionProperties(CollisionProperties &&) = default;
	constexpr ~CollisionProperties() = default;
	constexpr CollisionProperties& operator=(const CollisionProperties &) = default;
	constexpr CollisionProperties& operator=(CollisionProperties &&) = default;
};

/**
 * Holds all information about a piece of collision. There's one of this struct for each collision mesh.
 * If you wish to access the current piece of struct a Player is currently interacting with, see Player::currentCollision
 */
struct Collision {
	f32 boundX;
	f32 boundY;
	f32 boundZ;
	f32 boundScale;

	void* faceData;
	CollisionProperties properties;
};
static_assert(sizeof(Collision) == 24);