#include "../Engine/Graphics.h"
#include "../Engine/Input.h"
#include "../Texture/Texture.h"
#include "GameOverScene.h"

// ゲームクリアシーンの初期化
void InitGameClearScene();
// ゲームクリアシーンのメイン処理
void MainGameClearScene();
// ゲームクリアシーンの終了
SceneId FinishGameClearScene();

SceneId UpdateGameClearScene()
{
	switch (GetCurrentSceneStep())
	{
	case SceneStep::InitStep:
		InitGameClearScene();
		break;
	case SceneStep::MainStep:
		MainGameClearScene();
		break;
	case SceneStep::EndStep:
		return FinishGameClearScene();
		break;
	}

	return SceneId::GameClearScene;
}

void DrawGameClearScene()
{
}

void InitGameClearScene()
{
	LoadTexture("Res/GameClearBg.png", TEXTURE_CATEGORY_GAME_CLEAR, GameClearCategoryTextureList::GameClearBgTex);
	ChangeSceneStep(SceneStep::MainStep);
}

void MainGameClearScene()
{
}

SceneId FinishGameClearScene()
{
	ReleaseCategoryTexture(TEXTURE_CATEGORY_GAME_CLEAR);

	return SceneId::GameClearScene;
}

