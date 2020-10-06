#pragma once
#include <fstream>
#include <sstream>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

class FPS
{
public:
	//コンストラクタ
	FPS();
	//@param frame 固定するFPSを指定
	FPS(int frame);
	void Initialize();

	//FPSを固定するため処理時間に余裕がある場合その分待つ
	void TimeAdjustment();

private:
	//コンストラクタで受け取った値でFPS固定
	const double FRAME_TIME = 0.0;

	//計測開始時間
	LARGE_INTEGER mTimeStart;
	//計測終了時間
	LARGE_INTEGER mTimeEnd;
	//計測周波数
	LARGE_INTEGER mTimeFreq;
};