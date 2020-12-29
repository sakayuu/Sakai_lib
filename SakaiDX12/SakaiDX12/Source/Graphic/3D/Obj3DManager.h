#pragma once
#include "Object3d.h"

class Obj3DManager
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // メンバ関数
	Obj3DManager();
	~Obj3DManager();

	void Update();

	void Draw();

	void CreateLight(UINT num);

	void SetLightColor(UINT num, XMFLOAT3 color) { light[num]->SetLightColor(color); }

	void SetLightDir(UINT num, XMVECTOR dir) { light[num]->SetLightDir(dir); }

	void LightUpdate();

	// モデルの追加
	void AddModel(const std::string& modelName, const std::string& modelPath, bool smoothing);

	// モデルのセット
	void SetModel(const std::string& name, const std::string& modelName);

	// モデルの削除
	void DeleteModel(const std::string& modelName);

	// オブジェクトの追加
	void AddObj3D(const std::string& name, const std::string& modelName);

	// オブジェクトの削除
	void DeleteObj3D(const std::string& name);

public: // ゲッター
	// 座標の取得
	const XMFLOAT3& GetPosition(const std::string& name) { return objects[name]->GetPosition(); }

	// 回転角の取得
	const XMFLOAT3& GetRotation(const std::string& name) { return objects[name]->GetRotation(); }

public: // セッター
	// 座標の設定
	void SetPosition(const std::string& name, XMFLOAT3 position) { objects[name]->SetPosition(position); }

	// 回転角の設定
	void SetRotation(const std::string& name, XMFLOAT3 rotation) { objects[name]->SetRotation(rotation); }

	// スケールの設定
	void SetScale(const std::string& name, XMFLOAT3 scale) { objects[name]->SetScale(scale); }

	// ビルボードフラグのセット
	void SetBillboard(const std::string& name, bool isBillboard) { objects[name]->SetBillboard(isBillboard); }

private:
	std::vector<Light*> light;
	std::unordered_map <std::string, Model*> models;
	std::unordered_map <std::string, Object3d*> objects;

};
