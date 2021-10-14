#include "GameScene.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>


using namespace DirectX;

//GameScene::GameScene()
//{
//}

GameScene::~GameScene()
{
	safe_delete(sprite);
	safe_delete(obj3DManager);
}

void GameScene::Initialize(DX_Init* dx_Init, Input* input, Audio* audio, Score* score)
{
	// nullptrチェック
	assert(dx_Init);
	assert(input);
	assert(audio);

	this->dx_Init = dx_Init;
	this->input = input;
	this->audio = audio;
	this->score = score;

	camera = new DebugCamera(Window::window_width, Window::window_height, this->input);
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);

	obj3DManager = new Obj3DManager();
	obj3DManager->CreateLight(2);
	obj3DManager->SetLightColor(0, { 1,0,0 });
	obj3DManager->SetLightColor(1, { 0,0,1 });

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/Font/debugfont.png")) {
		assert(0);
		return;
	}
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/Textures/Target.png")) {
		assert(0);
		return;
	}
	sprite = Sprite::Create(1, { Window::window_width / 2,Window::window_height / 2 });
	sprite->SetPosition({
		(float)input->GetMousePoint().x - sprite->GetTexSize().x / 2 ,
		(float)input->GetMousePoint().y - sprite->GetTexSize().y / 2 });
	// 背景スプライト生成
	//spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	// パーティクルマネージャ生成
	particleManager = ParticleManager::Create(this->dx_Init->GetDevice(), camera);

	// 3Dオブジェクト生成
	obj3DManager->AddModel("sphereSmooth", "sphere", true);
	obj3DManager->AddModel("sphereBullet", "sphere", false);
	//Cube* cube = new Cube();
	//obj3DManager->AddPrimitive3D("cube", *cube);

	obj3DManager->AddObj3D("sphere1", "sphereSmooth");
	obj3DManager->AddObj3D("bullet", "sphereBullet");
	obj3DManager->SetScale("bullet", { 0.2f,0.2f ,0.2f });

	//obj3DManager->AddObj3DPrimitive("Cube", "cube1");

	obj3DManager->SetPosition("sphere1", { 10,0,10 });
	obj3DManager->SetPosition("bullet", { 0,0,-10 });

	sphere.center = XMVectorSet(
		obj3DManager->GetPosition("sphere1").x,
		obj3DManager->GetPosition("sphere1").y,
		obj3DManager->GetPosition("sphere1").z, 1);
	sphere.radius = 1.0f; // 半径

	sphere2.center = XMVectorSet(
		obj3DManager->GetPosition("bullet").x,
		obj3DManager->GetPosition("bullet").y,
		obj3DManager->GetPosition("bullet").z, 1);
	sphere2.radius = 0.2f; // 半径

	camera->SetTarget({ 0,1,0 });
	camera->SetDistance(3.0f);

	timer = 0.0f;
	shotPoint = 0;
	breakPoint = 0;
	isSceneEnd = false;
	shotFlag = false;
}

void GameScene::Update()
{
	camera->Update();

	obj3DManager->SetLightColor(0, { lightColor1[0],lightColor1[1],lightColor1[2] });
	obj3DManager->SetLightColor(1, { lightColor2[0],lightColor2[1],lightColor2[2] });

	// マウスカーソル移動
	{
		/*sprite->SetPosition({
		(float)input->GetMousePoint().x - sprite->GetTexSize().x / 2 ,
		(float)input->GetMousePoint().y - sprite->GetTexSize().y / 2 });*/

		sprite->SetPosition({
			Window::window_width / 2 - sprite->GetTexSize().x / 2,
			Window::window_height / 2 - sprite->GetTexSize().y / 2 });
	}

	{
		/*XMVECTOR moveZ = XMVectorSet(0, 0, 10, 0);
		if (input->PushKey(DIK_NUMPAD8) || input->PushKey(DIK_1)) { lay.start += moveZ; }
		else if (input->PushKey(DIK_NUMPAD2) || input->PushKey(DIK_2)) { lay.start -= moveZ; }

		XMVECTOR moveX = XMVectorSet(10, 0, 0, 0);
		if (input->PushKey(DIK_NUMPAD6) || input->PushKey(DIK_3)) { lay.start += moveX; }
		else if (input->PushKey(DIK_NUMPAD4) || input->PushKey(DIK_4)) { lay.start -= moveX; }*/
	}

	particleManager->Update();

	//obj3DManager->SetPosition("lay1", { lay.start.m128_f32[0], lay.start.m128_f32[1], lay.start.m128_f32[2] });

	ShotCheck();

	BulletUpdate();

	TimeCheck();

	obj3DManager->Update();

	/*std::ostringstream debugstr;
	debugstr.str("");
	debugstr.clear();
	const XMFLOAT2& spritePos = sprite->GetPosition();
	debugstr << "SpritePos("
		<< std::fixed << std::setprecision(2)
		<< spritePos.x << ","
		<< spritePos.y << ")";
	debugText.Print(debugstr.str(), 50, 50, 1.0f);

	debugstr.str("");
	debugstr.clear();
	const XMFLOAT3& spherePos = obj3DManager->GetPosition("sphere1");;
	debugstr << "SpherePos("
		<< std::fixed << std::setprecision(2)
		<< spherePos.x << ","
		<< spherePos.y << ","
		<< spherePos.z << ")";
	debugText.Print(debugstr.str(), 50, 100, 1.0f);

	debugstr.str("");
	debugstr.clear();
	const XMFLOAT3& cameraPos = camera->GetEye();
	debugstr << "CameraPos("
		<< std::fixed << std::setprecision(2)
		<< cameraPos.x << ","
		<< cameraPos.y << ","
		<< cameraPos.z << ")";
	debugText.Print(debugstr.str(), 50, 125, 1.0f);

	debugstr.str("");
	debugstr.clear();
	const XMFLOAT3& targetPos = camera->GetTarget();
	debugstr << "TargetPos("
		<< std::fixed << std::setprecision(2)
		<< targetPos.x << ","
		<< targetPos.y << ","
		<< targetPos.z << ")";
	debugText.Print(debugstr.str(), 50, 150, 1.0f);*/

	// カメラ移動
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		if (input->PushKey(DIK_W)) { camera->MoveVector(XMFLOAT3{ 0.0f,+1.0f,0.0f }); }
		else if (input->PushKey(DIK_S)) { camera->MoveVector(XMFLOAT3{ 0.0f,-1.0f,0.0f }); }
		if (input->PushKey(DIK_D)) { camera->MoveVector(XMFLOAT3{ +1.0f,0.0f,0.0f }); }
		else if (input->PushKey(DIK_A)) { camera->MoveVector(XMFLOAT3{ -1.0f,0.0f,0.0f }); }
	}

	//obj3DManager->SetPosition("lay1", { lay.start.m128_f32[0], lay.start.m128_f32[1], lay.start.m128_f32[2] });

	//audio->PlayWave("Resources/Alarm01.wav");
}

