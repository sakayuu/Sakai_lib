#include "Dx12.h"
#include <d3dx12.h>
//#include<cassert>
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx12.h"

#pragma comment(lib,"DirectXTex.lib")

using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;

DX_Init::~DX_Init()
{
	Finalize();
}

void DX_Init::Initialize(Window* win)
{
	assert(win);

	this->win = win;

	//各種初期化処理
	if (!InitializeDXGIDevice()) {
		assert(0);
	}
	if (!InitializeCommand()) {
		assert(0);
	}
	if (!CreateSwapChain()) {
		assert(0);
	}
	if (!CreateFinalRenderTargets()) {
		assert(0);
	}
	if (!CreateDepthStencilView()) {
		assert(0);
	}
	if (!CreateFence()) {
		assert(0);
	}
	// imgui初期化
	if (!InitImgui()) {
		assert(0);
	}
}

void DX_Init::Finalize()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DX_Init::BeginDraw()
{
#pragma region リソースバリア
	//バックバッファの番号を取得
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	// 1.リソースバリアを変更（表示状態→描画対象）
	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get(); //バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; //表示から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; //描画
	cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(), bbIndex, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	// レンダーターゲットをセット
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	ClearRenderTarget(); //全画面クリア
	ClearDepthBuffer();	 //深度バッファクリア

	//無いと動かない
	auto VIEWPORT = CD3DX12_VIEWPORT(0.0f, 0.0f, Window::window_width, Window::window_height);
	auto RECT = CD3DX12_RECT(0, 0, Window::window_width, Window::window_height);
	// ビューポートの設定
	cmdList->RSSetViewports(1, &VIEWPORT);
	// シザリング矩形の設定
	cmdList->RSSetScissorRects(1, &RECT);

	// imgui開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void DX_Init::EndDraw()
{
	// imgui描画
	ImGui::Render();
	ID3D12DescriptorHeap* ppHeaps[] = { imguiHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList.Get());

	//バックバッファの番号を取得(2つなので0番か1番)
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

#pragma region リソースバリア
	// 1.リソースバリアを変更
	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get(); //バックバッファを指定
	// 4.リソースバリアを戻す（描画対象→表示状態）
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; //描画
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; //表示
	cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region コマンドのフラッシュ
	//命令のクローズ
	cmdList->Close();

	//コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { cmdList.Get() }; //コマンドリストの配列
	cmdQueue->ExecuteCommandLists(1, cmdLists);
	//コマンドリストの実行完了を待つ
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	cmdAllocator->Reset(); //キューをクリア
	cmdList->Reset(cmdAllocator.Get(), nullptr);
#pragma endregion

	swapchain->Present(1, 0); //バッファをフリップ
}

void DX_Init::ClearRenderTarget()
{
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();
	// 2.画面クリアコマンド
	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(), bbIndex, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	//全画面クリア(RGB)
	float clearColor[] = { 0.1f,0.25f,0.5f,0.0f };
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
}

void DX_Init::ClearDepthBuffer()
{
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	// 深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

bool DX_Init::InitializeDXGIDevice()
{
	HRESULT result = S_FALSE;

#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugController;
	//デバッグレイヤーをオンに
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.ReleaseAndGetAddressOf())))) {
		debugController->EnableDebugLayer();
	}
#endif

#pragma region アダプタの列挙
	//DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(dxgiFactory.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	//アダプターの列挙用
	vector<ComPtr<IDXGIAdapter1>> adapters;
	//ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter1> tmpAdapter;
	for (int i = 0; dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
		adapters.push_back(tmpAdapter); //動的配列に追加する
	}

	for (int i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc); //アダプターの情報を取得

		// ソフトウェアデバイスを回避
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			continue;
		}

		std::wstring strDesc = adesc.Description; //アダプター名
		//Microsoft Basic Render Driver,Intel UHD Graphics を回避
		if (strDesc.find(L"Microsoft") == std::wstring::npos &&
			strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i]; //採用
			break;
		}
	}
#pragma endregion

