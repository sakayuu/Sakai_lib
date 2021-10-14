#include "GameOverScene.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>

//GameOverScene::GameOverScene()
//{
//	
//}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize(DX_Init* dx_Init, Input* input, Audio* audio, Score* score)
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
	Sprite::LoadTexture(1, L"Resources/Textures/GameOver.png");
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	// 3Dオブジェクト生成

	camera->SetTarget({ 0,1,0 });
	camera->SetDistance(3.0f);

	isSceneEnd = false;
	isNextScene = "GamePlay";
}

void GameOverScene::Update()
{
	camera->Update();

	obj3DManager->Update();

	std::ostringstream debugstr;
	debugstr.str("");
	debugstr.clear();

	const auto& shot = score->numObShot;
	debugstr << "Shot -"
		<< std::fixed << std::setprecision(2)
		<< shot << "-";
	debugText.Print(debugstr.str(), 75, 75, 5.0f);

	debugstr.str("");
	debugstr.clear();

	const auto& hit = score->hit;
	debugstr << "Hit -"
		<< std::fixed << std::setprecision(2)
		<< hit << "-";
	debugText.Print(debugstr.str(), 75, 225, 5.0f);

	debugstr.str("");
	debugstr.clear();

	debugText.Print("LeftClick - GameScene", 150, 450, 3.0f);
	debugstr.str("");
	debugstr.clear();
	debugText.Print("MiddleClick - TitleScene", 150, 600, 3.0f);

	if (input->TriggerMouseLeft()) {
		isNextScene = "GamePlay";
		isSceneEnd = true;
	}
	if (input->TriggerMouseMiddle()) {
		isNextScene = "Title";
		isSceneEnd = true;
	}
	//audio->PlayWave("Resources/Alarm01.wav");
}

void GameOverScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dx_Init->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	//spriteBG->Draw();

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
