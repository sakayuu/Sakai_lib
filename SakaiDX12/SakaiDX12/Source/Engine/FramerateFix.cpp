#include "FramerateFix.h"

FPS::FPS() :FRAME_TIME(1.0f / 30) {
	Initialize();
}

FPS::FPS(int frame) : FRAME_TIME(1.0f / frame) {
	Initialize();
}

void FPS::Initialize()
{
	//周波数取得
	QueryPerformanceFrequency(&mTimeFreq);
	//計測開始時間の初期化
	QueryPerformanceCounter(&mTimeStart);
}

void FPS::TimeAdjustment()
{
	// 今の時間を取得
	QueryPerformanceCounter(&mTimeEnd);
	//(今の時間 - 前フレームの時間) / 周波数 = 経過時間(秒単位)
	double frameTime = static_cast<double>(mTimeEnd.QuadPart - mTimeStart.QuadPart) / static_cast<double>(mTimeFreq.QuadPart);
	double fps = 0.0;
	//処理に余裕がある場合はその分待つ
	if (frameTime < FRAME_TIME) {
		//Sleepの時間を計算
		DWORD sleepTime = static_cast<DWORD>((FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1);
		Sleep(sleepTime); //寝る
		timeEndPeriod(1);

		return;
	}
	fps = 1 / frameTime;

	//std::stringstream stream;
	//stream << std::to_string(fps).c_str() << " FPS" << std::endl;
	//OutputDebugString(stream.str().c_str());

	mTimeStart = mTimeEnd;
}


