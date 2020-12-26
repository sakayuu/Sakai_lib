﻿#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include <map>

#include "Model.h"
#include "../Camera/Camera.h"
#include "../Light/Light.h"

/// <summary>
/// 3Dオブジェクト
/// </summary>
class Object3d
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// パイプラインセット
	struct PipelineSet
	{
		// ルートシグネチャ
		ComPtr<ID3D12RootSignature> rootsignature;
		// パイプラインステートオブジェクト
		ComPtr<ID3D12PipelineState> pipelinestate;
	};

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		//XMMATRIX mat;	// ３Ｄ変換行列
		XMMATRIX viewproj;  //ビュープロジェクション行列
		XMMATRIX world;	    //ワールド行列
		XMFLOAT3 cameraPos; //カメラ座標（ワールド座標）
	};

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device, Camera* camera = nullptr);

	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="camera">カメラ</param>
	static void SetCamera(Camera* camera) {
		Object3d::camera = camera;
	}

	/// <summary>
	/// ライトのセット
	/// </summary>
	/// <param name="light">ライト</param>
	static void SetLight(Light* light) {
		Object3d::light = light;
	}

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	//static Object3d* Create(Model* model = nullptr);
	static Object3d* Create();

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// パイプライン
	static PipelineSet pipelineSet;
	// カメラ
	static Camera* camera;
	// ライト
	static Light* light;
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const std::vector<Light*>& light);

	void LightDraw(ID3D12GraphicsCommandList* cmdList, const std::vector<Light*>& light) {
		if (light.size() <= 0)
			return;
		for (int i = 0; i < light.size(); i++) {
			light[i]->Draw(cmdList, 3 + i);
		}
	}

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition(const std::string& key);
	
	/// <summary>
	/// 回転角の取得
	/// </summary>
	/// <returns>回転角</returns>
	const XMFLOAT3& GetRotation(const std::string& key);

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const std::string& key,XMFLOAT3 position);
	
	/// <summary>
	/// 回転角の設定
	/// </summary>
	/// <param name="rotation"></param>
	void SetRotation(const std::string& key, XMFLOAT3 rotation);

	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="position">スケール</param>
	void SetScale(const std::string& key, XMFLOAT3 scale);
	
	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model">モデル</param>
	//void SetModel(const std::string& modelname, Model* model);
	void AddModel(const std::string& modelname, Model* model);

	/// <summary>
	/// ビルボードフラグのセット
	/// </summary>
	/// <param name="isBillboard">ビルボードか</param>
	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }

private: // メンバ変数
	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	//// 色
	//XMFLOAT4 color = { 1,1,1,1 };
	//// ローカルスケール
	//XMFLOAT3 scale = { 1,1,1 };
	//// X,Y,Z軸回りのローカル回転角
	//XMFLOAT3 rotation = { 0,0,0 };
	//// ローカル座標
	//XMFLOAT3 position = { 0,0,0 };
	//// ローカルワールド変換行列
	//XMMATRIX matWorld;
	// 親オブジェクト
	Object3d* parent = nullptr;
	// モデル
	//Model* model = nullptr;
	std::unordered_map<std::string, Model*> models;
	// ビルボード
	bool isBillboard = false;
};

