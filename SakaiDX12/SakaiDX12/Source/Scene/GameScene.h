#pragma once

#include "../Engine/Window.h"
#include "../Engine/Dx12.h"
#include "../Input/Input.h"
#include "../Graphic/2D/Sprite.h"
#include "../Graphic/3D/Obj3DManager.h"
#include "../Graphic/3D/ParticleManager.h"
#include "../Scene/Scene.h"
#include "../Collision/Collision.h"
#include <DirectXMath.h>
#include <vector>

//#include "Object3d.h"
//#include "DebugText.h"
//#include "Audio.h"

#include "../Easing.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public Scene
{
private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

private:

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene(const std::string& sceneName) :Scene(sceneName) {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DX_Init* dx_Init, Input* input, Audio* audio, Score* score) override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:

	void ShotCheck();

	void HitCheck();

	/// <summary>
	/// パーティクル生成
	/// </summary>
	void CreateParticles(XMFLOAT3 pos);

	void TimeCheck();

	void BulletUpdate();

	void BulletMove(XMFLOAT3 vec);
private:
	Sprite* sprite = nullptr;
	Obj3DManager* obj3DManager = nullptr;
	ParticleManager* particleManager = nullptr;

	//当たり判定
	Sphere sphere;
	Sphere sphere2;

	float timer = 0.0f;
	float timeLimit = 600;
	int shotPoint = 0;
	int breakPoint = 0;
	int mokuhyoPoint = 10;

	float g = 0.98f;
	bool shotFlag = false;
	XMFLOAT3 bulletVec = { 0,0,0 };

	float lightColor1[3] = { 0,0,1 };
	float lightColor2[3] = { 0,0,1 };

};
