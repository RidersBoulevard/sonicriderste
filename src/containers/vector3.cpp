// This source file will contain the currently used/ported most useful vector functions
// Requires context.c as a prerequisite

#include "vector3.hpp"
#include <cmath>

constexpr Vector3 forward = {0.0f, 0.0f, -1.0f};

struct Quaternion Quaternion_CreateFromYawPitchRoll(f32 yaw, f32 pitch, f32 roll) {
	// should be compiled with CW 1.2.5

	//  Roll first, about axis the object is facing, then
	//  pitch upward, then yaw to face into the new heading
	f32 sr, cr, sp, cp, sy, cy, halfRoll, halfPitch, halfYaw;
	Quaternion result{};

	halfRoll = roll * 0.5f;
	sr = std::sin(halfRoll);
	cr = std::cos(halfRoll);

	halfPitch = pitch * 0.5f;
	sp = std::sin(halfPitch);
	cp = std::cos(halfPitch);

	halfYaw = yaw * 0.5f;
	sy = std::sin(halfYaw);
	cy = std::cos(halfYaw);

	result.x = cy * sp * cr + sy * cp * sr;
	result.y = sy * cp * cr - cy * sp * sr;
	result.z = cy * cp * sr - sy * sp * cr;
	result.w = cy * cp * cr + sy * sp * sr;

	return result;
}

struct Matrix4x4 Matrix4x4_CreateFromQuaternion(Quaternion quaternion) {
	/*
	rodata should look like:
	.single 1 # 0x0
	.single 2 # 0x4
	.single 0 # 0x8
	*/
	Matrix4x4 result{};

	f32 xx = quaternion.x * quaternion.x;
	f32 yy = quaternion.y * quaternion.y;
	f32 zz = quaternion.z * quaternion.z;

	f32 xy = quaternion.x * quaternion.y;
	f32 wz = quaternion.z * quaternion.w;
	f32 xz = quaternion.z * quaternion.x;
	f32 wy = quaternion.y * quaternion.w;
	f32 yz = quaternion.y * quaternion.z;
	f32 wx = quaternion.x * quaternion.w;

	result.field[0] = 1.0f - 2.0f * (yy + zz);
	result.field[1] = 2.0f * (xy + wz);
	result.field[2] = 2.0f * (xz - wy);
	result.field[3] = 0.0f;
	result.field[4] = 2.0f * (xy - wz);
	result.field[5] = 1.0f - 2.0f * (zz + xx);
	result.field[6] = 2.0f * (yz + wx);
	result.field[7] = 0.0f;
	result.field[8] = 2.0f * (xz + wy);
	result.field[9] = 2.0f * (yz - wx);
	result.field[10] = 1.0f - 2.0f * (yy + xx);
	result.field[11] = 0.0f;
	result.field[12] = 0.0f;
	result.field[13] = 0.0f;
	result.field[14] = 0.0f;
	result.field[15] = 1.0f;

	return result;
}

struct Matrix4x4 Matrix4x4_CreateFromYawPitchRoll(f32 yaw, f32 pitch, f32 roll) {
	Quaternion q = Quaternion_CreateFromYawPitchRoll(yaw, pitch, roll);

	return Matrix4x4_CreateFromQuaternion(q);
}

Vector3 Vector3_Transform(Vector3 position, Matrix4x4 matrix) {
	Vector3 vector{};
	vector.x = position.x * matrix.field[0] + position.y * matrix.field[4] + position.z * matrix.field[8] +
			   matrix.field[12];
	vector.y = position.x * matrix.field[1] + position.y * matrix.field[5] + position.z * matrix.field[9] +
			   matrix.field[13];
	vector.z = position.x * matrix.field[2] + position.y * matrix.field[6] + position.z * matrix.field[10] +
			   matrix.field[14];

	return vector;
}

f32 Vector3_CalculateAngle(Vector3 vector1, Vector3 vector2) {
	// example: α = arccos[(xa * xb + ya * yb + za * zb) / (√(xa2 + ya2 + za2) * √(xb2 + yb2 + zb2))]
	f32 dotproduct = vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
	f32 lengths = std::sqrt(vector1.x * vector1.x + vector1.y * vector1.y + vector1.z * vector1.z) *
				  std::sqrt(vector2.x * vector2.x + vector2.y * vector2.y + vector2.z * vector2.z);

	return std::acos(dotproduct / lengths);
}

Vector3 Vector3_Normalize(Vector3 value) {
	Vector3 result{};
	f32 ls = value.x * value.x + value.y * value.y + value.z * value.z;
	f32 length = std::sqrt(ls);
	result.x = value.x / length;
	result.y = value.y / length;
	result.z = value.z / length;

	return result;
}

Vector3 Vector3_GetForwardVectorForRidersRotation(Vector3 rotation) {
	/*
	f1 = single 0
	f0 = single -1
	*/
	Vector3 result{};
	struct Matrix4x4 matrix = Matrix4x4_CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);

	result = Vector3_Transform(forward, matrix);
	result = Vector3_Normalize(result);

	return result;
}

Vector3 &Vector3::Transform(const Matrix4x4 &matrix) {
	x = x * matrix.field[0] + y * matrix.field[4] + z * matrix.field[8] + matrix.field[12];
	y = x * matrix.field[1] + y * matrix.field[5] + z * matrix.field[9] + matrix.field[13];
	z = x * matrix.field[2] + y * matrix.field[6] + z * matrix.field[10] + matrix.field[14];
	return *this;
}

f32 Vector3::CalculateAngle(const Vector3 &other) const {
	// example: α = arccos[(xa * xb + ya * yb + za * zb) / (√(xa2 + ya2 + za2) * √(xb2 + yb2 + zb2))]
	f32 dotproduct = x * other.x + y * other.y + z * other.z;
	f32 lengths = std::sqrt(x * x + y * y + z * z) *
				  std::sqrt(other.x * other.x + other.y * other.y + other.z * other.z);

	return std::acos(dotproduct / lengths);
}

Vector3 &Vector3::Normalize() {
	f32 ls = x * x + y * y + z * z;
	f32 length = std::sqrt(ls);
	x = x / length;
	y = y / length;
	z = z / length;

	return *this;
}

Vector3 Vector3::GetForwardVectorForRidersRotation() const {
	/*
	f1 = single 0
	f0 = single -1
	*/
	Vector3 result{};
	Matrix4x4 matrix = Matrix4x4_CreateFromYawPitchRoll(y, x, z);

	result = Vector3_Transform(forward, matrix);
	result = Vector3_Normalize(result);

	return result;
}
