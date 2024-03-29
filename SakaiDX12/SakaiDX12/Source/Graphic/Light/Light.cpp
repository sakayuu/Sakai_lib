#include "Light.h"

using namespace DirectX;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* Light::device = nullptr;

bool Light::StaticInitialize(ID3D12Device* device) {
	//再初期化チェック
	assert(!Light::device);
	//nullptrチェック
	assert(device);
	//静的メンバ変数のセット
	Light::device = device;

	return true;
}

Light* Light::Create() {
	// 3Dオブジェクトのインスタンスを生成
	Light* instance = new Light();
	// 初期化
	instance->Initialize();
	// 生成したインスタンスを返す
	return instance;
}

void Light::Initialize() {
	HRESULT result;

	// 無いと動かない
	auto HEAP_PROPERTIES = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto RESOURCE_DESC_Buffer = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB2) + 0xff) & ~0xff);
	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&HEAP_PROPERTIES,
		D3D12_HEAP_FLAG_NONE,
		&RESOURCE_DESC_Buffer,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) { assert(0); }

	// 定数バッファへデータ転送
	TransferConstBuffer();
}

void Light::TransferConstBuffer() {
	HRESULT result;
	// 定数バッファへデータ転送
	ConstBufferDataB2* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->lightV = -lightDir;
		constMap->lightColor = lightColor;
		constBuff->Unmap(0, nullptr);
	}
}

void Light::SetLightDir(const XMVECTOR& lightDir) {
	// 正規化してセット
	this->lightDir = XMVector3Normalize(lightDir);
	dirty = true;
}

void Light::SetLightColor(const XMFLOAT3& lightColor) {
	this->lightColor = lightColor;
	dirty = true;
}

void Light::Update() {
	// 値の更新があった時だけ定数バッファに転送する
	if (dirty) {
		TransferConstBuffer();
		dirty = false;
	}
}

void Light::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex) {
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff->GetGPUVirtualAddress());
}
