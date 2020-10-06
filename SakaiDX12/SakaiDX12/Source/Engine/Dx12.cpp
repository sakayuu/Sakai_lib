#include "Dx12.h"
#include <d3dx12.h>
//#include<cassert>

#pragma comment(lib,"DirectXTex.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;

namespace {
	///モデルのパスとテクスチャのパスから合成パスを得る
	///@param modelPath アプリケーションから見たpmdモデルのパス
	///@param texPath PMDモデルから見たテクスチャのパス
	///@return アプリケーションから見たテクスチャのパス
	std::string GetTexturePathFromModelAndTexPath(const std::string& modelPath, const char* texPath) {
		//ファイルのフォルダ区切りは\と/の二種類が使用される可能性があり
		//ともかく末尾の\か/を得られればいいので、双方のrfindをとり比較する
		//int型に代入しているのは見つからなかった場合はrfindがepos(-1→0xffffffff)を返すため
		int pathIndex1 = modelPath.rfind('/');
		int pathIndex2 = modelPath.rfind('\\');
		auto pathIndex = max(pathIndex1, pathIndex2);
		auto folderPath = modelPath.substr(0, pathIndex + 1);
		return folderPath + texPath;
	}

	///ファイル名から拡張子を取得する
	///@param path 対象のパス文字列
	///@return 拡張子
	string GetExtension(const std::string& path) {
		int idx = path.rfind('.');
		return path.substr(idx + 1, path.length() - idx - 1);
	}

	///ファイル名から拡張子を取得する(ワイド文字版)
	///@param path 対象のパス文字列
	///@return 拡張子
	wstring GetExtension(const std::wstring& path) {
		int idx = path.rfind(L'.');
		return path.substr(idx + 1, path.length() - idx - 1);
	}

	///テクスチャのパスをセパレータ文字で分離する
	///@param path 対象のパス文字列
	///@param splitter 区切り文字
	///@return 分離前後の文字列ペア
	pair<string, string>
		SplitFileName(const std::string& path, const char splitter = '*') {
		int idx = path.find(splitter);
		pair<string, string> ret;
		ret.first = path.substr(0, idx);
		ret.second = path.substr(idx + 1, path.length() - idx - 1);
		return ret;
	}

	///string(マルチバイト文字列)からwstring(ワイド文字列)を得る
	///@param str マルチバイト文字列
	///@return 変換されたワイド文字列
	std::wstring GetWideStringFromString(const std::string& str) {
		//呼び出し1回目(文字列数を得る)
		auto num1 = MultiByteToWideChar(CP_ACP,
			MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
			str.c_str(), -1, nullptr, 0);

		std::wstring wstr;//stringのwchar_t版
		wstr.resize(num1);//得られた文字列数でリサイズ

		//呼び出し2回目(確保済みのwstrに変換文字列をコピー)
		auto num2 = MultiByteToWideChar(CP_ACP,
			MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
			str.c_str(), -1, &wstr[0], num1);

		assert(num1 == num2);//一応チェック
		return wstr;
	}
	///デバッグレイヤーを有効にする
	void EnableDebugLayer() {
		ComPtr<ID3D12Debug> debugLayer = nullptr;
		auto result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
		debugLayer->EnableDebugLayer();
	}
}


DX_Init::DX_Init(HWND hwnd)
{
#ifdef _DEBUG
	//デバッグレイヤーをオンに
	EnableDebugLayer();
#endif

	if (!InitializeDXGIDevice()){
		assert(0);
	}

	if (!InitializeCommand()){
		assert(0);
	}

	CreateSwapChain(hwnd);
	CreateFinalRenderTargets();
	CreateSceneView();

	CreateTextureLoaderTable();
	CreateDepthStencilView();

	if (!CreateFence()) {
		assert(0);
	}
}

DX_Init::~DX_Init()
{
}

