#include "GameClearScene.h"

GameClearScene::~GameClearScene()
{
}

void GameClearScene::Initialize(DX_Init* dx_Init, Input* input, Audio* audio, Score* score)
{
	// nullptrチェック
	assert(dx_Init);
	assert(input);
	assert(audio);

	this->dx_Init = dx_Init;
	this->input = input;
	this->audio = audio;

	camera = new DebugCamera(Window::window_width, Window::window_height, input);
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);

	obj3DManager = new Obj3DManager();
	obj3DManager->CreateLight(1);
	obj3DManager->SetLightColor(0, { 1,1,1 });
	obj3DManager->SetLightDir(0, { 1,-1,-1,0 });

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/Font/debugfont.png")) {
		assert(0);
		return;
	}
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	// 背景スプライト生成
	Sprite::LoadTexture(1, L"Resources/Textures/GameClear.png");
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	// 3Dオブジェクト生成
	
	camera->SetTarget({ 0,1,0 });
	camera->SetDistance(3.0f);

	isSceneEnd = false;
	isNextScene = "Title";
}

void GameClearScene::Update()
{
	camera->Update();

	obj3DManager->Update();

	if (input->TriggerMouseLeft())
		isSceneEnd = true;

	//audio->PlayWave("Resources/Alarm01.wav");
}

void GameClearScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dx_Init->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dx_Init->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	//obj3DManager->Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// ここに前景スプライトの描画処理を追加できる
	//sprite->Draw();
	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
