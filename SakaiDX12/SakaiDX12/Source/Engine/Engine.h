#include "Window.h"
#include "Dx12.h"
#include "../Graphic/2D/Sprite.h"
#include "../Scene/Scene.h"

class Engine
{
private:
	Window* window = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	Sprite* sprite = nullptr;

	//参照が無くなれば自動で解放
	DX_Init* dx_Init = nullptr;
	Input* input = nullptr;
	GameScene* scene = nullptr;

	//シングルトン
	Engine();
	Engine(const Engine&) = delete;
	void operator= (const Engine&) = delete;

public:
	//Engineのシングルトンインスタンスを得る
	static Engine& Instance();

	//初期化
	bool Init();

	// ループ起動
	void Run();

	//描画
	//void Draw();

	//後処理
	void Terminate();

	~Engine();
};