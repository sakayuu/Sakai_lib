#include <stdio.h>
#include "../Engine/Input.h"
#include "../Engine/Graphics.h"
#include "../Texture/Texture.h"
#include "GameScene.h"

// ゲーム本編シーンの初期化
void InitGameScene();
// ゲーム本編シーンのメイン処理
void MainGameScene();
// ゲーム本編シーンの終了
SceneId FinishGameScene();

SceneId UpdateGameScene()
{
	switch (GetCurrentSceneStep())
	{
	case SceneStep::InitStep:
		InitGameScene();
		break;
	case SceneStep::MainStep:
		MainGameScene();
		break;
	case SceneStep::EndStep:
		return FinishGameScene();
		break;
	}

	return SceneId::GameScene;
}

void DrawGameScene()
{
}

void InitGameScene()
{
	LoadTexture("Res/GameBg.png", TEXTURE_CATEGORY_GAME, GameCategoryTextureList::GameBgTex);
	LoadTexture("Res/Enemy.png", TEXTURE_CATEGORY_GAME, GameCategoryTextureList::GameEnemyTex);

	ChangeSceneStep(SceneStep::MainStep);
}

void MainGameScene()
{
}

SceneId FinishGameScene()
{
	ReleaseCategoryTexture(TEXTURE_CATEGORY_GAME);

	return SceneId::GameScene;
}

