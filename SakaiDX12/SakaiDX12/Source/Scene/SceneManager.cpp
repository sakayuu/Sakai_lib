#include "SceneManager.h"


SceneManager::SceneManager(DX_Init* dx_Init, Input* input, Audio* audio)
{
	this->dx_Init = dx_Init;
	this->input = input;
	this->audio = audio;
	AddScene();
	score = new Score();
	SetScene("Title");
}

SceneManager::~SceneManager()
{
	dx_Init = nullptr;
	input = nullptr;
	audio = nullptr;
	for (auto& s : scenes) {
		delete(s.second);
		s.second = nullptr;
	}
	scenes.clear();
}

void SceneManager::Initialize()
{
	currentScene->Initialize(dx_Init, input, audio,score);
}

void SceneManager::Update()
{
	if (currentScene->IsSceneEnd()) {
		SetScene(currentScene->IsNextScene());
	}
	currentScene->Update();
}

void SceneManager::Draw()
{
	currentScene->Draw();
}

void SceneManager::AddScene()
{
	scenes.emplace("Title", new TitleScene("Title"));
	scenes.emplace("GamePlay", new GameScene("GamePlay"));
	scenes.emplace("GameOver", new GameOverScene("GameOver"));
	scenes.emplace("GameClear", new GameClearScene("GameClear"));
}

void SceneManager::SetScene(const std::string& sceneName)
{
	currentScene = scenes[sceneName];
	Initialize();
}

void SceneManager::SetScore()
{

}
