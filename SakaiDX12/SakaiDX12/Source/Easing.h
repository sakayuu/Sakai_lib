#pragma once
#include <cmath>

struct Easing
{
public:
	
	/// <param name="start">開始速度</param>
	/// <param name="end">開始と終了の差(限界速度)</param>
	/// <param name="time">時間</param>
	/// <returns>加速度を返す</returns>
	float easeOutQuart(float start, float end, float time)const {
		time--;
		return -end * (pow(time, 4) - 1) + start;
	}

};