void GameScene::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(0, 200));
	ImGui::SetNextWindowSize(ImVec2(500, 200));
	ImGui::Begin("Test");
	ImGui::InputFloat3("lightColor0", lightColor1);
	ImGui::InputFloat3("lightColor1", lightColor2);
	ImGui::End();

	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dx_Init->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	//Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	//spriteBG->Draw();

	// スプライト描画後処理
	//Sprite::PostDraw();
	// 深度バッファクリア
	dx_Init->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	obj3DManager->Draw();

	// パーティクルの描画
	particleManager->Draw(cmdList);

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	sprite->Draw();
	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameScene::ShotCheck()
{
	if (shotFlag)
	{
		bulletVec.y -= g * 0.02f;
		return;
	}

	if (input->TriggerMouseLeft())
	{
		shotFlag = true;
		shotPoint++;
		obj3DManager->SetPosition("bullet", camera->GetEye());

		sphere2.center = XMVectorSet(
			obj3DManager->GetPosition("bullet").x,
			obj3DManager->GetPosition("bullet").y,
			obj3DManager->GetPosition("bullet").z, 1);

		bulletVec = {
			camera->GetTarget().x - camera->GetEye().x,
			camera->GetTarget().y - camera->GetEye().y,
			camera->GetTarget().z - camera->GetEye().z
		};
	}
}

void GameScene::HitCheck()
{
	if (!Collision::CheckSphere2Sphere(sphere, sphere2))
		return;

	/*std::ostringstream debugstr;
	debugstr.str("");
	debugstr.clear();

	debugText.Print("HIT", 50, 200, 1.0f);*/

	breakPoint++;
	CreateParticles(obj3DManager->GetPosition("sphere1"));

	obj3DManager->SetPosition("sphere1", {
		(float)rand() / RAND_MAX * 6 - 6 / 2.0f,
		(float)rand() / RAND_MAX * 4 - 4 / 2.0f,
		(float)rand() / RAND_MAX * 10 });

	sphere.center = XMVectorSet(
		obj3DManager->GetPosition("sphere1").x,
		obj3DManager->GetPosition("sphere1").y,
		obj3DManager->GetPosition("sphere1").z, 1);

	obj3DManager->SetPosition("bullet", { 0,0,-10 });
	sphere2.center = XMVectorSet(
		obj3DManager->GetPosition("bullet").x,
		obj3DManager->GetPosition("bullet").y,
		obj3DManager->GetPosition("bullet").z, 1);

	shotFlag = false;
}

void GameScene::CreateParticles(XMFLOAT3 pos)
{
	for (int i = 0; i < 10; i++) {
		// X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		//const float rnd_pos = 10.0f;
		/*XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;*/

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		// 追加
		particleManager->Add(60, pos, vel, acc, 1.0f, 0.0f);
	}
}

void GameScene::TimeCheck()
{
	/*if (breakPoint >= mokuhyoPoint) {
		isNextScene = "GameClear";
		isSceneEnd = true;
	}*/
	//timer++;
	if (timer >= 600) {
		isNextScene = "GameOver";
		score->SetScore(shotPoint, breakPoint);
		isSceneEnd = true;
	}
}

void GameScene::BulletUpdate()
{
	if (!shotFlag)
		return;

	if (obj3DManager->GetPosition("bullet").x < -20 ||
		obj3DManager->GetPosition("bullet").y < -20 ||
		obj3DManager->GetPosition("bullet").z < -20 ||
		obj3DManager->GetPosition("bullet").x > 20 ||
		obj3DManager->GetPosition("bullet").y > 20 ||
		obj3DManager->GetPosition("bullet").z > 20) {

		shotFlag = false;

		obj3DManager->SetPosition("bullet", { 0,0,-10 });
		sphere2.center = XMVectorSet(
			obj3DManager->GetPosition("bullet").x,
			obj3DManager->GetPosition("bullet").y,
			obj3DManager->GetPosition("bullet").z, 1);
	}

	BulletMove(bulletVec);

	HitCheck();

}

void GameScene::BulletMove(XMFLOAT3 vec)
{
	if (!shotFlag)
		return;

	auto& bPos = obj3DManager->GetPosition("bullet");

	obj3DManager->SetPosition("bullet", {
		bPos.x + vec.x,
		bPos.y + vec.y,
		bPos.z + vec.z });

	sphere2.center = XMVectorSet(
		obj3DManager->GetPosition("bullet").x,
		obj3DManager->GetPosition("bullet").y,
		obj3DManager->GetPosition("bullet").z, 1);

}
