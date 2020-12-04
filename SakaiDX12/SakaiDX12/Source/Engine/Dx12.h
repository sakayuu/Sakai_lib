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

class DX_Init
{
private: //エイリアス
	template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	
public: //メンバ関数（public）
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="win">ウィンドウ情報</param>
	void Initialize(Window* win);

	//描画前処理
	void BeginDraw();

	//描画後処理
	void EndDraw();

	//レンダーターゲットのクリア
	void ClearRenderTarget();

	//深度バッファのクリア
	void ClearDepthBuffer();

public: //ゲッター
	//デバイスを渡す
	ID3D12Device* GetDevice() { return device.Get(); }
	//コマンドリストを渡す
	ID3D12GraphicsCommandList* GetCommandList() { return cmdList.Get(); };
	//スワップチェーンを渡す
	IDXGISwapChain4* GetSwapchain() { return swapchain.Get(); };

private: //メンバ変数（private）
	//ウィンドウズアプリケーション管理
	Window* win;
	//DXGIまわり
	ComPtr<IDXGIFactory6> dxgiFactory = nullptr; //DXGIインターフェース
	ComPtr<IDXGISwapChain4> swapchain = nullptr; //スワップチェーン
	//DrectX12まわり
	ComPtr<ID3D12Device> device = nullptr; //デバイス
	ComPtr<ID3D12GraphicsCommandList> cmdList = nullptr; //コマンドリスト
	ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr; //コマンドアロケーター
	ComPtr<ID3D12CommandQueue> cmdQueue = nullptr; //コマンドキュー
	//表示に関わるバッファまわり
	std::vector<ComPtr<ID3D12Resource>> backBuffers; //バックバッファ(2つ以上…スワップチェインが確保)
	ComPtr<ID3D12Resource> depthBuffer = nullptr; //深度バッファ
	ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr; //レンダーターゲット用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr; //深度バッファビュー用デスクリプタヒープ
	//std::unique_ptr<CD3DX12_VIEWPORT> viewport; //ビューポート
	//std::unique_ptr<CD3DX12_RECT> scissorrect; //シザー矩形
	//フェンス
	ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;

private: //メンバ関数（private）
	//DXGIまわり初期化
	bool InitializeDXGIDevice();

	//スワップチェーンの生成
	bool CreateSwapChain();

	//コマンドまわり初期化
	bool InitializeCommand();

	//最終的なレンダーターゲットの生成
	bool CreateFinalRenderTargets();
	
	//デプスステンシルビューの生成
	bool CreateDepthStencilView();
	
	/// <summary>
	/// フェンス生成
	/// </summary>
	/// <returns>成否</returns>
	bool CreateFence();
};
