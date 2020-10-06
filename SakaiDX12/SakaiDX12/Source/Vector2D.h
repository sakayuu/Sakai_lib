#pragma once
#include <cmath>

// Vector2Dクラス
struct Vector2D
{
	float x;
	float y;

	// デフォルトコンストラクタ
	Vector2D() = default;

	// 引数付きコンストラクタ
	constexpr Vector2D(float _x, float _y)
		:x(_x), y(_y) {}

	// 斜辺の長さ
	float Length() const {
		return sqrt(LengthSquare());
	}

	// ベクトルの長さの二乗
	constexpr float LengthSquare() const {
		return Dot(*this);
	}

	// もう一方のベクトルとの内積
	constexpr float Dot(const Vector2D& vec) const {
		return x * vec.x + y * vec.y;
	}

	// もう一方のベクトルとの距離
	float DistanceFrom(const Vector2D& vec) const {
		return (vec - *this).Length();
	}

	// 正規化
	Vector2D Normalized() const {
		return *this / Length();
	}

	// 演算子オーバーロード「+」単項
	constexpr Vector2D operator +() const {
		return *this;
	}

	// 演算子オーバーロード「+」2項
	constexpr Vector2D operator +(const Vector2D& vec) const {
		return { x + vec.x,y + vec.y };
	}

	// 演算子オーバーロード「+=」
	Vector2D& operator +=(const Vector2D& vec) {
		x += vec.x;
		y += vec.y;
		return *this;
	}

	// 演算子オーバーロード「-」単項
	constexpr Vector2D operator -() const {
		return { -x,-y };
	}

	// 演算子オーバーロード「-」2項
	constexpr Vector2D operator -(const Vector2D& vec) const {
		return { x - vec.x,y - vec.y };
	}

	// 演算子オーバーロード「-=」
	Vector2D& operator -=(const Vector2D& vec) {
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	// 演算子オーバーロード「*」2項
	constexpr Vector2D operator *(float s)const {
		return { x * s,y * s };
	}

	// 演算子オーバーロード「*=」
	Vector2D& operator *= (float s) {
		x *= s;
		y *= s;
		return *this;
	}

	// 演算子オーバーロード「/」2項
	constexpr Vector2D operator /(float s)const {
		return { x / s,y / s };
	}

	// 演算子オーバーロード「/=」
	Vector2D& operator /= (float s) {
		x /= s;
		y /= s;
		return *this;
	}

	//// 外積
	//float Cross(const Vector2D& vec);

};

// Vec2が後にくる2項
inline Vector2D operator *(float s, const Vector2D& vec)
{
	return { s * vec.x,s * vec.y };
}

inline Vector2D operator /(float s, const Vector2D& vec)
{
	return { s / vec.x,s / vec.y };
}
