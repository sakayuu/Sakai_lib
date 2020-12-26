#pragma once
#include <DirectXMath.h>
#include <d3dx12.h>

class Light
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferDataB2
	{
		XMVECTOR lightV;     //ライトへの方向を表すベクトル
		XMFLOAT3 lightColor; //ライトの色
	};

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static Light* Create();

private: // メンバ変数
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// ライト光線方向（単位ベクトル）
	XMVECTOR lightDir = { 1,0,0,0 };
	// ライト色
	XMFLOAT3 lightColor = { 1,1,1 };
	// ダーティフラグ
	bool dirty = false;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 定数バッファ転送
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// ライト方向をセット
	/// </summary>
	/// <param name="lightDir">ライト方向</param>
	void SetLightDir(const XMVECTOR& lightDir);

	/// <summary>
	/// ライト色をセット
	/// </summary>
	/// <param name="lightColor">ライト色</param>
	void SetLightColor(const XMFLOAT3& lightColor);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	/// <param name="rootParameterIndex">ルートパラメータ</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);
};