HRESULT DX_Init::CreateDepthStencilView()
{
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	auto result = _swapchain->GetDesc1(&desc);

	D3D12_RESOURCE_DESC resdesc = {};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resdesc.DepthOrArraySize = 1;
	resdesc.Width = desc.Width;
	resdesc.Height = desc.Height;
	resdesc.Format = DXGI_FORMAT_D32_FLOAT; //深度値フォーマット
	resdesc.SampleDesc.Count = 1;
	resdesc.SampleDesc.Quality = 0;
	resdesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; //デプスステンシル
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resdesc.MipLevels = 1;
	resdesc.Alignment = 0;

	auto depthHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	CD3DX12_CLEAR_VALUE depthClearValue(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	result = _dev->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, //デプス書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(_depthBuffer.ReleaseAndGetAddressOf()));

	//深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {}; //深度に使うよという事がわかればいい
	dsvHeapDesc.NumDescriptors = 1; //深度ビュー1つのみ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; //デプスステンシルビューとして使う
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	result = _dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(_dsvHeap.ReleaseAndGetAddressOf()));

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; //デプス値に32bit使用
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; //2Dテクスチャ
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE; //フラグは特になし
	_dev->CreateDepthStencilView(
		_depthBuffer.Get(),
		&dsvDesc,
		_dsvHeap->GetCPUDescriptorHandleForHeapStart());

	return result;
}

void DX_Init::CreateTextureLoaderTable()
{
	_loadLambdaTable["sph"] = _loadLambdaTable["spa"] = _loadLambdaTable["bmp"] = _loadLambdaTable["png"] = _loadLambdaTable["jpg"] = [](const wstring& path, TexMetadata* meta, ScratchImage& img)->HRESULT {
		return LoadFromWICFile(path.c_str(), (WIC_FLAGS)0, meta, img);
	};

	_loadLambdaTable["tga"] = [](const wstring& path, TexMetadata* meta, ScratchImage& img)->HRESULT {
		return LoadFromTGAFile(path.c_str(), meta, img);
	};

	_loadLambdaTable["dds"] = [](const wstring& path, TexMetadata* meta, ScratchImage& img)->HRESULT {
		return LoadFromDDSFile(path.c_str(), (DDS_FLAGS)0, meta, img);
	};
}

ID3D12Resource* DX_Init::CreateTextureFromFire(const char* texpath)
{
	string texPath = texpath;
	//テクスチャのロード
	TexMetadata metadata = {};
	ScratchImage scratchImg = {};
	auto wtexpath = GetWideStringFromString(texPath);//テクスチャのファイルパス
	auto ext = GetExtension(texPath);//拡張子を取得
	auto result = _loadLambdaTable[ext](wtexpath,
		&metadata,
		scratchImg);

	auto img = scratchImg.GetImage(0, 0, 0); //生データ抽出

	//WriteToSubresourceで転送する用のヒープ設定
	auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(metadata.format, metadata.width, metadata.height, metadata.arraySize, metadata.mipLevels);

	ID3D12Resource* texbuff = nullptr;
	result = _dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,//特に指定なし
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texbuff)
	);

	result = texbuff->WriteToSubresource(0,
		nullptr,//全領域へコピー
		img->pixels,//元データアドレス
		img->rowPitch,//1ラインサイズ
		img->slicePitch//全サイズ
	);

	return texbuff;
}

bool DX_Init::InitializeDXGIDevice()
{
	HRESULT result = S_FALSE;

#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugController;
	//デバッグレイヤーをオンに
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))){
		debugController->EnableDebugLayer();
	}
#endif

#pragma region アダプタの列挙
	//DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&_dxgiFactory));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	//アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter1>> adapters;
	//ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter1> tmpAdapter;
	for (int i = 0; _dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
		adapters.push_back(tmpAdapter); //動的配列に追加する
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc); //アダプターの情報を取得

		// ソフトウェアデバイスを回避
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			continue;
		}

		std::wstring strDesc = adesc.Description; //アダプター名
		//Microsoft Basic Render Driver,Intel UHD Graphics を回避
		if (strDesc.find(L"Microsoft") == std::wstring::npos
			&& strDesc.find(L"Intel") == std::wstring::npos)
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
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&_dev));
		if (SUCCEEDED(result))
		{
			//デバイスを生成出来た時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
	if (FAILED(result)) {
		assert(0);
		return false;
	}
#pragma endregion

	return true;
}

