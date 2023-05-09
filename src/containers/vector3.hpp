#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "context.hpp"

struct Vector3 {
	f32 x;
	f32 y;
	f32 z;

	constexpr Vector3() : x(0), y(0), z(0){}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
	constexpr Vector3(const f32 &x, const f32 &y, const f32 &z) : x(x), y(y), z(z){}
#pragma GCC diagnostic pop

	Vector3& Transform(const Matrix4x4 &matrix);
	[[nodiscard]] f32 CalculateAngle(const Vector3 &other) const;
	Vector3& Normalize();
	[[nodiscard]] Vector3 GetForwardVectorForRidersRotation() const;
};

Quaternion Quaternion_CreateFromYawPitchRoll(f32 yaw, f32 pitch, f32 roll);

Matrix4x4 Matrix4x4_CreateFromQuaternion(Quaternion quaternion);

Matrix4x4 Matrix4x4_CreateFromYawPitchRoll(f32 yaw, f32 pitch, f32 roll);

Vector3 Vector3_Transform(Vector3 position, Matrix4x4 matrix);

f32 Vector3_CalculateAngle(Vector3 vector1, Vector3 vector2);

Vector3 Vector3_Normalize(Vector3 value);

Vector3 Vector3_GetForwardVectorForRidersRotation(Vector3 rotation);


#endif //VECTOR_HPP
