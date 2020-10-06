#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <DirectXTex.h>
#include <map>
#include <unordered_map>
#include"../Engine/Window.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class DX_Init
{
private:
	SIZE _winSize;
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DXGIまわり
	ComPtr<IDXGIFactory6> _dxgiFactory = nullptr; //DXGIインターフェース
	ComPtr<IDXGISwapChain4> _swapchain = nullptr; //スワップチェーン

	//DrectX12まわり
	ComPtr<ID3D12Device> _dev = nullptr; //デバイス
	ComPtr<ID3D12CommandAllocator> _cmdAllocator = nullptr; //コマンドアロケーター
	ComPtr<ID3D12GraphicsCommandList> _cmdList = nullptr; //コマンドリスト
	ComPtr<ID3D12CommandQueue> _cmdQueue = nullptr; //コマンドキュー

	//表示に関わるバッファまわり
	ComPtr<ID3D12Resource> _depthBuffer = nullptr; //深度バッファ
	std::vector<ID3D12Resource*> _backBuffers; //バックバッファ(2つ以上…スワップチェインが確保)
	ComPtr<ID3D12DescriptorHeap> _rtvHeaps = nullptr; //レンダーターゲット用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> _dsvHeap = nullptr; //深度バッファビュー用デスクリプタヒープ
	std::unique_ptr<D3D12_VIEWPORT> _viewport; //ビューポート
	std::unique_ptr<D3D12_RECT> _scissorrect; //シザー矩形

	//シーンを構成するバッファまわり
	ComPtr<ID3D12Resource> _sceneConstBuff = nullptr;

	struct SceneData {
		DirectX::XMMATRIX view;//ビュー行列
		DirectX::XMMATRIX proj;//プロジェクション行列
		DirectX::XMFLOAT3 eye;//視点座標
	};
	SceneData* _mappedSceneData;
	ComPtr<ID3D12DescriptorHeap> _sceneDescHeap = nullptr;

	//フェンス
	ID3D12Fence* _fence = nullptr;
	UINT64 _fenceVal = 0;

	//最終的なレンダーターゲットの生成
	HRESULT CreateFinalRenderTargets();
	//デプスステンシルビューの生成
	HRESULT CreateDepthStencilView();

	//スワップチェーンの生成
	bool CreateSwapChain(const HWND& hwnd);

	//DXGIまわり初期化
	bool InitializeDXGIDevice();

	//コマンドまわり初期化
	bool InitializeCommand();

	//ビュープロジェクション用ビューの生成
	HRESULT CreateSceneView();

	/// <summary>
	/// フェンス生成
	/// </summary>
	/// <returns>成否</returns>
	bool CreateFence();
public:
	/// <summary>
	/// レンダーターゲットのクリア
	/// </summary>
	void ClearRenderTarget();

	/// <summary>
	/// 深度バッファのクリア
	/// </summary>
	void ClearDepthBuffer();

	//ロード用テーブル
	using LoadLambda_t = std::function<HRESULT(const std::wstring& path, DirectX::TexMetadata*, DirectX::ScratchImage&)>;
	std::map<std::string, LoadLambda_t> _loadLambdaTable;
	//テクスチャテーブル
	std::unordered_map<std::string, ComPtr<ID3D12Resource>> _textureTable;
	//テクスチャローダテーブル
	void CreateTextureLoaderTable();
	//テクスチャ名からテクスチャバッファ作成、中身をコピー
	ID3D12Resource* CreateTextureFromFire(const char* texpath);

public:
	DX_Init(HWND hwnd);
	~DX_Init();

	void Update();
	void BeginDraw();
	void EndDraw();

	///テクスチャパスから必要なテクスチャバッファへのポインタを返す
	/// @param texpath テクスチャファイルパス
	ComPtr<ID3D12Resource> GetTextureByPath(const char* texpath);

	//デバイスを渡す
	ComPtr<ID3D12Device> GetDevice();
	//コマンドリストを渡す
	ComPtr<ID3D12GraphicsCommandList> GetCommandList();
	//スワップチェーンを渡す
	ComPtr<IDXGISwapChain4> GetSwapchain();

	void SetScene();
};
