#pragma once
#include <cmath>

struct Easing
{
public:
	
	/// <param name="start">�J�n���x</param>
	/// <param name="end">�J�n�ƏI���̍�(���E���x)</param>
	/// <param name="time">����</param>
	/// <returns>�����x��Ԃ�</returns>
	float easeOutQuart(float start, float end, float time)const {
		time--;
		return -end * (pow(time, 4) - 1) + start;
	}

};