bool DX_Init::CreateSwapChain(const HWND& hwnd)
{

#pragma region スワップチェーン
	//各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = _winSize.cx;
	swapchainDesc.Height = _winSize.cy;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //色情報の書式
	swapchainDesc.SampleDesc.Count = 1; //マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; //バックバッファ用
	swapchainDesc.BufferCount = 2; //バッファ数を2つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; //フリップ後は破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	auto result = _dxgiFactory->CreateSwapChainForHwnd(
		_cmdQueue.Get(),
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)_swapchain.ReleaseAndGetAddressOf());
#pragma endregion

	return result;
}

bool DX_Init::InitializeCommand()
{
	HRESULT result = S_FALSE;

#pragma region コマンドリスト
	//コマンドアロケーターを生成
	result = _dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAllocator));
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//コマンドリストを生成
	result = _dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&_cmdList));
	if (FAILED(result)) {
		assert(0);
		return result;
	}
#pragma endregion

#pragma region コマンドキュー
	//標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	result = _dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&_cmdQueue));
	if (FAILED(result)) {
		assert(0);
		return result;
	}
#pragma endregion

	return true;
}

HRESULT DX_Init::CreateSceneView()
{
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	auto result = _swapchain->GetDesc1(&desc);

	//定数バッファ作成
	result = _dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(SceneData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(_sceneConstBuff.ReleaseAndGetAddressOf())
	);

	_mappedSceneData = nullptr;//マップ先を示すポインタ
	result = _sceneConstBuff->Map(0, nullptr, (void**)&_mappedSceneData);//マップ

	XMFLOAT3 eye(0, 15, -15);
	XMFLOAT3 target(0, 15, 0);
	XMFLOAT3 up(0, 1, 0);
	_mappedSceneData->view = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	_mappedSceneData->proj = XMMatrixPerspectiveFovLH(XM_PIDIV4,//画角は45°
		static_cast<float>(desc.Width) / static_cast<float>(desc.Height),//アス比
		0.1f,//近い方
		1000.0f//遠い方
	);
	_mappedSceneData->eye = eye;

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NodeMask = 0;//マスクは0
	descHeapDesc.NumDescriptors = 1;//
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;//デスクリプタヒープ種別
	result = _dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(_sceneDescHeap.ReleaseAndGetAddressOf()));//生成

	////デスクリプタの先頭ハンドルを取得しておく
	auto heapHandle = _sceneDescHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = _sceneConstBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = _sceneConstBuff->GetDesc().Width;
	//定数バッファビューの作成
	_dev->CreateConstantBufferView(&cbvDesc, heapHandle);
	return result;
}

HRESULT DX_Init::CreateFinalRenderTargets()
{
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	auto result = _swapchain->GetDesc1(&desc);

#pragma region レンダーターゲットビュー
	//各種設定をしてディスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; //レンダーターゲットビュー
	heapDesc.NumDescriptors = 2; //裏表の2つ

	result = _dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_rtvHeaps));

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = _swapchain->GetDesc(&swcDesc);
	//裏表の2つ分について
	_backBuffers.resize(swcDesc.BufferCount);


	for (int i = 0; i < 2; i++)
	{
		//スワップチェーンからバッファを取得
		result = _swapchain->GetBuffer(i, IID_PPV_ARGS(&_backBuffers[i]));
		//ディスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE handle =
			_rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		//裏か表かでアドレスがずれる
		handle.ptr += i * _dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
		//レンダーターゲットビューの生成
		_dev->CreateRenderTargetView(
			_backBuffers[i],
			nullptr,
			handle);
	}
