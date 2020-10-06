#pragma once
#include <cmath>

// Vector2D�N���X
struct Vector2D
{
	float x;
	float y;

	// �f�t�H���g�R���X�g���N�^
	Vector2D() = default;

	// �����t���R���X�g���N�^
	constexpr Vector2D(float _x, float _y)
		:x(_x), y(_y) {}

	// �Εӂ̒���
	float Length() const {
		return sqrt(LengthSquare());
	}

	// �x�N�g���̒����̓��
	constexpr float LengthSquare() const {
		return Dot(*this);
	}

	// ��������̃x�N�g���Ƃ̓���
	constexpr float Dot(const Vector2D& vec) const {
		return x * vec.x + y * vec.y;
	}

	// ��������̃x�N�g���Ƃ̋���
	float DistanceFrom(const Vector2D& vec) const {
		return (vec - *this).Length();
	}

	// ���K��
	Vector2D Normalized() const {
		return *this / Length();
	}

	// ���Z�q�I�[�o�[���[�h�u+�v�P��
	constexpr Vector2D operator +() const {
		return *this;
	}

	// ���Z�q�I�[�o�[���[�h�u+�v2��
	constexpr Vector2D operator +(const Vector2D& vec) const {
		return { x + vec.x,y + vec.y };
	}

	// ���Z�q�I�[�o�[���[�h�u+=�v
	Vector2D& operator +=(const Vector2D& vec) {
		x += vec.x;
		y += vec.y;
		return *this;
	}

	// ���Z�q�I�[�o�[���[�h�u-�v�P��
	constexpr Vector2D operator -() const {
		return { -x,-y };
	}

	// ���Z�q�I�[�o�[���[�h�u-�v2��
	constexpr Vector2D operator -(const Vector2D& vec) const {
		return { x - vec.x,y - vec.y };
	}

	// ���Z�q�I�[�o�[���[�h�u-=�v
	Vector2D& operator -=(const Vector2D& vec) {
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	// ���Z�q�I�[�o�[���[�h�u*�v2��
	constexpr Vector2D operator *(float s)const {
		return { x * s,y * s };
	}

	// ���Z�q�I�[�o�[���[�h�u*=�v
	Vector2D& operator *= (float s) {
		x *= s;
		y *= s;
		return *this;
	}

	// ���Z�q�I�[�o�[���[�h�u/�v2��
	constexpr Vector2D operator /(float s)const {
		return { x / s,y / s };
	}

	// ���Z�q�I�[�o�[���[�h�u/=�v
	Vector2D& operator /= (float s) {
		x /= s;
		y /= s;
		return *this;
	}

	//// �O��
	//float Cross(const Vector2D& vec);

};

// Vec2����ɂ���2��
inline Vector2D operator *(float s, const Vector2D& vec)
{
	return { s * vec.x,s * vec.y };
}

inline Vector2D operator /(float s, const Vector2D& vec)
{
	return { s / vec.x,s / vec.y };
}
