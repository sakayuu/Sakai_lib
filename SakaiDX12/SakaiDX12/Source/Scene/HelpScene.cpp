#include "../Engine/Graphics.h"
#include "../Engine/Input.h"
#include "../Texture/Texture.h"
#include "HelpScene.h"

// ヘルプシーンの初期化
void InitHelpScene();
// ヘルプシーンのメイン
void MainHelpScene();
// ヘルプシーンの終了
SceneId FinishHelpScene();

SceneId UpdateHelpScene()
{
	switch (GetCurrentSceneStep())
	{
	case SceneStep::InitStep:
		InitHelpScene();
		break;
	case SceneStep::MainStep:
		MainHelpScene();
		break;
	case SceneStep::EndStep:
		return FinishHelpScene();
		break;
	}

	return SceneId::HelpScene;
}

void DrawHelpScene()
{
}

void InitHelpScene()
{
	LoadTexture("Res/HelpBg.png", TEXTURE_CATEGORY_HELP, HelpCategoryTextureList::HelpBgTex);

	ChangeSceneStep(SceneStep::MainStep);
}

void MainHelpScene()
{
}

SceneId FinishHelpScene()
{
	ReleaseCategoryTexture(TEXTURE_CATEGORY_HELP);

	return SceneId::HelpScene;
}