#pragma endregion

	return result;
}

bool DX_Init::CreateFence()
{
	HRESULT result = S_FALSE;

#pragma region フェンス
	//フェンスの生成
	result = _dev->CreateFence(_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	if (FAILED(result)) {
		assert(0);
		return false;
	}
#pragma endregion

	return true;
}

void DX_Init::BeginDraw()
{
#pragma region リソースバリア
	//バックバッファの番号を取得
	UINT bbIndex = _swapchain->GetCurrentBackBufferIndex();

	// 1.リソースバリアを変更
	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = _backBuffers[bbIndex]; //バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; //表示から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; //描画
	_cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(_rtvHeaps->GetCPUDescriptorHandleForHeapStart(), bbIndex, _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(_dsvHeap->GetCPUDescriptorHandleForHeapStart());
	// レンダーターゲットをセット
	_cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	ClearRenderTarget();
	ClearDepthBuffer();

	// ビューポートの設定
	_cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, Window::window_width, Window::window_height));
	// シザリング矩形の設定
	_cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, Window::window_width, Window::window_height));
}

void DX_Init::SetScene()
{
	//現在のシーン(ビュープロジェクション)をセット
	ID3D12DescriptorHeap* sceneheaps[] = { _sceneDescHeap.Get() };
	_cmdList->SetDescriptorHeaps(1, sceneheaps);
	_cmdList->SetGraphicsRootDescriptorTable(0, _sceneDescHeap->GetGPUDescriptorHandleForHeapStart());
}

void DX_Init::EndDraw()
{
	//バックバッファの番号を取得(2つなので0番か1番)
	UINT bbIndex = _swapchain->GetCurrentBackBufferIndex();

#pragma region リソースバリア

	// 1.リソースバリアを変更
	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = _backBuffers[bbIndex]; //バックバッファを指定

	// 4.リソースバリアを戻す
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; //描画
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; //表示
	_cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region コマンドのフラッシュ
	//命令のクローズ
	_cmdList->Close();

	//コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { _cmdList.Get() }; //コマンドリストの配列
	_cmdQueue->ExecuteCommandLists(1, cmdLists);
	//コマンドリストの実行完了を待つ
	_cmdQueue->Signal(_fence, ++_fenceVal);
	if (_fence->GetCompletedValue() != _fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		_fence->SetEventOnCompletion(_fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	_cmdAllocator->Reset(); //キューをクリア
	_cmdList->Reset(_cmdAllocator.Get(), nullptr);
#pragma endregion

	_swapchain->Present(1, 0);
}

void DX_Init::ClearRenderTarget()
{
	UINT bbIndex = _swapchain->GetCurrentBackBufferIndex();
	// 2.画面クリアコマンド
	//レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(_rtvHeaps->GetCPUDescriptorHandleForHeapStart(), bbIndex, _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	//_cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
	//全画面クリア(RGB)
	float clearColor[] = { 0.1f,0.25f,0.5f,0.0f };
	_cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
}

void DX_Init::ClearDepthBuffer()
{
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(_dsvHeap->GetCPUDescriptorHandleForHeapStart());
	// 深度バッファのクリア
	_cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

ComPtr<ID3D12Resource> DX_Init::GetTextureByPath(const char* texpath)
{
	auto it = _textureTable.find(texpath);
	if (it != _textureTable.end()) {
		//テーブル内にあればロードせずマップ内のリソースを返す
		return _textureTable[texpath];
	}
	return ComPtr<ID3D12Resource>(CreateTextureFromFire(texpath));
}

ComPtr<ID3D12Device> DX_Init::GetDevice() {
	return _dev;
}

ComPtr<ID3D12GraphicsCommandList> DX_Init::GetCommandList() {
	return _cmdList;
}

ComPtr<IDXGISwapChain4> DX_Init::GetSwapchain() {
	return _swapchain;
}