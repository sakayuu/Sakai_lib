#include "Window.h"
#include "Graphics.h"
#include "Input.h"
#include "Engine.h"
#include "FramerateFix.h"

Engine& Engine::Instance() {
	static Engine instance;
	return instance;
}

Engine::Engine()
{

}
Engine::~Engine()
{
	
}

bool Engine::Init()
{

	window = new Window();
	window->CreateGameWindow();

	dx_Init = new DX_Init(window->GetHwnd());

	// 入力の初期化
	input = new Input();
	if (!input->Initialize(window->GetInstance(), window->GetHwnd())) {
		assert(0);
		return 1;
	}
	// スプライト静的初期化
	if (!Sprite::StaticInitialize(dx_Init->GetDevice().Get(), Window::window_width, Window::window_height)) {
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
	//whileの外で一度ウィンドウを表示

	FPS* fpsFix = new FPS(60); //FPS固定クラス
	MSG msg = {};
	scene = new GameScene();
	scene->Initialize(dx_Init, input);

	while (true)
	{
		if (window->ProcessMessage()) {
			break;
		}
		input->Update();

		//描画準備
		scene->Update();

		dx_Init->BeginDraw();

		scene->Draw();

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