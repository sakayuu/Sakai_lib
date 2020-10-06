#include "../Engine/Graphics.h"
#include "../Engine/Input.h"
#include "../Texture/Texture.h"
#include "TitleScene.h"

// タイトルシーンの初期化
void InitTitleScene();
// タイトルシーンのメイン処理
void MainTitleScene();
// タイトルシーンの終了
SceneId FinishTitleScene();

struct Sprite
{
	float m_TexturePosX;	// テクスチャ座標(左上X)
	float m_TexturePosY;	// テクスチャ座標(左上Y)
	float m_Width;			// 矩形サイズ(横)
	float m_Height;			// 矩形サイズ(縦)
	int m_CategoryId;
	int m_TextureId;
};

struct SpriteAnimation
{
	int m_ChangeFlame;
	int m_NextId;
};

struct SpriteAnimationPlayer
{
	Sprite* Sprites;
	SpriteAnimation* Animations;
	int m_FlameCounter;
	int m_CurrentAnimationId;
};

Sprite BombWalkSprites[] = 
{
	// １枚目
	{ 0.0f, 0.0f, 128.0f, 128.0f,TEXTURE_CATEGORY_TITLE,TitleCategoryTextureList::TitleMenuGoGameTex },
	// ２枚目
	{ 128.0f, 0.0f, 128.0f, 128.0f,TEXTURE_CATEGORY_TITLE,TitleCategoryTextureList::TitleMenuGoGameTex },
	// ３枚目
	{ 256.0f, 0.0f, 128.0f, 128.0f,TEXTURE_CATEGORY_TITLE,TitleCategoryTextureList::TitleMenuGoGameTex },
	// ４枚目
	{ 374.0f, 0.0f, 128.0f, 128.0f,TEXTURE_CATEGORY_TITLE,TitleCategoryTextureList::TitleMenuGoGameTex },
	// ５枚目
	{ 512.0f, 0.0f, 128.0f, 128.0f,TEXTURE_CATEGORY_TITLE,TitleCategoryTextureList::TitleMenuGoGameTex },
};

SpriteAnimation BombWalkAnimations[] =
{
	{ 6, 1 },
	{ 6, 2 },
	{ 6, 3 },
	{ 6, 4 },
	{ 6, 0 },
};


// 雑バージョン
int g_FlameCounter = 0;
int g_ChangeFlame = 6;
int g_AnimationId = 0;
int g_MaxAnimation = 5;

SceneId UpdateTitleScene()
{
	switch (GetCurrentSceneStep())
	{
	case SceneStep::InitStep:
		InitTitleScene();
		break;
	case SceneStep::MainStep:
		MainTitleScene();
		break;
	case SceneStep::EndStep:
		return FinishTitleScene();
		break;
	}

	return SceneId::TitleScene;
}

void DrawTitleScene()
{
	DrawTextureUV(
		0, 
		0, 
		GetTexture(TEXTURE_CATEGORY_TITLE, TitleCategoryTextureList::TitleMenuGoGameTex), 
		BombWalkSprites[g_AnimationId].m_TexturePosX,	// テクスチャ座標(X)
		BombWalkSprites[g_AnimationId].m_TexturePosY,	// テクスチャ座標(Y)
		BombWalkSprites[g_AnimationId].m_Width,			// スプライトサイズ(横)
		BombWalkSprites[g_AnimationId].m_Height);		// スプライトサイズ(縦)
}

void InitTitleScene()
{
	LoadTexture("Res/TitleBg.png", TEXTURE_CATEGORY_TITLE, TitleCategoryTextureList::TitleBgTex);
	LoadTexture("Res/bomb_move.png", TEXTURE_CATEGORY_TITLE, TitleCategoryTextureList::TitleMenuGoGameTex);
	LoadTexture("Res/TitleMenuHelp.png", TEXTURE_CATEGORY_TITLE, TitleCategoryTextureList::TitleMenuGoHelpTex);

	ChangeSceneStep(SceneStep::MainStep);
}

void MainTitleScene()
{
	// フレームカウントする
	g_FlameCounter++;

	// 切り替え時間を超えたか調べる
	if (g_FlameCounter >= g_ChangeFlame)
	{
		// フレームカウントをリセット
		g_FlameCounter = 0;
		// IDを加算
		g_AnimationId++;

		// IDが最大アニメーション数を超えてないかチェック
		if (g_AnimationId >= g_MaxAnimation)
		{
			// 超えてたらリセット
			g_AnimationId = 0;
		}
	}
}

SceneId FinishTitleScene()
{
	ReleaseCategoryTexture(SceneId::TitleScene);

	return SceneId::TitleScene;
}

