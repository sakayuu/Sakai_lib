#pragma once

#include "../Engine/Window.h"
#include "../Engine/Dx12.h"
#include "../Input/Input.h"
#include "../Graphic/2D/Sprite.h"
#include "../Scene/Scene.h"
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

public: // メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DX_Init* dx_Init, Input* input, Audio* audio) override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	void CreateLight(UINT num);

	void LightUpdate();

private:
	std::vector<Light*> light;
};
