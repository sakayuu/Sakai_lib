#include "TitleScene.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace DirectX;

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize(DX_Init* dx_Init, Input* input, Audio* audio, Score* score)
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
	if (!Sprite::LoadTexture(1, L"Resources/Textures/Shageki.png")) {
		assert(0);
		return;
	}
	sprite = Sprite::Create(1, { 130,0 });
	// 背景スプライト生成
	Sprite::LoadTexture(2, L"Resources/Textures/shagekizyo.jpg");
	spriteBG = Sprite::Create(2, { 0.0f,0.0f });
	// 3Dオブジェクト生成
	obj3DManager->AddModel("Gun", "Gun", true);
	obj3DManager->AddObj3D("Gun1", "Gun");

	obj3DManager->SetPosition("Gun1", { 0,1,-2 });

	camera->SetTarget({ 0,1,0 });
	camera->SetDistance(3.0f);

	isSceneEnd = false;
	isNextScene = "GamePlay";
}

void TitleScene::Update()
{
	camera->Update();

	//オブジェクトの回転
	{
		XMFLOAT3 rot = obj3DManager->GetRotation("Gun1");
		if (rot.y >= 360)
			rot.y = 0;
		rot.y += 1.0f;
		obj3DManager->SetRotation("Gun1", rot);

		/*std::ostringstream debugstr;
		debugstr << "Gun1Rot("
			<< std::fixed << std::setprecision(2)
			<< rot.y << ")";
		debugText.Print(debugstr.str(), 50, 100, 1.0f);*/
	}
	/*std::ostringstream debugstr;
	debugstr.str("");
	debugstr.clear();
	const XMFLOAT2& spritePos = sprite->GetPosition();
	debugstr << "SpritePos("
		<< std::fixed << std::setprecision(2)
		<< spritePos.x << ","
		<< spritePos.y << ")";
	debugText.Print(debugstr.str(), 50, 50, 1.0f);*/

	// カメラ移動
	//if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	//{
	//	if (input->PushKey(DIK_W)) { Object3d::CameraMoveVector({ 0.0f,+1.0f,0.0f }); }
	//	else if (input->PushKey(DIK_S)) { Object3d::CameraMoveVector({ 0.0f,-1.0f,0.0f }); }
	//	if (input->PushKey(DIK_D)) { Object3d::CameraMoveVector({ +1.0f,0.0f,0.0f }); }
	//	else if (input->PushKey(DIK_A)) { Object3d::CameraMoveVector({ -1.0f,0.0f,0.0f }); }
	//}

	obj3DManager->Update();

	if (input->TriggerMouseLeft())
		isSceneEnd = true;

	//audio->PlayWave("Resources/Alarm01.wav");
}

void TitleScene::Draw()
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
	obj3DManager->Draw();

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
	sprite->Draw();
	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