#pragma region デバイスの生成
	//対応レベルの配列
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	result = S_FALSE;
	for (int i = 0; i < _countof(levels); i++)
	{
		//採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(device.ReleaseAndGetAddressOf()));
		if (SUCCEEDED(result)) {
			//デバイスを生成出来た時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
	assert(SUCCEEDED(result));
#pragma endregion

	return true;
}

bool DX_Init::CreateSwapChain()
{
	HRESULT result = S_FALSE;

#pragma region スワップチェーン
	//各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = Window::window_width;
	swapchainDesc.Height = Window::window_height;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //色情報の書式
	swapchainDesc.SampleDesc.Count = 1; //マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; //バックバッファ用
	swapchainDesc.BufferCount = 2; //バッファ数を2つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; //フリップ後は破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	ComPtr<IDXGISwapChain1> swapchain1;
	HWND hwnd = win->GetHwnd();
	result = dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1);
	assert(SUCCEEDED(result));
	swapchain1.As(&swapchain);
#pragma endregion

	return true;
}

bool DX_Init::InitializeCommand()
{
	HRESULT result = S_FALSE;

#pragma region コマンドリスト
	//コマンドアロケーターを生成
	result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(cmdAllocator.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	//コマンドリストを生成
	result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(cmdList.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
#pragma endregion

#pragma region コマンドキュー
	//標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	result = device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(cmdQueue.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
#pragma endregion

	return true;
}

bool DX_Init::CreateFinalRenderTargets()
{
	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapchain->GetDesc(&swcDesc);
	assert(SUCCEEDED(result));

#pragma region レンダーターゲットビュー
	//各種設定をしてディスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; //レンダーターゲットビュー
	heapDesc.NumDescriptors = swcDesc.BufferCount; //裏表の2つ
	result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(rtvHeaps.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	//裏表の2つ分について
	backBuffers.resize(swcDesc.BufferCount);
	for (int i = 0; i < backBuffers.size(); i++) {
		//スワップチェーンからバッファを取得
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(backBuffers[i].ReleaseAndGetAddressOf()));
		assert(SUCCEEDED(result));

		//ディスクリプタヒープのハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(), i, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		//レンダーターゲットビューの生成
		device->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			handle);
	}
#pragma endregion

	return true;
}

bool DX_Init::CreateDepthStencilView()
{
	HRESULT result = S_FALSE;

	//リソース設定
	CD3DX12_RESOURCE_DESC resdesc = {};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resdesc.DepthOrArraySize = 1;
	resdesc.Width = Window::window_width;
	resdesc.Height = Window::window_height;
	resdesc.Format = DXGI_FORMAT_D32_FLOAT; //深度値フォーマット
	resdesc.SampleDesc.Count = 1;
	resdesc.SampleDesc.Quality = 0;
	resdesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; //デプスステンシル
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resdesc.MipLevels = 1;
	resdesc.Alignment = 0;

	//無いと動かない
	auto depthHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_CLEAR_VALUE depthClearValue(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, //デプス書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(depthBuffer.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	//深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {}; //深度に使うよという事がわかればいい
	dsvHeapDesc.NumDescriptors = 1; //深度ビュー1つのみ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; //デプスステンシルビューとして使う
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(dsvHeap.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; //デプス値に32bit使用
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; //2Dテクスチャ
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE; //フラグは特になし
	device->CreateDepthStencilView(
		depthBuffer.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());

	return true;
}

bool DX_Init::CreateFence()
{
	HRESULT result = S_FALSE;

#pragma region フェンス
	//フェンスの生成
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
#pragma endregion

	return true;
}

bool DX_Init::InitImgui()
{
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&imguiHeap));
	assert(SUCCEEDED(result));
	
	// スワップチェーンの情報を取得
	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapchain->GetDesc(&swcDesc);
	assert(SUCCEEDED(result));

	if (ImGui::CreateContext() == nullptr) {
		assert(0);
		return false;
	}
	if (!ImGui_ImplWin32_Init(win->GetHwnd())) {
		assert(0);
		return false;
	}
	if (!ImGui_ImplDX12_Init(
		GetDevice(),
		swcDesc.BufferCount,
		swcDesc.BufferDesc.Format,
		imguiHeap.Get(),
		imguiHeap->GetCPUDescriptorHandleForHeapStart(),
		imguiHeap->GetGPUDescriptorHandleForHeapStart())) {
		assert(0);
		return false;
	}

	return true;
}
