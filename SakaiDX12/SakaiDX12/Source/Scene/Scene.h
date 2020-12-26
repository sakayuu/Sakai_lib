#pragma once

#include "../Engine/Window.h"
#include "../Engine/Dx12.h"
#include "../Engine/SafeDelete.h"
#include "../Input/Input.h"
#include "../Sound/Audio.h"
#include "../Graphic/2D/Sprite.h"
#include "../Graphic/2D/DebugText.h"
#include "../Graphic/3D/Object3d.h"
#include "../Graphic/Camera/DebugCamera.h"
#include <DirectXMath.h>

/// <summary>
/// シーン（抽象クラス）
/// </summary>
class Scene
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

protected: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

public: // メンバ関数（純粋仮想関数）

	//初期化
	virtual void Initialize(DX_Init* dx_Init, Input* input,Audio* audio) = 0;

	//毎フレーム処理
	virtual void Update() = 0;

	//描画
	virtual void Draw() = 0;

protected: // メンバ変数
	DX_Init* dx_Init = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugCamera* camera = nullptr;
	DebugText debugText;

	Sprite* sprite = nullptr;
	Object3d* object3d = nullptr;
};

