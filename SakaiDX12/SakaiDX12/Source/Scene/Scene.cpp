#include "Scene.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete(sprite);
}

void GameScene::Initialize(DX_Init* dx_Init, Input* input)
{
	// nullptrチェック
	assert(dx_Init);
	assert(input);
	//assert(audio);

	this->dx_Init = dx_Init;
	this->input = input;
	//this->audio = audio;

	//// デバッグテキスト用テクスチャ読み込み
	//if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
	//	assert(0);
	//	return;
	//}
	// デバッグテキスト初期化
	//debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/Textures/texture.png")) {
		assert(0);
		return;
	}
	sprite = Sprite::Create(1, { Window::window_width / 2,Window::window_height / 2 });
	// 背景スプライト生成
	//spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	// 3Dオブジェクト生成
	//object3d = Object3d::Create();
	//object3d->Update();

	
}

void GameScene::Update()
{
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

	// カメラ移動
	//if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	//{
	//	if (input->PushKey(DIK_W)) { Object3d::CameraMoveVector({ 0.0f,+1.0f,0.0f }); }
	//	else if (input->PushKey(DIK_S)) { Object3d::CameraMoveVector({ 0.0f,-1.0f,0.0f }); }
	//	if (input->PushKey(DIK_D)) { Object3d::CameraMoveVector({ +1.0f,0.0f,0.0f }); }
	//	else if (input->PushKey(DIK_A)) { Object3d::CameraMoveVector({ -1.0f,0.0f,0.0f }); }
	//}

	//object3d->Update();

	//audio->PlayWave("Resources/Alarm01.wav");
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dx_Init->GetCommandList().Get();

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
	//Object3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	//object3d->Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	//Object3d::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite->Draw();
	// デバッグテキストの描画
	//debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
