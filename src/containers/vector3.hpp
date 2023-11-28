#pragma once

#include <cmath>
#include "types.hpp"

struct Quaternion {
	f32 x;
	f32 y;
	f32 z;
	f32 w;

	[[nodiscard]] constexpr static Quaternion fromYawPitchRoll(const f32 &yaw, const f32 &pitch, const f32 &roll);
};

template<typename T> requires arithmetic<T>
struct Matrix4x4 {
	std::array<T, 16> field;

	[[nodiscard]] constexpr static Matrix4x4 fromQuaternion(const Quaternion &quaternion);

	[[nodiscard]] constexpr static Matrix4x4 fromYawPitchRoll(const f32 &yaw, const f32 &pitch, const f32 &roll);
};

template<typename T> requires arithmetic<T>
struct Matrix3x3 {
	std::array<T, 12> field;
};

template<typename T> requires arithmetic<T>
struct Vector3 {
	T x;
	T y;
	T z;

	[[nodiscard]] constexpr Vector3<T> transform(const Matrix4x4<T> &matrix) const;
	[[nodiscard]] constexpr f32 calculateAngle(const Vector3<T> &other) const;
	[[nodiscard]] constexpr Vector3<T> getForwardVectorForRidersRotation() const;
	[[nodiscard]] constexpr T absolute() const;
	[[nodiscard]] constexpr T absoluteSquared() const;
	constexpr void normalize();
	[[nodiscard]] constexpr Vector3<T> normalized() const;
	[[nodiscard]] constexpr bool isZero() const{ return x == 0 && y == 0 && z == 0; }
	[[nodiscard]] constexpr T dotProduct(const Vector3 &right);
	[[nodiscard]] constexpr Vector3<T> operator+(const Vector3<T> &right) const;
	[[nodiscard]] constexpr Vector3<T> operator-(const Vector3<T> &right) const;
	[[nodiscard]] constexpr Vector3<T> operator-() const;
	[[nodiscard]] constexpr Vector3<T> operator*(const Vector3 &right) const;
	[[nodiscard]] constexpr Vector3<T> operator/(const Vector3 &right) const;
	constexpr Vector3<T>& operator+=(const Vector3 &right);
	constexpr Vector3<T>& operator-=(const Vector3 &right);
	constexpr Vector3<T>& operator*=(const Vector3 &right);
	constexpr Vector3<T>& operator/=(const Vector3 &right);
};

// Implementation details

constexpr Quaternion Quaternion::fromYawPitchRoll(const f32 &yaw, const f32 &pitch, const f32 &roll) {
	//  Roll first, about axis the object is facing, then
	//  pitch upward, then yaw to face into the new heading
	Quaternion result{};

	const f32 halfRoll = roll * 0.5f;
	const f32 sr = std::sin(halfRoll);
	const f32 cr = std::cos(halfRoll);

	const f32 halfPitch = pitch * 0.5f;
	const f32 sp = std::sin(halfPitch);
	const f32 cp = std::cos(halfPitch);

	const f32 halfYaw = yaw * 0.5f;
	const f32 sy = std::sin(halfYaw);
	const f32 cy = std::cos(halfYaw);

	result.x = cy * sp * cr + sy * cp * sr;
	result.y = sy * cp * cr - cy * sp * sr;
	result.z = cy * cp * sr - sy * sp * cr;
	result.w = cy * cp * cr + sy * sp * sr;

	return result;
}

template<typename T> requires arithmetic<T>
constexpr Matrix4x4<T> Matrix4x4<T>::fromYawPitchRoll(const f32 &yaw, const f32 &pitch, const f32 &roll) {
	const Quaternion quaternion = Quaternion::fromYawPitchRoll(yaw, pitch, roll);
	return fromQuaternion(quaternion);
}

template<typename T> requires arithmetic<T>
constexpr Matrix4x4<T> Matrix4x4<T>::fromQuaternion(const Quaternion &quaternion) {
	Matrix4x4 result{};

	const f32 xx = quaternion.x * quaternion.x;
	const f32 yy = quaternion.y * quaternion.y;
	const f32 zz = quaternion.z * quaternion.z;

	const f32 xy = quaternion.x * quaternion.y;
	const f32 wz = quaternion.z * quaternion.w;
	const f32 xz = quaternion.z * quaternion.x;
	const f32 wy = quaternion.y * quaternion.w;
	const f32 yz = quaternion.y * quaternion.z;
	const f32 wx = quaternion.x * quaternion.w;

	result.field[0] = static_cast<T>(1.0f - 2.0f * (yy + zz));
	result.field[1] = static_cast<T>(2.0f * (xy + wz));
	result.field[2] = static_cast<T>(2.0f * (xz - wy));
	result.field[3] = 0;
	result.field[4] = static_cast<T>(2.0f * (xy - wz));
	result.field[5] = static_cast<T>(1.0f - 2.0f * (zz + xx));
	result.field[6] = static_cast<T>(2.0f * (yz + wx));
	result.field[7] = 0;
	result.field[8] = static_cast<T>(2.0f * (xz + wy));
	result.field[9] = static_cast<T>(2.0f * (yz - wx));
	result.field[10] = static_cast<T>(1.0f - 2.0f * (yy + xx));
	result.field[11] = 0;
	result.field[12] = 0;
	result.field[13] = 0;
	result.field[14] = 0;
	result.field[15] = 1;

	return result;
}

