#pragma once
#include <unordered_map>
#include "Scene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameOverScene.h"
#include "GameClearScene.h"
#include "../Game/Score.h"

class SceneManager
{
public:
	SceneManager(DX_Init* dx_Init, Input* input, Audio* audio);
	~SceneManager();

	void Initialize();

	void Update();

	void Draw();

	void AddScene();

	void SetScene(const std::string& sceneName);

	void SetScore();

private:
	std::unordered_map<std::string, Scene*> scenes;
	Scene* currentScene = nullptr;
	Score* score = nullptr;

	DX_Init* dx_Init = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
};
