#include "Window.h"
#include "Dx12.h"
#include "../Engine/SafeDelete.h"
#include "../Graphic/2D/Sprite.h"
#include "../Sound/Audio.h"
#include "../Scene/SceneManager.h"

class Engine
{
private:
	Window* window = nullptr;
	DX_Init* dx_Init = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;

	Sprite* sprite = nullptr;

	double fps = 0.0;

	//参照が無くなれば自動で解放
	SceneManager* sceneManager = nullptr;

	//シングルトン
	Engine() = default;
	~Engine() = default;
public:
#pragma region コンストラクタを制限
	Engine(const Engine&) = delete;
	Engine& operator= (const Engine&) = delete;
	Engine(const Engine&&) = delete;
	Engine& operator= (const Engine&&) = delete;
#pragma endregion

	//Engineのシングルトンインスタンスを得る
	static Engine& Instance();

	// 初期化
	bool Init();

	// ループ起動
	void Run();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 後処理
	void Terminate();

private:

	void Create();

};