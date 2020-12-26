#include "GameScene.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	safe_delete(sprite);
}

void GameScene::Initialize(DX_Init* dx_Init, Input* input, Audio* audio)
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

	CreateLight(2);
	light[0]->SetLightColor({ 1,0,0 });
	light[1]->SetLightColor({ 0,0,1 });

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/Font/debugfont.png")) {
		assert(0);
		return;
	}
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/Textures/Sayu.png")) {
		assert(0);
		return;
	}
	sprite = Sprite::Create(1, { Window::window_width / 2,Window::window_height / 2 });
	// 背景スプライト生成
	//spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	// 3Dオブジェクト生成
	object3d = Object3d::Create();
	object3d->AddModel("sphere", Model::CreateFromOBJ("sphere", true));

	object3d->SetPosition("sphere", { -1,1,0 });

	camera->SetTarget({ 0,1,0 });
	camera->SetDistance(3.0f);
}

void GameScene::Update()
{
	camera->Update();

	LightUpdate();

	// オブジェクト移動
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		// 現在の座標を取得
		XMFLOAT2 position = sprite->GetPosition();
		Easing ease;
		// 移動後の座標を計算
		if (input->PushKey(DIK_UP)) { position.y += 1.0f; }
		else if (input->PushKey(DIK_DOWN)) { position.y -= 1.0f; }
		if (input->PushKey(DIK_RIGHT)) { position.x += 1.0f; }
		else if (input->PushKey(DIK_LEFT)) { position.x -= 1.0f; }

		// 座標の変更を反映
		//object3d->SetPosition(position);
		sprite->SetPosition(position);
	}
	std::ostringstream debugstr;
	debugstr.str("");
	debugstr.clear();
	const XMFLOAT2& spritePos = sprite->GetPosition();
	debugstr << "SpritePos("
		<< std::fixed << std::setprecision(2)
		<< spritePos.x << ","
		<< spritePos.y << ")";
	debugText.Print(debugstr.str(), 50, 50, 1.0f);

	// カメラ移動
	//if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	//{
	//	if (input->PushKey(DIK_W)) { Object3d::CameraMoveVector({ 0.0f,+1.0f,0.0f }); }
	//	else if (input->PushKey(DIK_S)) { Object3d::CameraMoveVector({ 0.0f,-1.0f,0.0f }); }
	//	if (input->PushKey(DIK_D)) { Object3d::CameraMoveVector({ +1.0f,0.0f,0.0f }); }
	//	else if (input->PushKey(DIK_A)) { Object3d::CameraMoveVector({ -1.0f,0.0f,0.0f }); }
	//}

	object3d->Update();

	//audio->PlayWave("Resources/Alarm01.wav");
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dx_Init->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	//Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	//spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	//Sprite::PostDraw();
	// 深度バッファクリア
	dx_Init->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	object3d->Draw(light);

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

void GameScene::CreateLight(UINT num)
{
	if (num <= 0)
		return;
	for (int i = 0; i < num; i++) {
		Light* l;
		l = l->Create();
		light.push_back(l);
	}
}

void GameScene::LightUpdate()
{
	if (light.size() <= 0)
		return;
	for (auto& l : light)
		l->Update();
}
