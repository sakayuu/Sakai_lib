#pragma once
#include <cmath>

// Vector3Dクラス
struct Vector3D
{
	float x;
	float y;
	float z;

	// デフォルトコンストラクタ
	Vector3D() = default;

	// 引数付きコンストラクタ
	constexpr Vector3D(float _x, float _y, float _z)
		:x(_x), y(_y), z(_z) {}

	// 斜辺の長さ
	float Length() const {
		return sqrt(LengthSquare());
	}

	// ベクトルの長さの二乗
	constexpr float LengthSquare() const {
		return Dot(*this);
	}

	// もう一方のベクトルとの内積
	constexpr float Dot(const Vector3D& vec) const {
		return x * vec.x + y * vec.y + z * vec.z;
	}

	// もう一方のベクトルとの外積
	constexpr Vector3D Cross(const Vector3D& vec) const {
		return Vector3D(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
	}

	// もう一方のベクトルとの距離
	float DistanceFrom(const Vector3D& vec) const {
		return (vec - *this).Length();
	}

	// 正規化
	Vector3D Normalized() const {
		return *this / Length();
	}

	// 演算子オーバーロード「+」単項
	constexpr Vector3D operator +() const {
		return *this;
	}

	// 演算子オーバーロード「+」2項
	constexpr Vector3D operator +(const Vector3D& vec) const {
		return { x + vec.x,y + vec.y,z + vec.z };
	}

	// 演算子オーバーロード「+=」
	Vector3D& operator +=(const Vector3D& vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	// 演算子オーバーロード「-」単項
	constexpr Vector3D operator -() const {
		return { -x,-y,-z };
	}

	// 演算子オーバーロード「-」2項
	constexpr Vector3D operator -(const Vector3D& vec) const {
		return { x - vec.x,y - vec.y ,z - vec.z };
	}

	// 演算子オーバーロード「-=」
	Vector3D& operator -=(const Vector3D& vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	// 演算子オーバーロード「*」2項
	constexpr Vector3D operator *(float s)const {
		return { x * s,y * s ,z * s };
	}

	// 演算子オーバーロード「*=」
	Vector3D& operator *= (float s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	// 演算子オーバーロード「/」2項
	constexpr Vector3D operator /(float s)const {
		return { x / s,y / s ,z / s };
	}

	// 演算子オーバーロード「/=」
	Vector3D& operator /= (float s) {
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}


};

// Vec2が後にくる2項
inline Vector3D operator *(float s, const Vector3D& vec)
{
	return { s * vec.x,s * vec.y ,s * vec.z };
}

inline Vector3D operator /(float s, const Vector3D& vec)
{
	return { s / vec.x,s / vec.y ,s / vec.z };
}

// ベクトルが一致しているかチェック
inline bool operator ==(Vector3D v, const Vector3D& vec) {
	return v.x == vec.x && v.y == vec.y && v.z == vec.z;
}

// ベクトルが一致していないかチェック
inline bool operator !=(Vector3D v, const Vector3D& vec) {
	return !(v == vec);
}
