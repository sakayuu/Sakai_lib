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
	FPS(double frame);

	void Initialize();

	//FPSを固定するため処理時間に余裕がある場合その分待つ
	double TimeAdjustment();

private:
	//コンストラクタで受け取った値でFPS固定
	const double FRAME_TIME = 0.0;

	//計測開始時間
	LARGE_INTEGER mTimeStart;
	//計測終了時間
	LARGE_INTEGER mTimeEnd;
	//計測周波数
	LARGE_INTEGER mTimeFreq;

#ifdef _DEBUG
public:
private:
	int mFrameCount = 0;
	int mDebugCount = 10;
#endif // _DEBUG

};