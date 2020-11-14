#pragma once
#include <cmath>

// Vector3D�N���X
struct Vector3D
{
	float x;
	float y;
	float z;

	// �f�t�H���g�R���X�g���N�^
	Vector3D() = default;

	// �����t���R���X�g���N�^
	constexpr Vector3D(float _x, float _y, float _z)
		:x(_x), y(_y), z(_z) {}

	// �Εӂ̒���
	float Length() const {
		return sqrt(LengthSquare());
	}

	// �x�N�g���̒����̓��
	constexpr float LengthSquare() const {
		return Dot(*this);
	}

	// ��������̃x�N�g���Ƃ̓���
	constexpr float Dot(const Vector3D& vec) const {
		return x * vec.x + y * vec.y + z * vec.z;
	}

	// ��������̃x�N�g���Ƃ̊O��
	constexpr Vector3D Cross(const Vector3D& vec) const {
		return Vector3D(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
	}

	// ��������̃x�N�g���Ƃ̋���
	float DistanceFrom(const Vector3D& vec) const {
		return (vec - *this).Length();
	}

	// ���K��
	Vector3D Normalized() const {
		return *this / Length();
	}

	// ���Z�q�I�[�o�[���[�h�u+�v�P��
	constexpr Vector3D operator +() const {
		return *this;
	}

	// ���Z�q�I�[�o�[���[�h�u+�v2��
	constexpr Vector3D operator +(const Vector3D& vec) const {
		return { x + vec.x,y + vec.y,z + vec.z };
	}

	// ���Z�q�I�[�o�[���[�h�u+=�v
	Vector3D& operator +=(const Vector3D& vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	// ���Z�q�I�[�o�[���[�h�u-�v�P��
	constexpr Vector3D operator -() const {
		return { -x,-y,-z };
	}

	// ���Z�q�I�[�o�[���[�h�u-�v2��
	constexpr Vector3D operator -(const Vector3D& vec) const {
		return { x - vec.x,y - vec.y ,z - vec.z };
	}

	// ���Z�q�I�[�o�[���[�h�u-=�v
	Vector3D& operator -=(const Vector3D& vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	// ���Z�q�I�[�o�[���[�h�u*�v2��
	constexpr Vector3D operator *(float s)const {
		return { x * s,y * s ,z * s };
	}

	// ���Z�q�I�[�o�[���[�h�u*=�v
	Vector3D& operator *= (float s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	// ���Z�q�I�[�o�[���[�h�u/�v2��
	constexpr Vector3D operator /(float s)const {
		return { x / s,y / s ,z / s };
	}

	// ���Z�q�I�[�o�[���[�h�u/=�v
	Vector3D& operator /= (float s) {
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}


};

// Vec2����ɂ���2��
inline Vector3D operator *(float s, const Vector3D& vec)
{
	return { s * vec.x,s * vec.y ,s * vec.z };
}

inline Vector3D operator /(float s, const Vector3D& vec)
{
	return { s / vec.x,s / vec.y ,s / vec.z };
}

// �x�N�g������v���Ă��邩�`�F�b�N
inline bool operator ==(Vector3D v, const Vector3D& vec) {
	return v.x == vec.x && v.y == vec.y && v.z == vec.z;
}

// �x�N�g������v���Ă��Ȃ����`�F�b�N
inline bool operator !=(Vector3D v, const Vector3D& vec) {
	return !(v == vec);
}
