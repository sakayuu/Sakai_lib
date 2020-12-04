#include "Window.h"
#include "Graphics.h"
#include "Input.h"
#include "Engine.h"
#include "FramerateFix.h"

Engine& Engine::Instance() {
	static Engine instance;
	return instance;
}

Engine::Engine() {}
Engine::~Engine() {}

bool Engine::Init()
{
	window = new Window();
	dx_Init = new DX_Init();
	input = new Input();

	//ウィンドウ作成
	window->CreateGameWindow();

	//DirectX初期化
	dx_Init->Initialize(window);

	// 入力の初期化
	if (!input->Initialize(window->GetInstance(), window->GetHwnd())) {
		assert(0);
		return 1;
	}
	// スプライト静的初期化
	if (!Sprite::StaticInitialize(dx_Init->GetDevice(), Window::window_width, Window::window_height)) {
		assert(0);
		return 1;
	}

	/*if (InitGraphics() == false)
	{
		return false;
	}*/

	/*if (InitInput() == false)
	{
		return false;
	}*/

	return true;
}

void Engine::Run()
{
	FPS* fpsFix = new FPS(60); //FPS固定クラス
	MSG msg = {};

	scene = new GameScene();
	scene->Initialize(dx_Init, input);

	while (true)
	{
		//メッセージ処理
		if (window->ProcessMessage()) { break; }

		//入力
		input->Update();

		//更新
		scene->Update();

		//描画開始
		dx_Init->BeginDraw();

		scene->Draw();

		//描画終了
		dx_Init->EndDraw();

		fpsFix->TimeAdjustment();
	}
}

void Engine::Terminate()
{
	delete(scene);
	delete(input);
	delete(dx_Init);

	window->TerminateGameWindow();
	delete(window);
}