#include "../Engine/Graphics.h"
#include "../Engine/Input.h"
#include "../Texture/Texture.h"
#include "GameOverScene.h"

// ゲームオーバーシーンの初期化
void InitGameOverScene();
// ゲームオーバーシーンのメイン
void MainGameOverScene();
// ゲームオーバーシーンの終了
SceneId FinishGameOverScene();

SceneId UpdateGameOverScene()
{
	switch (GetCurrentSceneStep())
	{
	case SceneStep::InitStep:
		InitGameOverScene();
		break;
	case SceneStep::MainStep:
		MainGameOverScene();
		break;
	case SceneStep::EndStep:
		return FinishGameOverScene();
		break;
	}

	return SceneId::GameOverScene;
}

void DrawGameOverScene()
{
}

void InitGameOverScene()
{
	LoadTexture("Res/GameOverBg.png", TEXTURE_CATEGORY_GAME_OVER, GameOverCategoryTextureList::GameOverBgTex);
	ChangeSceneStep(SceneStep::MainStep);
}

void MainGameOverScene()
{
}

SceneId FinishGameOverScene()
{
	ReleaseCategoryTexture(TEXTURE_CATEGORY_GAME_OVER);

	return SceneId::GameOverScene;
}

