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


#include"../Game/Score.h"


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
	Scene(const std::string& sceneName) : sceneName(sceneName) {};

	//初期化
	virtual void Initialize(DX_Init* dx_Init, Input* input, Audio* audio, Score* score) = 0;

	//毎フレーム処理
	virtual void Update() = 0;

	//描画
	virtual void Draw() = 0;

	virtual bool IsSceneEnd() { return isSceneEnd; }

	virtual std::string IsNextScene() { return isNextScene; }

	std::string GetMySceneName() { return sceneName; }
protected: // メンバ変数
	DX_Init* dx_Init = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugCamera* camera = nullptr;
	DebugText debugText;

	bool isSceneEnd = false;
	std::string isNextScene;

	std::string sceneName;

	Score* score = nullptr;
};

