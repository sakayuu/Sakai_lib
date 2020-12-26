#include "Window.h"
#include "../Input/Input.h"
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
	audio = new Audio();
	scene = new GameScene();

	window->CreateGameWindow();  //ウィンドウ作成
	dx_Init->Initialize(window); //DirectX初期化


	// 入力の初期化
	if (!input->Initialize(window->GetInstance(), window->GetHwnd())) {
		assert(0);
		return 1;
	}
	// オーディオの初期化
	if (!audio->Initialize()) {
		assert(0);
		return 1;
	}
	// スプライト静的初期化
	if (!Sprite::StaticInitialize(dx_Init->GetDevice(), Window::window_width, Window::window_height)) {
		assert(0);
		return 1;
	}
	Object3d::StaticInitialize(dx_Init->GetDevice());
	Light::StaticInitialize(dx_Init->GetDevice());

	scene->Initialize(dx_Init, input, audio);

	return true;
}

void Engine::Run()
{
	FPS* fpsFix = new FPS(60); //FPS固定クラス

	while (true) {
		//メッセージ処理
		if (window->ProcessMessage()) { break; }

		Update();

		Draw();

		fpsFix->TimeAdjustment();
	}
	safe_delete(fpsFix);
}

void Engine::Update()
{
	//入力
	input->Update();

	//シーンの更新
	scene->Update();
}

void Engine::Draw()
{
	//描画開始
	dx_Init->BeginDraw();

	scene->Draw();

	//描画終了
	dx_Init->EndDraw();
}

void Engine::Terminate()
{
	safe_delete(scene);
	safe_delete(input);
	safe_delete(audio);
	safe_delete(dx_Init);

	window->TerminateGameWindow();
	safe_delete(window);
}