template<typename T> requires arithmetic<T>
constexpr Vector3<T> Vector3<T>::transform(const Matrix4x4<T> &matrix) const {
	Vector3 newVec = *this;
	newVec.x = x * matrix.field[0] + y * matrix.field[4] + z * matrix.field[8] + matrix.field[12];
	newVec.y = x * matrix.field[1] + y * matrix.field[5] + z * matrix.field[9] + matrix.field[13];
	newVec.z = x * matrix.field[2] + y * matrix.field[6] + z * matrix.field[10] + matrix.field[14];
	return newVec;
}

template<typename T> requires arithmetic<T>
constexpr f32 Vector3<T>::calculateAngle(const Vector3<T> &other) const {
	// example: α = arccos[(xa * xb + ya * yb + za * zb) / (√(xa2 + ya2 + za2) * √(xb2 + yb2 + zb2))]
	const f32 dotproduct = x * other.x + y * other.y + z * other.z;
	const f32 lengths = std::sqrt(x * x + y * y + z * z) * std::sqrt(other.x * other.x + other.y * other.y + other.z * other.z);

	return std::acos(dotproduct / lengths);
}

template<typename T> requires arithmetic<T>
constexpr Vector3<T> Vector3<T>::getForwardVectorForRidersRotation() const {
	/*
	f1 = single 0
	f0 = single -1
	*/
	Vector3<T> result{};
	const Matrix4x4<T> matrix = Matrix4x4<T>::fromYawPitchRoll(y, x, z);

	static const Vector3<T> forward = {0, 0, -1};
	result = forward.transform(matrix);
	return result.normalized();
}

template<typename T> requires arithmetic<T>
constexpr T Vector3<T>::absolute() const {
	return std::sqrt(absoluteSquared());
}

template<typename T> requires arithmetic<T>
constexpr T Vector3<T>::absoluteSquared() const {
	T ret = 0;
	ret += x * x;
	ret += y * y;
	ret += z * z;
	return ret;
}

template<typename T> requires arithmetic<T>
constexpr void Vector3<T>::normalize() {
	*this = normalized();
}

template<typename T> requires arithmetic<T>
constexpr Vector3<T> Vector3<T>::normalized() const {
	T inverseOfLength = 1/absolute();
	return {
			x * inverseOfLength,
			y * inverseOfLength,
			z * inverseOfLength
	};
}

template<typename T> requires arithmetic<T>
constexpr T Vector3<T>::dotProduct(const Vector3 &right) {
	T ret = 0;
	ret += x * right.x;
	ret += y * right.y;
	ret += z * right.z;
	return ret;
}

template<typename T> requires arithmetic<T>
constexpr Vector3<T> Vector3<T>::operator+(const Vector3<T> &right) const {
	Vector3 temp(*this);
	temp += right;
	return temp;
}

template<typename T> requires arithmetic<T>
constexpr Vector3<T> Vector3<T>::operator-(const Vector3<T> &right) const {
	Vector3 temp(*this);
	temp -= right;
	return temp;
}

template<typename T> requires arithmetic<T>
constexpr Vector3<T> Vector3<T>::operator-() const {
	return {-x, -y, -z};
}

template<typename T> requires arithmetic<T>
constexpr Vector3<T> Vector3<T>::operator*(const Vector3 &right) const {
	Vector3 temp(*this);
	temp *= right;
	return temp;
}

template<typename T> requires arithmetic<T>
constexpr Vector3<T> Vector3<T>::operator/(const Vector3 &right) const {
	Vector3 temp(*this);
	temp /= right;
	return temp;
}

template<typename T> requires arithmetic<T>
constexpr Vector3<T> &Vector3<T>::operator+=(const Vector3 &right) {
	x += right.x;
	y += right.y;
	z += right.z;
	return *this;
}

template<typename T> requires arithmetic<T>
constexpr Vector3<T> &Vector3<T>::operator-=(const Vector3 &right) {
	x -= right.x;
	y -= right.y;
	z -= right.z;
	return *this;
}

template<typename T> requires arithmetic<T>
constexpr Vector3<T> &Vector3<T>::operator*=(const Vector3 &right) {
	x *= right.x;
	y *= right.y;
	z *= right.z;
	return *this;
}

template<typename T> requires arithmetic<T>
constexpr Vector3<T> &Vector3<T>::operator/=(const Vector3 &right) {
	x /= right.x;
	y /= right.y;
	z /= right.z;
	return *this;
}

using Vector3F = Vector3<f32>;
using Euler = Vector3<s32>;
using Matrix3x3F = Matrix3x3<f32>;
//using Matrix4x4F = Matrix4x4<f32>;