﻿#pragma once

#include "../Engine/Window.h"
#include "../Engine/Dx12.h"
#include "../Input/Input.h"
#include "../Graphic/2D/Sprite.h"
#include "../Graphic/3D/Obj3DManager.h"
#include "../Scene/Scene.h"
#include <DirectXMath.h>
#include <vector>

class TitleScene : public Scene
{
private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

public: // メンバ関数

	// コンストクラタ
	TitleScene(const std::string& sceneName) :Scene(sceneName) {};

	// デストラクタ
	~TitleScene();

	// 初期化
	void Initialize(DX_Init* dx_Init, Input* input, Audio* audio, Score* score) override;

	// 毎フレーム処理
	void Update() override;

	// 描画
	void Draw() override;

private:
	Sprite* sprite = nullptr;
	Sprite* spriteBG = nullptr;
	Obj3DManager* obj3DManager = nullptr;

};