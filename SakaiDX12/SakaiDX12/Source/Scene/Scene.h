#pragma once

//#include "SafeDelete.h"
#include "../Engine/Dx12.h"
#include "../Engine/Input.h"
#include "../Graphic/2D/Sprite.h"
#include "../Engine/Window.h"
#include <DirectXMath.h>
//#include "Object3d.h"
//#include "DebugText.h"
//#include "Audio.h"

#include "../Easing.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

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
	void Initialize(DX_Init* dx_Init, Input* input);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DX_Init* dx_Init = nullptr;
	Input* input = nullptr;
	//Audio* audio = nullptr;
	//DebugText debugText;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	Sprite* sprite = nullptr;
	//Object3d* object3d = nullptr;
};

